/**
 * Copyright (c) 2016 - 2018 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "lrceditor.h"
#include "lrchighlighter.h"

#include <lrc/lrcreader.h>

#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QFile>
#include <functional>

static QString _makeTimeTag(qint64 ms)
{
	int mm = ms / 1000 / 60;
	int ss = ms / 1000 % 60;
	int xx = (ms % 1000) / 10;

	QString str;
	str.sprintf("[%02d:%02d.%02d]", mm, ss, xx);
	return str;
}

LRCX_BEGIN_NS

LrcEditor::LrcEditor(QWidget *parent)
	: QTextEdit(parent)
	, m_expTag("^\\[[A-Za-z]+:.*\\][\\s]{0,}$")
	, m_charNewLine('\n')
{
	new LrcHighlighter(document());
}

LrcEditor::~LrcEditor()
{

}

bool LrcEditor::openFile(const QString &file, const QString &encoding)
{
	QFile qfile(file);
	if (!qfile.open(QFile::ReadOnly | QFile::Text))
		return false;

	QTextStream is(&qfile);
	if (!encoding.isEmpty())
	{
		is.setAutoDetectUnicode(false);
		is.setCodec(encoding.toLocal8Bit());
	}

	setPlainText(is.readAll());

	m_encoding = is.codec()->name();
	m_file = file;

	qfile.close();

	return true;
}

bool LrcEditor::saveFile()
{
	Q_ASSERT(!m_file.isEmpty());
	return saveFileAs(m_file);
}

bool LrcEditor::saveFileAs(const QString &file)
{
	QFile qFile(file);
	if (!qFile.open(QFile::WriteOnly | QFile::Text))
		return false;

	QTextStream os(&qFile);
	os.setCodec("UTF-8");
	os.setGenerateByteOrderMark(true);

	os << toPlainText();

	qFile.close();

	return true;
}

QString LrcEditor::getFile() const
{
	return m_file;
}

void LrcEditor::setTitle(const QString &title)
{
	addTag("ti", title);
}

void LrcEditor::setArtist(const QString &artist)
{
	addTag("ar", artist);
}

void LrcEditor::setAlbum(const QString &album)
{
	addTag("al", album);
}

void LrcEditor::setCreator(const QString &creator)
{
	addTag("by", creator);
}

void LrcEditor::addMark(qint64 ms)
{
	// move cursor to begin
	QTextCursor cursor = textCursor();
	cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

	if (m_expTag.indexIn(cursor.block().text()) >= 0)
		gotoNextMark(cursor);

	// check whether already contains the same time
	QByteArray strLine = cursor.block().text().trimmed().toLocal8Bit();
	if (!strLine.isEmpty())
	{
		LrcReader reader(strLine, strLine.length());
		reader.parse();
		if (reader.getLrc().getLineByTime(ms) != nullptr)
			goto _end_of_insert;
	}

	{
		QString strTag = _makeTimeTag(ms);
		cursor.insertText(strTag);
	}

_end_of_insert:
	gotoNextMark(cursor);
}

void LrcEditor::removeMark()
{
	QTextCursor cursor = textCursor();
	QString strText = cursor.block().text();

	if (strText.isEmpty())
		return ;

	const QRegExp exp("^\\[\\d+:\\d+\\.\\d+\\]");

	int index = exp.indexIn(strText);
	// no time tags
	if (index == -1)
		return ;

	strText.remove(index, exp.matchedLength());

	cursor.beginEditBlock();

	cursor.select(QTextCursor::LineUnderCursor);
	cursor.removeSelectedText();
	cursor.insertText(strText);

	cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

	cursor.endEditBlock();

	setTextCursor(cursor);
}

void LrcEditor::removeAllMarks()
{
	QString strText = toPlainText();

	strText.remove(QRegExp( "\\[\\d+:\\d+\\.\\d+\\]" ));

	replaceWholeText(strText);
}

void LrcEditor::mergeLyrics()
{
	QByteArray strText = toPlainText().toLocal8Bit();
	if (strText.isEmpty())
		return ;

	LrcReader parser(strText, strText.length());
	parser.parse();

	QStringList newText;

	const LRC &lrc = parser.getLrc();
	generateTags(newText, lrc);

	struct Line
	{
		std::string lyrics;
		std::vector<int> times;

		Line(const LRC::Line &line)
		{
			lyrics = line.lyrics;
			times.push_back(line.time);
		}
	};

	std::vector<Line> sortedLines;

	const auto &lines = lrc.getLines();
	// merge the same lyrics
	for (const auto &line : lines)
	{
		auto it = sortedLines.begin();
		for ( ; it != sortedLines.end(); ++it)
		{
			if (it->lyrics == line.lyrics)
				break;
		}

		if (it != sortedLines.end())
			it->times.push_back(line.time);
		else
			sortedLines.push_back(Line(line));
	}

	// sort by time
	for (auto &line : sortedLines)
		std::sort(line.times.begin(), line.times.end(), std::greater<int>());

	std::sort(sortedLines.begin(), sortedLines.end(), [](const Line &lhs, const Line &rhs)
	{
		Q_ASSERT(lhs.times.size() > 0 && rhs.times.size() > 0);
		return lhs.times.back() < rhs.times.back();
	});

	for (const auto &line : sortedLines)
	{
		QString strLine;
		for (auto time : line.times)
		{
			strLine += _makeTimeTag(time);
		}
		strLine += QString::fromStdString(line.lyrics);

		newText << strLine;
	}

	replaceWholeText(newText.join(m_charNewLine));
}

void LrcEditor::expandMarks()
{
	QByteArray strText = toPlainText().toLocal8Bit();
	if (strText.isEmpty())
		return ;

	LrcReader parser(strText, strText.length());
	parser.parse();

	QStringList newText;

	const LRC &lrc = parser.getLrc();
	generateTags(newText, lrc);

	const auto &lines = lrc.getLines();
	for (const auto &line : lines)
	{
		QString strLine = _makeTimeTag(line.time);
		strLine += QString::fromStdString(line.lyrics);

		newText << strLine;
	}

	replaceWholeText(newText.join(m_charNewLine));
}

void LrcEditor::addTag(const QString &attr, const QString &value)
{
	// ignore empty value
	if (value.isEmpty())
		return ;

	QRegExp exp("^\\[" + attr + ":.*\\][\\s]{0,}$");
	QString  newValue = "[" + attr + ":" + value + "]";
	bool found = false;

	for (auto it = document()->begin(); it != document()->end(); it = it.next())
	{
		if (exp.indexIn(it.text()) >= 0)
		{
			found = true;
			qDebug() << "found tag at line: " << it.blockNumber() << qPrintable(it.text());

			if (it.text().trimmed() != newValue)
			{
				QTextCursor cursor(it);
				cursor.beginEditBlock();

				cursor.select(QTextCursor::LineUnderCursor);
				cursor.removeSelectedText();
				cursor.insertText(newValue);

				cursor.endEditBlock();
			}

			// we assume there's only one
			break;
		}
	}

	if (!found)
	{
		QTextCursor cursor = textCursor();
		cursor.movePosition(QTextCursor::Start, QTextCursor::MoveAnchor);
		cursor.insertText(newValue + m_charNewLine);
	}
}

void LrcEditor::gotoNextMark(QTextCursor &cursor)
{
	cursor.movePosition(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);

	while (true)
	{
		bool ok = cursor.movePosition( QTextCursor::Down, QTextCursor::MoveAnchor);
		if (!ok)
			break;

		// skip attribute tag
		if (m_expTag.indexIn(cursor.block().text()) >= 0)
			continue ;

		break;
	}

	setTextCursor(cursor);
	ensureCursorVisible();
}

void LrcEditor::replaceWholeText(const QString &text)
{
	// for  undo/redo :(
	selectAll();
	insertPlainText(text);
}

void LrcEditor::generateTags(QStringList &list, const LRC &lrc)
{
	const auto &attrs = lrc.attributes();

	for (const auto &attr : attrs)
	{
		QString strTag = "[" + QString::fromStdString(attr.first) + ":" +
				QString::fromStdString(attr.second) + "]";
		list << strTag;
	}
}

LRCX_END_NS
