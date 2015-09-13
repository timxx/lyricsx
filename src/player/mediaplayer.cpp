#include "mediaplayer.h"

#include <QMediaMetaData>
#include <QEventLoop>

LRCX_BEGIN_NS

MediaPlayer::MediaPlayer(QObject *parent)
	: Player(parent)
	, m_player(new QMediaPlayer)
{
	connect(m_player.get(), SIGNAL(durationChanged(qint64)), this, SIGNAL(durationChanged(qint64)));
	connect(m_player.get(), SIGNAL(positionChanged(qint64)), this, SIGNAL(positionChanged(qint64)));
	connect(m_player.get(), SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(onStateChanged(QMediaPlayer::State)));
	connect(m_player.get(), SIGNAL(seekableChanged(bool)), this, SIGNAL(seekableChanged(bool)));
}

MediaPlayer::~MediaPlayer()
{

}

void MediaPlayer::open(const QString &uri)
{
	m_player->setMedia(QUrl::fromLocalFile(uri));

	// FIXME: ugly code
	QEventLoop loop;
	connect(m_player.get(), SIGNAL(metaDataAvailableChanged(bool)), &loop, SLOT(quit()));
	loop.exec();
}

qint64 MediaPlayer::duration() const
{
	return m_player->duration();
}

qint64 MediaPlayer::position() const
{
	return m_player->position();
}

void MediaPlayer::setPosition(qint64 pos)
{
	m_player->setPosition(pos);
}

bool MediaPlayer::isSeekable() const
{
	return m_player->isSeekable();
}

Player::State MediaPlayer::state() const
{
	return (Player::State)m_player->state();
}

void MediaPlayer::play()
{
	m_player->play();
}

void MediaPlayer::pause()
{
	m_player->pause();
}

void MediaPlayer::stop()
{
	m_player->stop();
}

QVariant MediaPlayer::metaData(MetaData key)
{
	static std::map<MetaData, QString> s_keys =
	{
		{ Title,            QMediaMetaData::Title },
		{ Artist,           QMediaMetaData::ContributingArtist },
		{ AlbumTitle,       QMediaMetaData::AlbumTitle },
	};

	return m_player->metaData(s_keys[key]).toString();
}

void MediaPlayer::onStateChanged(QMediaPlayer::State state)
{
	emit stateChanged(Player::State(state));
}

LRCX_END_NS
