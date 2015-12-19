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

	// LrcEditor

private:
	std::shared_ptr<Ui::MainWindow> m_ui;
	PlayerWidget *m_playerWidget;
};

LRCX_END_NS

#endif // __MAIN_WINDOW_H__
