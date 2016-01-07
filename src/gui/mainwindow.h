/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __MAIN_WINDOW_H__
#define __MAIN_WINDOW_H__

#include "config.h"

#include <QMainWindow>

namespace Ui {
	class MainWindow;
}

LRCX_BEGIN_NS

class PlayerWidget;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget *parent = 0);
	~MainWindow();

	bool openFile(const QString &filePath);

private:
	void createToolBars();

	void loadSettings();
	void saveSettings();

	void moveToCenter();

	void resetEditorFont();

private Q_SLOTS:
	// File menu
	void onAction_Open();
	void onAction_ReloadWithEncoding();
	void onAction_Save();
	void onAction_Saveas();
	void onAction_Quit();

	// Edit menu
	void onAction_InsertMark();
	void onAction_DeleteMark();
	void onAction_RemoveAllMarks();
	void onAction_AddMusicInfo();
	void onAction_MergeLyrics();
	void onAction_ExpandMarks();

	// Settings menu
	void onAction_Preferences();

	// Help menu
	void onAction_About();

	// LrcEditor

private:
	std::shared_ptr<Ui::MainWindow> m_ui;
	PlayerWidget *m_playerWidget;
};

LRCX_END_NS

#endif // __MAIN_WINDOW_H__
