/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __MPRIS_PLAYER_HELPER_H__
#define __MPRIS_PLAYER_HELPER_H__

#include "player_export.h"

#include <QObject>

class QDBusServiceWatcher;

LRCX_BEGIN_NS

struct PLAYER_EXPORT MprisPlayerInfo
{
	QString name;
	QString icon;
	QString service;
};

class PLAYER_EXPORT MprisPlayerHelper : public QObject
{
	Q_OBJECT
public:
	MprisPlayerHelper(QObject *parent = 0);
	~MprisPlayerHelper();

	const std::vector<MprisPlayerInfo> &getPlayers() const;

private:
	void appendPlayer(const QString &service);
	void removePlayer(const QString &service);

Q_SIGNALS:
	void playerAdded(const MprisPlayerInfo &player);
	void playerClosed(const MprisPlayerInfo &player);

private Q_SLOTS:
	void onServiceOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner);

private:
	QDBusServiceWatcher *m_dbusWatcher;
	std::vector<MprisPlayerInfo> m_players;
};

LRCX_END_NS

#endif // __MPRIS_PLAYER_HELPER_H__
