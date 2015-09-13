#ifndef __MPRIS_PLAYER_H__
#define __MPRIS_PLAYER_H__

#include "player.h"

LRCX_BEGIN_NS

class MprisPlayer : public Player
{
	Q_OBJECT
public:
	MprisPlayer(QObject *parent = 0);
	~MprisPlayer();

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
};

LRCX_END_NS

#endif // __MPRIS_PLAYER_H__
