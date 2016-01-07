/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __LRC_HIGHLIGHTER_H__
#define __LRC_HIGHLIGHTER_H__

#include "config.h"

#include <QSyntaxHighlighter>

class QTextDocument;

LRCX_BEGIN_NS

class LrcHighlighter : public QSyntaxHighlighter
{
	Q_OBJECT
public:
	LrcHighlighter(QTextDocument *parent = 0);

protected:
	// QSyntaxHighlighter
	void highlightBlock(const QString &text) override;

private:
	struct Rule
	{
		QRegExp pattern;
		QTextCharFormat format;
	};

	QVector<Rule> m_rules;
};

LRCX_END_NS

#endif // __LRC_HIGHLIGHTER_H__
