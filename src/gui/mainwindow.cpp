#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playerwidget.h"
#include "application.h"

#include <QToolBar>
#include <QDesktopWidget>
#include <QDebug>

LRCX_BEGIN_NS

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindow)
{
	setIconSize(QSize(24, 24));
	setToolButtonStyle(Qt::ToolButtonTextUnderIcon);

	m_ui->setupUi(this);

	m_playerWidget = new PlayerWidget(this);
	addDockWidget(Qt::BottomDockWidgetArea, m_playerWidget);

	createToolBars();

	connect(m_ui->action_Quit, SIGNAL(triggered()), this, SLOT(onAction_Quit()));
	connect(m_ui->action_Save, SIGNAL(triggered()), this, SLOT(onAction_Save()));

	connect(m_ui->action_Undo, SIGNAL(triggered()), m_ui->lrcEditor, SLOT(undo()));
	connect(m_ui->action_Redo, SIGNAL(triggered()), m_ui->lrcEditor, SLOT(redo()));

	loadSettings();
}

MainWindow::~MainWindow()
{
	saveSettings();
}

void MainWindow::createToolBars()
{
	QToolBar *fileToolbar = addToolBar(QLatin1String("File Toolbar"));
	fileToolbar->addAction(m_ui->action_Open);
	fileToolbar->addAction(m_ui->action_Save);

	QToolBar *editToolbar = addToolBar(QLatin1String("Edit Toolbar"));
	editToolbar->addAction(m_ui->action_Undo);
	editToolbar->addAction(m_ui->action_Redo);
	editToolbar->addSeparator();
	editToolbar->addAction(m_ui->action_Insert_mark);
	editToolbar->addAction(m_ui->action_Delete_mark);
	editToolbar->addSeparator();
	editToolbar->addAction(m_ui->action_Add_music_info);
}

void MainWindow::loadSettings()
{
	QVariant value = xApp->settings(Application::AS_WindowPostion);
	if (value.isValid())
	{
		QRect rc = value.toRect();
		resize(rc.size());
		move(rc.x(), rc.y());
	}
	else
	{
		moveToCenter();
	}

	value = xApp->settings(Application::AS_WindowMaximized);
	if (value.isValid() && value.toBool())
		showMaximized();
}

void MainWindow::saveSettings()
{
	if (isMaximized())
	{
		xApp->setSetting(Application::AS_WindowMaximized, true);
	}
	else
	{
		xApp->setSetting(Application::AS_WindowMaximized, false);
		xApp->setSetting(Application::AS_WindowPostion, geometry());
	}
}

void MainWindow::moveToCenter()
{
	const QRect &rc = QApplication::desktop()->screenGeometry();
	int x = (rc.width() - width()) / 2;
	int y = (rc.height() - height()) / 2;

	move(x, y);
}

void MainWindow::onAction_Quit()
{
	qApp->quit();
}

void MainWindow::onAction_Save()
{
}

LRCX_END_NS
