#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "playerwidget.h"
#include "application.h"
#include "preferences.h"
#include "encodingchooser.h"

#include <QToolBar>
#include <QDesktopWidget>
#include <QDebug>
#include <QFileDialog>

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

	connect(m_ui->action_Open, SIGNAL(triggered()), this, SLOT(onAction_Open()));
	connect(m_ui->action_Reload_With_Encoding, SIGNAL(triggered()), this, SLOT(onAction_ReloadWithEncoding()));
	connect(m_ui->action_Save, SIGNAL(triggered()), this, SLOT(onAction_Save()));
	connect(m_ui->action_Save_as, SIGNAL(triggered()), this, SLOT(onAction_Saveas()));
	connect(m_ui->action_Quit, SIGNAL(triggered()), this, SLOT(onAction_Quit()));

	connect(m_ui->action_Undo, SIGNAL(triggered()), m_ui->lrcEditor, SLOT(undo()));
	connect(m_ui->action_Redo, SIGNAL(triggered()), m_ui->lrcEditor, SLOT(redo()));
	connect(m_ui->action_Insert_mark, SIGNAL(triggered()), this, SLOT(onAction_InsertMark()));
	connect(m_ui->action_Delete_mark, SIGNAL(triggered()), this, SLOT(onAction_DeleteMark()));
	connect(m_ui->action_Remove_all_marks, SIGNAL(triggered()), this, SLOT(onAction_RemoveAllMarks()));
	connect(m_ui->action_Add_music_info, SIGNAL(triggered()), this, SLOT(onAction_AddMusicInfo()));
	connect(m_ui->action_Merge_Lyrics, SIGNAL(triggered()), this, SLOT(onAction_MergeLyrics()));
	connect(m_ui->action_Expand_Marks, SIGNAL(triggered()), this, SLOT(onAction_ExpandMarks()));

	connect(m_ui->action_Preferences, SIGNAL(triggered()), this, SLOT(onAction_Preferences()));

	connect(m_ui->lrcEditor, SIGNAL(undoAvailable(bool)), m_ui->action_Undo, SLOT(setEnabled(bool)));
	connect(m_ui->lrcEditor, SIGNAL(redoAvailable(bool)), m_ui->action_Redo, SLOT(setEnabled(bool)));

	m_ui->action_Undo->setEnabled(false);
	m_ui->action_Redo->setEnabled(false);

	m_ui->action_Reload_With_Encoding->setEnabled(false);

	loadSettings();
}

MainWindow::~MainWindow()
{
	saveSettings();
}

bool MainWindow::openFile(const QString &filePath)
{
	bool ok = m_ui->lrcEditor->openFile(filePath);
	m_ui->action_Reload_With_Encoding->setEnabled(ok);
	return ok;
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

	resetEditorFont();
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

void MainWindow::resetEditorFont()
{
	QFont font = m_ui->lrcEditor->font();

	QVariant value = xApp->settings(Application::AS_EditorFontFamily);
	if (value.isValid())
		font.setFamily(value.toString());

	value = xApp->settings(Application::AS_EditorFontStyle);
	if (value.isValid())
		font.setStyleName(value.toString());

	value = xApp->settings(Application::AS_EditorFontSize);
	if (value.isValid())
		font.setPointSize(value.toInt());

	m_ui->lrcEditor->setFont(font);
}

void MainWindow::onAction_Open()
{
	const QString strFilter = tr("LRC files (*.lrc);;All files (*)");
	QString dir = xApp->settings(Application::AS_LrcLastDir).toString();

	QString lrcFile = QFileDialog::getOpenFileName(this,
												   tr("Open File"),
												   dir,
												   strFilter
												   );
	if (!lrcFile.isEmpty())
	{
		openFile(lrcFile);

		xApp->setSetting(Application::AS_LrcLastDir, lrcFile);
	}
}

void MainWindow::onAction_ReloadWithEncoding()
{
	EncodingChooser chooser(this);

	QString strFile = m_ui->lrcEditor->getFile();
	QFile qfile(strFile);
	if (qfile.open(QFile::ReadOnly))
		chooser.setPreviewData(qfile.readAll());
	chooser.setCurrentEncoding(m_ui->lrcEditor->currentEncoding());

	if (chooser.exec() == QDialog::Accepted)
	{
		m_ui->lrcEditor->openFile(strFile, chooser.getCurrentEncoding());
	}
}

void MainWindow::onAction_Save()
{
	if (m_ui->lrcEditor->getFile().isEmpty())
		onAction_Saveas();
	else
		m_ui->lrcEditor->saveFile();
}

void MainWindow::onAction_Saveas()
{
	const QString strFilter = tr("LRC files (*.lrc)");
	QFileInfo fileInfo(xApp->settings(Application::AS_LrcLastDir).toString());

	QString strFile = QFileDialog::getSaveFileName(this,
												   tr("Save File"),
												   fileInfo.path(),
												   strFilter);

	if (!strFile.isEmpty())
	{
		if (!strFile.endsWith(".lrc", Qt::CaseInsensitive))
			strFile.append(".lrc");

		m_ui->lrcEditor->saveFileAs(strFile);
		xApp->setSetting(Application::AS_LrcLastDir, strFile);
	}
}

void MainWindow::onAction_Quit()
{
	qApp->quit();
}

void MainWindow::onAction_InsertMark()
{
	m_ui->lrcEditor->addMark(m_playerWidget->getCurrentPosition());
}

void MainWindow::onAction_DeleteMark()
{
	m_ui->lrcEditor->removeMark();
}

void MainWindow::onAction_RemoveAllMarks()
{
	m_ui->lrcEditor->removeAllMarks();
}

void MainWindow::onAction_AddMusicInfo()
{
	LrcEditor *editor = m_ui->lrcEditor;

	editor->setTitle(m_playerWidget->getTitle());
	editor->setArtist(m_playerWidget->getArtist());
	editor->setAlbum(m_playerWidget->getAlbum());
	editor->setCreator(m_playerWidget->getEditor());
}

void MainWindow::onAction_MergeLyrics()
{
	m_ui->lrcEditor->mergeLyrics();
}

void MainWindow::onAction_ExpandMarks()
{
	m_ui->lrcEditor->expandMarks();
}

void MainWindow::onAction_Preferences()
{
	Preferences prefDlg(this);
	if (prefDlg.exec() == QDialog::Accepted)
	{
		resetEditorFont();
	}
}

LRCX_END_NS
