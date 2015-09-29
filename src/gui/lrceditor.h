#ifndef __LRC_EDITOR_H__
#define __LRC_EDITOR_H__

#include "config.h"

#include <QTextEdit>

LRCX_BEGIN_NS

class LrcEditor : public QTextEdit
{
	Q_OBJECT
public:
	LrcEditor(QWidget *parent = 0);
	~LrcEditor();

	bool openFile(const QString &file);
	QString getFile() const;

	void setTitle(const QString &title);
	void setArtist(const QString &artist);
	void setAlbum(const QString &album);
	void setCreator(const QString &creator);

	void addMark(qint64 ms);
	// remove the time mark of left most
	void removeMark();
	void removeAllMarks();

private:
	void addTag(const QString &attr, const QString &value);
	void gotoNextMark(QTextCursor &cursor);

private:
	QString m_file;
	QRegExp m_expTag;
};

LRCX_END_NS

#endif // __LRC_EDITOR_H__
