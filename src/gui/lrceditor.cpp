#include "lrceditor.h"
#include "lrchighlighter.h"

#include <lrc/lrcreader.h>

#include <QKeyEvent>
#include <QDebug>

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
{
	new LrcHighlighter(document());
}

LrcEditor::~LrcEditor()
{

}

bool LrcEditor::openFile(const QString &file)
{
	QFile qfile(file);
	if (!qfile.open(QFile::ReadOnly | QFile::Text))
		return false;

	setPlainText(qfile.readAll());
	qfile.close();

	m_file = file;
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

	// for  undo/redo :(
	selectAll();
	insertPlainText(strText);
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
		cursor.insertText(newValue + '\n');
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

LRCX_END_NS
