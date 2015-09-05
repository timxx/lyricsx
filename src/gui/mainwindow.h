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

private:
	void createToolBars();

	void loadSettings();
	void saveSettings();

	void moveToCenter();

private Q_SLOTS:
	void onAction_Quit();
	void onAction_Save();

private:
	std::shared_ptr<Ui::MainWindow> m_ui;
	PlayerWidget *m_playerWidget;
};

LRCX_END_NS

#endif // __MAIN_WINDOW_H__
