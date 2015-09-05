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
};

LRCX_END_NS

#endif // __LRC_EDITOR_H__
