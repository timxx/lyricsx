/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "lrcreader.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

static size_t _skipNewLine(const std::string &str, size_t begin)
{
	size_t pos = begin;
	for (; pos != str.length(); ++pos)
	{
		char ch = str[pos];
		if (ch != '\r' && ch != '\n')
			break;
	}

	return pos;
}

static std::string _trimString(const std::string &str)
{
	static const char *whitespace = " \t\r\n";

	size_t begin = str.find_first_not_of(whitespace);
	if (begin == std::string::npos)
		return "";

	size_t end = str.find_last_not_of(whitespace);

	return str.substr(begin, end - begin + 1);
}

LRCX_BEGIN_NS

LrcReader::LrcReader()
	: m_lrc(new LRC)
{
}

LrcReader::LrcReader(const std::string &file)
	: m_lrc(new LRC)
{
	setFile(file);
}

LrcReader::LrcReader(const char *buffer, int size)
	: m_lrc(new LRC)
{
	setBuffer(buffer, size);
}

LrcReader::~LrcReader()
{
}

void LrcReader::setFile(const std::string &file)
{
	m_file = file;

	// TODO:
	// deal with different encoding
	QFile qFile(m_file.c_str());
	if (qFile.open(QIODevice::ReadOnly))
	{
		QByteArray data = qFile.readAll();
		setBuffer(data.constData(), data.length());
	}
}

void LrcReader::setBuffer(const char *buffer, int size)
{
	if (size != -1)
		m_buffer.assign(buffer, size);
	else
		m_buffer.assign(buffer);
}

const LRC& LrcReader::getLrc() const
{
	return *(m_lrc.get());
}

const std::string& LrcReader::getFile() const
{
	return m_file;
}

bool LrcReader::parse()
{
	m_lrc->clear();

	size_t offset = 0;
	size_t lineNo = 0;

	while (offset < m_buffer.length())
	{
		++lineNo;
		size_t pos = m_buffer.find_first_of("\r\n", offset);

		std::string line;
		if (pos == std::string::npos)
		{
			line = m_buffer.substr(offset);
			offset = m_buffer.length();
		}
		else
		{
			line = m_buffer.substr(offset, pos - offset);
			offset = _skipNewLine(m_buffer, pos);
		}

		if (!parseLine(_trimString(line)))
		{
			qDebug() << "LrcReader::parse: invalid line" << lineNo;
		}
	}

	m_lrc->sort();

	return m_lrc->count() > 0;
}

bool LrcReader::parseLine(const std::string &line)
{
	if (line.empty() || line[0] != '[')
		return false;

	size_t pos = line.find(']');
	if (pos == std::string::npos || pos == 1) // invalid tag
		return false;

	--pos; // ']'
	bool isTimeTag = true;
	for (size_t i = 1; i < pos; ++i)
	{
		char ch = line[i];
		if (!isdigit(ch) && ch != ':' && ch != '.')
		{
			isTimeTag = false;
			break;
		}
	}

	std::string tag = line.substr(1, pos);

	if (isTimeTag)
	{
		std::vector<int> times;
		times.push_back(parseTime(tag));

		pos += 2; // skip first tag
		while (true)
		{
			size_t begin = line.find('[', pos);
			size_t end = line.find(']', pos);

			if (pos == std::string::npos || end == std::string::npos)
				break;

			pos = end;
			++pos, ++begin;
			tag = line.substr(begin, end - begin);
			times.push_back(parseTime(tag));
		}

		std::string lyrics = line.substr(pos);
		for (int time : times)
		{
			LRC::Line l = { time, lyrics };
#ifdef _DEBUG
			printf("lyrics: %d | %s\n", l.time, l.lyrics.c_str());
#endif
			m_lrc->append(l);
		}
	}
	else
	{
		size_t colonPos = tag.find(':');
		if (colonPos == std::string::npos) // invalid attribute
		{
			return false;
		}
		std::string attr = tag.substr(0, colonPos);
		std::string value = tag.substr(colonPos + 1);

#ifdef _DEBUG
		printf("attribute: %s | %s\n", attr.c_str(), value.c_str());
#endif
		m_lrc->addTag(attr, value);
	}

	return true;
}

int LrcReader::parseTime(const std::string &tag)
{
	// hh, mm, ss
	double times[3] = { 0.f, 0.f, 0.f };
	for (int i = int(tag.length()) - 1,  idx = 2; i >= 0 && idx >= 0; --idx, i -= 2)
	{
		while (i > 0 && tag[i - 1] != ':')
			--i;
		sscanf(&tag[i], "%lf", &times[idx]);
	}

	return int(((times[0] * 60 + times[1]) * 60 + times[2]) * 1000 + 0.5);
}

LRCX_END_NS
