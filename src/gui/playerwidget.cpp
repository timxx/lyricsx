/**
 * Copyright (c) 2019 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "playerwidget.h"
#include "ui_playerwidget.h"
#include "musicselector.h"
#include "application.h"
#include "i18n.h"

static QString _ms2mmss(qint64 ms)
{
	if (ms == 0)
		return "--:--";

	int ss = ms / 1000;
	int mm = ss / 60;
	ss = ss % 60;

	QString str = QString("%1:%2").arg(mm, 2, 10, QChar('0'))
		.arg(ss, 2, 10, QChar('0'));
	return str;
}

LRCX_BEGIN_NS

PlayerWidget::PlayerWidget(QWidget *parent)
	: QDockWidget(parent)
	, m_ui(new Ui::PlayerWidget)
{
	m_ui->setupUi(this);

	connect(m_ui->btn_Open, SIGNAL(clicked(bool)), this, SLOT(onBtnOpen_Clicked()));
	connect(m_ui->btn_PlayPause, SIGNAL(clicked(bool)), this, SLOT(onBtnPlayPause_Clicked()));
	connect(m_ui->slider_Duration, SIGNAL(sliderReleased()), this, SLOT(onSliderDuration_Changed()));

	QVariant var = xApp->settings(Application::AS_LrcEditor);
	m_ui->le_Editor->setText(var.isValid() ? var.toString() : i18n::appName());

	updatePlayPauseIcon();
}

PlayerWidget::~PlayerWidget()
{

}

QString PlayerWidget::getTitle() const
{
	return m_ui->le_Title->text().trimmed();
}

QString PlayerWidget::getArtist() const
{
	return m_ui->le_Artist->text().trimmed();
}

QString PlayerWidget::getAlbum() const
{
	return m_ui->le_Album->text().trimmed();
}

QString PlayerWidget::getEditor() const
{
	return m_ui->le_Editor->text().trimmed();
}

qint64 PlayerWidget::getCurrentPosition() const
{
	if (m_player)
		return m_player->position();
	return 0;
}

void PlayerWidget::updateTitle()
{
	QString strTitle = m_player->metaData(Player::Artist).toString();
	if (strTitle.isEmpty())
		strTitle = m_player->metaData(Player::Title).toString();
	else
		strTitle += " - " + m_player->metaData(Player::Title).toString();
	setWindowTitle(strTitle);
}

void PlayerWidget::updatePlayPauseIcon()
{
	Player::State state = Player::Paused;
	if (m_player)
		state = m_player->state();

	QIcon icon;
	if (state == Player::Playing)
		icon = QIcon::fromTheme("media-playback-pause", QIcon(":/icons/media-pause"));
	else
		icon = QIcon::fromTheme("media-playback-start", QIcon(":/icons/media-start"));

	m_ui->btn_PlayPause->setIcon(icon);
}

void PlayerWidget::onBtnOpen_Clicked()
{
	std::unique_ptr<Player> player(MusicSelector::select());
	if (player == nullptr)
		return ;

	if (m_player)
	{
		m_player->disconnect();
		m_player->deleteLater();
	}

	m_player = std::move(player);

	m_ui->le_Title->setText(m_player->metaData(Player::Title).toString());
	m_ui->le_Artist->setText(m_player->metaData(Player::Artist).toString());
	m_ui->le_Album->setText(m_player->metaData(Player::AlbumTitle).toString());

	updateTitle();

	m_ui->btn_PlayPause->setEnabled(true);
	m_ui->slider_Duration->setEnabled(m_player->isSeekable());
	m_ui->slider_Duration->setMaximum(m_player->duration());
	m_ui->slider_Duration->setValue(m_player->position());

	onPlayerPositionChanged(m_player->position());
	onPlayerStateChanged(m_player->state());

	connect(m_player.get(), SIGNAL(stateChanged(Player::State)), this, SLOT(onPlayerStateChanged(Player::State)));
	connect(m_player.get(), SIGNAL(durationChanged(qint64)), this, SLOT(onPlayerDurationChanged(qint64)));
	connect(m_player.get(), SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));
	connect(m_player.get(), SIGNAL(metaDataChanged(Player::MetaData,QVariant)), this, SLOT(onPlayerMetadataChanged(Player::MetaData,QVariant)));
	connect(m_player.get(), SIGNAL(seekableChanged(bool)), this, SLOT(onPlayerSeekableChanged(bool)));
}

void PlayerWidget::onBtnPlayPause_Clicked()
{
	if (m_player)
	{
		if (m_player->state() == Player::Playing)
			m_player->pause();
		else
			m_player->play();
	}
}

void PlayerWidget::onSliderDuration_Changed()
{
	if (m_player)
	{
		int pos = m_ui->slider_Duration->value();
		if (m_player->position() != pos)
		{
			m_ui->slider_Duration->blockSignals(true);
			m_player->setPosition(pos);
			m_ui->slider_Duration->blockSignals(false);
		}
	}
}

void PlayerWidget::onPlayerStateChanged(Player::State state)
{
	updatePlayPauseIcon();
}

void PlayerWidget::onPlayerDurationChanged(qint64 duration)
{
	m_ui->slider_Duration->setMaximum(duration);
	onPlayerPositionChanged(m_player->position());
}

void PlayerWidget::onPlayerPositionChanged(qint64 pos)
{
	qint64 total = m_player->duration();
	QString strText = _ms2mmss(pos) + "/" + _ms2mmss(total);
	m_ui->label_Duration->setText(strText);

	m_ui->slider_Duration->blockSignals(true);
	m_ui->slider_Duration->setValue(pos);
	m_ui->slider_Duration->blockSignals(false);
}

void PlayerWidget::onPlayerSeekableChanged(bool state)
{
	m_ui->slider_Duration->setEnabled(state);
}

void PlayerWidget::onPlayerMetadataChanged(Player::MetaData key, const QVariant &value)
{
	switch (key)
	{
	case Player::Title:
		m_ui->le_Title->setText(value.toString());
		updateTitle();
		break;

	case Player::Artist:
		m_ui->le_Artist->setText(value.toString());
		updateTitle();
		break;

	case Player::AlbumTitle:
		m_ui->le_Album->setText(value.toString());
		break;
	}
}

LRCX_END_NS
