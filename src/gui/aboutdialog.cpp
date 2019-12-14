/**
 * Copyright (c) 2019 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include "i18n.h"
#include "stylehelper.h"

LRCX_BEGIN_NS

AboutDialog::AboutDialog(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::AboutDialog)
{
	m_ui->setupUi(this);

	QIcon icon = QIcon::fromTheme("lyricsx", QIcon(":/icons/128x128/lyricsx"));
	m_ui->appIcon->setPixmap(icon.pixmap(StyleHelper::dpiScaled(QSize(128, 128))));

	m_ui->tb_LyricsX->setOpenExternalLinks(true);
	m_ui->tb_License->setOpenExternalLinks(true);

	initLyricsX();
	initLicense();

	resize(StyleHelper::dpiScaled(QSize(465, 470)));
}

AboutDialog::~AboutDialog()
{
}

void AboutDialog::initLyricsX()
{
	QString strDesc =
			"<center><h3>" + i18n::appName() + " " + VERSION + "</h3></center>"
			"<center>" + lrcxns::AboutDialog::tr("free, open source, cross-platform software for editing lyrics") + "</center>"
			"<center><a href=https://github.com/timxx/lyricsx>" + lrcxns::AboutDialog::tr("Visit project host") + "</a></center>"
			"<br/>"
			"<center>Copyright © 2019 Weitian Leung</center>"
			;

	m_ui->tb_LyricsX->setHtml(strDesc);
}

void AboutDialog::initLicense()
{
	QString strLicense =
			"<p>The MIT License (MIT)</p>"
			"<p>"
			"Permission is hereby granted, free of charge, to any person obtaining a copy "
			"of this software and associated documentation files (the \"Software\"), to deal "
			"in the Software without restriction, including without limitation the rights "
			"to use, copy, modify, merge, publish, distribute, sublicense, and/or sell "
			"copies of the Software, and to permit persons to whom the Software is "
			"furnished to do so, subject to the following conditions: "
			"</p><p>"
			"The above copyright notice and this permission notice shall be included in all "
			"copies or substantial portions of the Software."
			"</p><p>"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR "
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, "
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE "
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER "
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, "
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE "
			"SOFTWARE."
			"</p>"
			;

	m_ui->tb_License->setHtml(strLicense);
}

LRCX_END_NS
