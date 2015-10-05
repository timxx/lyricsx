#include "playerwidget.h"
#include "ui_playerwidget.h"
#include "musicselector.h"
#include "application.h"

static QString _ms2mmss(qint64 ms)
{
	int ss = ms / 1000;
	int mm = ss / 60;
	ss = ss % 60;

	QString str;
	str.sprintf("%02d:%02d", mm, ss);
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
	connect(m_ui->slider_Duration, SIGNAL(sliderReleased()), this, SLOT(onSliderDuration_Changed()) );
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

void PlayerWidget::onBtnOpen_Clicked()
{
	std::unique_ptr<Player> player(MusicSelector::select());
	if (player == nullptr)
		return ;

	m_ui->le_Title->setText(player->metaData(Player::Title).toString());
	m_ui->le_Artist->setText(player->metaData(Player::Artist).toString());
	m_ui->le_Album->setText(player->metaData(Player::AlbumTitle).toString());
	if (m_ui->le_Editor->text().trimmed().isEmpty())
	{
		QVariant editor = xApp->settings(Application::AS_LrcEditor);
		if (editor.isValid())
			m_ui->le_Editor->setText(editor.toString());
		else
			m_ui->le_Editor->setText(tr("LyricsX"));
	}

	QString strTitle = player->metaData(Player::Artist).toString();
	if (strTitle.isEmpty())
		strTitle = player->metaData(Player::Title).toString();
	else
		strTitle += " - " + player->metaData(Player::Title).toString();
	setWindowTitle(strTitle);

	m_ui->btn_PlayPause->setEnabled(true);
	m_ui->slider_Duration->setEnabled(true);

	if (m_player)
		m_player->disconnect();

	m_player = std::move(player);

	connect(m_player.get(), SIGNAL(stateChanged(Player::State)), this, SLOT(onPlayerStateChanged(Player::State)));
	connect(m_player.get(), SIGNAL(durationChanged(qint64)), this, SLOT(onPlayerDurationChanged(qint64)));
	connect(m_player.get(), SIGNAL(positionChanged(qint64)), this, SLOT(onPlayerPositionChanged(qint64)));
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
	QIcon icon = QIcon::fromTheme(state == Player::Playing ?
									  "media-playback-pause" :
									  "media-playback-start"
				);
	m_ui->btn_PlayPause->setIcon(icon);
}

void PlayerWidget::onPlayerDurationChanged(qint64 duration)
{
	m_ui->slider_Duration->setMaximum(duration);
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

LRCX_END_NS
