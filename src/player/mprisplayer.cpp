#include "mprisplayer.h"

LRCX_BEGIN_NS

MprisPlayer::MprisPlayer(QObject *parent)
	: Player(parent)
{

}

MprisPlayer::~MprisPlayer()
{

}

void MprisPlayer::open(const QString &uri)
{

}

qint64 MprisPlayer::duration() const
{
	return 0;
}

qint64 MprisPlayer::position() const
{
	return 0;
}

void MprisPlayer::setPosition(qint64 pos)
{

}

bool MprisPlayer::isSeekable() const
{
	return false;
}

Player::State MprisPlayer::state() const
{
	return Stopped;
}

void MprisPlayer::play()
{

}

void MprisPlayer::pause()
{

}

void MprisPlayer::stop()
{

}

QVariant MprisPlayer::metaData(MetaData key)
{
	return QVariant();
}

LRCX_END_NS
