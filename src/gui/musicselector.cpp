#include "musicselector.h"
#include "ui_musicselector.h"
#include <player/player.h>
#include <player/playerfactory.h>

#include <QFileDialog>

LRCX_BEGIN_NS

MusicSelector::MusicSelector(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::MusicSelector)
{
	m_ui->setupUi(this);

	initMpris();

	connect(m_ui->rb_Local, SIGNAL(clicked(bool)), this, SLOT(onRB_Local_Clicked(bool)));
	connect(m_ui->rb_Mpris, SIGNAL(clicked(bool)), this, SLOT(onRB_Mpris_Clicked(bool)));
	connect(m_ui->btn_Open, SIGNAL(clicked(bool)), this, SLOT(onBtn_Open_Clicked(bool)));

	m_ui->rb_Local->click();
}

MusicSelector::~MusicSelector()
{

}

std::unique_ptr<Player> MusicSelector::select()
{
	std::unique_ptr<Player> player;
	MusicSelector selector;
	if (selector.exec() == QDialog::Accepted)
	{
		if (selector.m_ui->rb_Local->isChecked())
		{
			QString strFile = selector.m_ui->le_File->text();
			if (!strFile.isEmpty())
			{
				player = PlayerFactory::create("media");
				player->open(strFile);
			}
		}
		else
		{
			// TODO
		}
	}

	return player;
}

void MusicSelector::initMpris()
{

}

void MusicSelector::onRB_Local_Clicked(bool checked)
{
	m_ui->le_File->setEnabled(true);
	m_ui->btn_Open->setEnabled(true);

	m_ui->cb_Mpris->setEnabled(false);
}

void MusicSelector::onRB_Mpris_Clicked(bool checked)
{
	m_ui->cb_Mpris->setEnabled(true);
	m_ui->le_File->setEnabled(false);
	m_ui->btn_Open->setEnabled(false);
}

void MusicSelector::onBtn_Open_Clicked(bool checked)
{
	const QString filters = tr("All files (*);;MP3 files (*.mp3)");
	QString strFile = QFileDialog::getOpenFileName(this,
												   tr("Open File"),
												   QString(),
												   filters);

	if (!strFile.isEmpty())
	{
		m_ui->le_File->setText(strFile);
	}
}

LRCX_END_NS
