/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "lrchighlighter.h"

LRCX_BEGIN_NS

LrcHighlighter::LrcHighlighter(QTextDocument *parent)
	: QSyntaxHighlighter(parent)
{
	QTextCharFormat tagFormat;
	tagFormat.setForeground(Qt::red);

	QRegExp tagExp("^\\[[A-Za-z]+:.*\\][\\s]{0,}$");
	Rule tagRule = { tagExp, tagFormat };
	m_rules.append(tagRule);

	QTextCharFormat timeFormat;
	timeFormat.setForeground(Qt::blue);

	QRegExp timeExp("\\[[0-5][0-9]:[0-5][0-9]\\.[0-9]{2,2}\\]");
	Rule timeRule = { timeExp, timeFormat };
	m_rules.append(timeRule);
}

void LrcHighlighter::highlightBlock(const QString &text)
{
	// ignore empty line
	if (text.trimmed().isEmpty())
		return ;

	for (auto rule : m_rules)
	{
		QRegExp exp(rule.pattern);
		int index = exp.indexIn(text);
		while (index >= 0)
		{
			int length = exp.matchedLength();
			setFormat(index, length, rule.format);
			index = exp.indexIn(text, index + length);
		}
	}
}

LRCX_END_NS
