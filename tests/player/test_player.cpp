/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include <player/playerfactory.h>
#include <player/player.h>

#include <QtTest/QtTest>

class test_Player : public QObject
{
	Q_OBJECT
private Q_SLOTS:
	void testMetaData();
};

void test_Player::testMetaData()
{
	std::unique_ptr<Player> player = PlayerFactory::create("media");

	player->open(QFINDTESTDATA("blank.mp3"));

	QCOMPARE(player->metaData(Player::Title).toString(), QString("blank"));
	QVERIFY(player->metaData(Player::Artist).toStringList().contains(QString("lyricsx")));
	QCOMPARE(player->metaData(Player::AlbumTitle).toString(), QString("demo"));
}

QTEST_GUILESS_MAIN(test_Player)
#include "test_player.moc"
