#ifndef __MEDIA_PLAYER_H__
#define __MEDIA_PLAYER_H__

#include "player.h"

#include <QMediaPlayer>

LRCX_BEGIN_NS

class MediaPlayer : public Player
{
	Q_OBJECT
public:
	MediaPlayer(QObject *parent = 0);
	~MediaPlayer();

	void open(const QString &uri) override;

	qint64 duration() const override;

	qint64 position() const override;
	void setPosition(qint64 pos) override;

	bool isSeekable() const override;

	State state() const override;

	void play() override;
	void pause() override;
	void stop() override;

	QVariant metaData(MetaData key) override;

private Q_SLOTS:
	void onStateChanged(QMediaPlayer::State state);

private:
	std::shared_ptr<QMediaPlayer> m_player;
};

LRCX_END_NS

#endif // __MEDIA_PLAYER_H__
