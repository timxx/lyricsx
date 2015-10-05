#ifndef __LRC_EDITOR_H__
#define __LRC_EDITOR_H__

#include "config.h"

#include <QTextEdit>

LRCX_BEGIN_NS

class LRC;

class LrcEditor : public QTextEdit
{
	Q_OBJECT
public:
	LrcEditor(QWidget *parent = 0);
	~LrcEditor();

	bool openFile(const QString &file);
	bool saveFile();
	bool saveFileAs(const QString &file);

	QString getFile() const;

	void setTitle(const QString &title);
	void setArtist(const QString &artist);
	void setAlbum(const QString &album);
	void setCreator(const QString &creator);

	void addMark(qint64 ms);
	// remove the time mark of left most
	void removeMark();
	void removeAllMarks();

	void mergeLyrics();
	void expandMarks();

private:
	void addTag(const QString &attr, const QString &value);
	void gotoNextMark(QTextCursor &cursor);
	void replaceWholeText(const QString &text);

	void generateTags(QStringList &list, const LRC &lrc);

private:
	QString m_file;
	QRegExp m_expTag;
	QChar m_charNewLine;
};

LRCX_END_NS

#endif // __LRC_EDITOR_H__
