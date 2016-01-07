/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __MPRIS_PLAYER_H__
#define __MPRIS_PLAYER_H__

#include "player.h"

class MediaPlayer2Player;

LRCX_BEGIN_NS

class MprisPlayer : public Player
{
	Q_OBJECT
public:
	MprisPlayer(QObject *parent = 0);
	~MprisPlayer();

	void open(const QString &service) override;

	qint64 duration() const override;

	qint64 position() const override;
	void setPosition(qint64 pos) override;

	bool isSeekable() const override;

	State state() const override;

	void play() override;
	void pause() override;
	void stop() override;

	QVariant metaData(MetaData key) override;

private:
	State statusToState(const QString &status) const;

private Q_SLOTS:
	void onPlayerPositionChanged(qlonglong position);
	void onPlayerDurationChanged(qlonglong duration);
	void onPlayerStatusChanged(const QString &status);
	void onPlayerMetadataChanged(const QString &key, const QVariant &value);

private:
	QString m_service;
	MediaPlayer2Player *m_player;
};

LRCX_END_NS

#endif // __MPRIS_PLAYER_H__
