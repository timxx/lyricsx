#include "lrc.h"

#include <algorithm>

LRCX_BEGIN_NS

LRC::LRC()
{
}

LRC::LRC(const LRC &other)
{
	assign(other);
}

LRC::~LRC()
{
}

void LRC::addTag(const std::string &tag, const std::string &value)
{
	m_tags[tag] = value;
}

void LRC::setOffset(int offset)
{
	m_tags["offset"] = std::to_string(offset);
}

int LRC::getOffset() const
{
	auto it = m_tags.find("offset");
	if (it == m_tags.end())
		return 0;

	return std::stoi(it->second);
}

void LRC::append(const Line &line)
{
	m_lyrics.push_back(line);
}

void LRC::clear()
{
	m_lyrics.clear();
	m_tags.clear();
}

int LRC::count() const
{
	return m_lyrics.size();
}

int LRC::attrCount() const
{
	return m_tags.size();
}

const LRC::Line* LRC::getLine(int index) const
{
	if (index < 0 || size_t(index) >= m_lyrics.size())
		return nullptr;
	return &m_lyrics[index];
}

// we assumed the lyrics already sorted
const LRC::Line* LRC::getLineByTime(int time) const
{
	Line l;
	l.time = time;
	auto it = std::lower_bound(m_lyrics.begin(), m_lyrics.end(), l, [](const Line &rhl, const Line &rhs)
	{
		return rhl.time < rhs.time;
	});

	if (it != m_lyrics.end() && !(time < it->time))
		return &(*it);
	return nullptr;
}

const std::vector<LRC::Line>& LRC::getLines() const
{
	return m_lyrics;
}

std::string LRC::attribute(const std::string &name) const
{
	auto it = m_tags.find(name);
	if (it != m_tags.end())
		return it->second;
	return std::string();
}

const std::map<std::string, std::string>& LRC::attributes() const
{
	return m_tags;
}

void LRC::sort()
{
	std::sort(m_lyrics.begin(), m_lyrics.end(), [](const Line &rhl, const Line &rhs)
	{
		return rhl.time < rhs.time;
	});

	// update index
	int index = 0;
	for (Line &line : m_lyrics)
	{
		line.index = index++;
	}
}

LRC& LRC::operator = (const LRC &other)
{
	assign(other);
	return *this;
}

void LRC::assign(const LRC &other)
{
	clear();

	this->m_lyrics = other.m_lyrics;
	this->m_tags = other.m_tags;
}

LRCX_END_NS
