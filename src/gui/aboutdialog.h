/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __ABOUT_DIALOG_H__
#define __ABOUT_DIALOG_H__

#include "config.h"

#include <QDialog>

namespace Ui {
class AboutDialog;
}

LRCX_BEGIN_NS

class AboutDialog : public QDialog
{
	Q_OBJECT
public:
	AboutDialog(QWidget *parent = 0);
	~AboutDialog();

private:
	void initLyricsX();
	void initLicense();

private:
	std::shared_ptr<Ui::AboutDialog> m_ui;
};

LRCX_END_NS

#endif // __ABOUT_DIALOG_H__
