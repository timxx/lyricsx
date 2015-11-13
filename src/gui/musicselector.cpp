#include "musicselector.h"
#include "ui_musicselector.h"
#include "application.h"

#include <player/player.h>
#include <player/playerfactory.h>

#include <QFileDialog>

LRCX_BEGIN_NS

MusicSelector::MusicSelector(QWidget *parent)
	: QDialog(parent)
	, m_ui(new Ui::MusicSelector)
{
	m_ui->setupUi(this);

#ifndef LRCX_HAVE_DBUS
	m_ui->rb_Mpris->setVisible(false);
	m_ui->cb_Mpris->setVisible(false);
#else
	initMpris();
#endif

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
#ifdef LRCX_HAVE_DBUS
		else
		{
			int idx = selector.m_ui->cb_Mpris->currentIndex();
			if (idx != -1)
			{
				player = PlayerFactory::create("mpris");
				player->open(selector.m_ui->cb_Mpris->itemData(idx).toString());
			}
		}
#endif
	}

	return player;
}

#ifdef LRCX_HAVE_DBUS
void MusicSelector::initMpris()
{
	m_mprisPlayer = new MprisPlayerHelper(this);

	const auto &players = m_mprisPlayer->getPlayers();
	for (const auto &info : players)
	{
		onMprisPlayerAdded(info);
	}

	connect(m_mprisPlayer, SIGNAL(playerAdded(MprisPlayerInfo)), this, SLOT(onMprisPlayerAdded(MprisPlayerInfo)));
	connect(m_mprisPlayer, SIGNAL(playerClosed(MprisPlayerInfo)), this, SLOT(onMprisPlayerClosed(MprisPlayerInfo)));
}
#endif

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
	QString dir = xApp->settings(Application::AS_MusicLastDir).toString();
	QString strFile = QFileDialog::getOpenFileName(this,
												   tr("Open File"),
												   dir,
												   filters);

	if (!strFile.isEmpty())
	{
		m_ui->le_File->setText(strFile);
		xApp->setSetting(Application::AS_MusicLastDir, strFile);
	}
}

#ifdef LRCX_HAVE_DBUS

void MusicSelector::onMprisPlayerAdded(const MprisPlayerInfo &player)
{
	onMprisPlayerClosed(player);

	QIcon icon;
	QFileInfo fi(player.icon);
	if (fi.isAbsolute())
		icon = QIcon(player.icon);
	else
		icon = QIcon::fromTheme(player.icon);

	m_ui->cb_Mpris->addItem(icon, player.name, QVariant(player.service));
}

void MusicSelector::onMprisPlayerClosed(const MprisPlayerInfo &player)
{
	int idx = m_ui->cb_Mpris->findData(QVariant(player.service));
	if (idx != -1)
		m_ui->cb_Mpris->removeItem(idx);
}

#endif // LRCX_HAVE_DBUS

LRCX_END_NS
