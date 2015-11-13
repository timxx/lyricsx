#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "player_export.h"

#include <QObject>
#include <QVariant>

LRCX_BEGIN_NS

class PLAYER_EXPORT Player : public QObject
{
	Q_OBJECT
	Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)
	Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
	Q_PROPERTY(bool seekable READ isSeekable NOTIFY seekableChanged)
	Q_PROPERTY(State state READ state NOTIFY stateChanged)
	Q_ENUMS(State)
	Q_ENUMS(MetaData)
public:
	enum State
	{
		Stopped,
		Playing,
		Paused,
	};

	enum MetaData
	{
		Title,
		Artist,
		AlbumTitle,
	};

public:
	Player(QObject *parent = 0);
	virtual ~Player();

	virtual void open(const QString &uri) = 0;

	virtual qint64 duration() const = 0;

	virtual qint64 position() const = 0;
	virtual void setPosition(qint64 pos) = 0;

	virtual bool isSeekable() const = 0;

	virtual State state() const = 0;

	virtual void play() = 0;
	virtual void pause() = 0;
	virtual void stop() = 0;

	virtual QVariant metaData(MetaData key) = 0;

Q_SIGNALS:
	void durationChanged(qint64 duration);
	void positionChanged(qint64 position);
	void stateChanged(Player::State newState);
	void seekableChanged(bool);
	void metaDataChanged(Player::MetaData key, const QVariant &value);
};

LRCX_END_NS

#endif // ___MUSIC_PLAYER_H__
