/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "mprisplayer.h"

#include <QtDBus>

class MediaPlayer2Player : public QObject
{
	Q_OBJECT
public:
	MediaPlayer2Player(const QString &service, QObject *parent = 0);
	~MediaPlayer2Player()
	{
	}

	inline QString title() const
	{
		return m_title;
	}

	inline QString artist() const
	{
		return m_artist;
	}

	inline QString album() const
	{
		return m_album;
	}

	inline qlonglong duratoin() const
	{
		return m_duration;
	}

	inline qlonglong position() const
	{
		return m_position;
	}

	bool canSeek() const
	{
		return m_iface->property("CanSeek").toBool();
	}

	inline QString playbackStatus() const
	{
		return m_status;
	}

	void seek(qint64 pos)
	{
		m_iface->call("Seek", QVariant(pos));
	}

	void play()
	{
		m_iface->call("Play");
	}

	void pause()
	{
		m_iface->call("Pause");
	}

	void stop()
	{
		m_iface->call("Stop");
	}

private:
	void checkMetadata(const QVariantMap &metadata);
	template<class T>
	bool checkData(const QVariantMap &data, const char *key, T &value);

Q_SIGNALS:
	void positionChanged(qlonglong position);
	void durationChanged(qlonglong duration);
	void statusChanged(const QString &state);
	void metaDataChanged(const QString &key, const QVariant &value);

private Q_SLOTS:
	void getProps();
	void onPropertiesChanged(const QString &iface, const QVariantMap &props, const QStringList &invalidProps);
	void updatePosition();

private:
	QDBusInterface *m_iface;
	QDBusInterface *m_ifaceProps;

	QString m_title;
	QString m_artist;
	QString m_album;

	QString m_status;
	qlonglong m_position;
	qlonglong m_duration;

	QTimer m_timerUpadte;
};

MediaPlayer2Player::MediaPlayer2Player(const QString &service, QObject *parent)
	: QObject(parent)
	, m_status("Stopped")
	, m_position(0)
	, m_duration(0)
{
	m_iface = new QDBusInterface(service,
								 "/org/mpris/MediaPlayer2",
								 "org.mpris.MediaPlayer2.Player",
								 QDBusConnection::sessionBus(),
								 this);

	m_ifaceProps = new QDBusInterface(service,
									  "/org/mpris/MediaPlayer2",
									  "org.freedesktop.DBus.Properties",
									  QDBusConnection::sessionBus(),
									  this);

	QDBusConnection::sessionBus().connect(
				m_ifaceProps->service(),
				m_ifaceProps->path(),
				m_ifaceProps->interface(),
				"PropertiesChanged",
				this,
				SLOT(onPropertiesChanged(QString,QVariantMap,QStringList))
			);

	// may not receive the Seeked signal
	m_timerUpadte.setInterval(250);
	connect(&m_timerUpadte, SIGNAL(timeout()),
			this, SLOT(updatePosition()));

	getProps();
}

void MediaPlayer2Player::checkMetadata(const QVariantMap &metadata)
{
	if (!checkData(metadata, "mpris:length", m_duration))
		emit durationChanged(m_duration);

	if (!checkData(metadata, "xesam:title", m_title))
		emit metaDataChanged("xesam:title", m_title);

	if (!checkData(metadata, "xesam:artist", m_artist))
		emit metaDataChanged("xesam:artist", m_artist);

	if (!checkData(metadata, "xesam:album", m_album))
		emit metaDataChanged("xesam:album", m_album);
}

template<class T>
bool MediaPlayer2Player::checkData(const QVariantMap &data, const char *key, T &value)
{
	if (data.contains(key))
	{
		T newValue = data[key].value<T>();
		if (value != newValue)
		{
			value = newValue;
			return false;
		}
	}

	return true;
}

void MediaPlayer2Player::getProps()
{
	QDBusReply<QVariantMap> reply = m_ifaceProps->call("GetAll", m_iface->interface());
	if (reply.isValid())
	{
		onPropertiesChanged(m_iface->interface(), reply.value(), QStringList());
	}
}

void MediaPlayer2Player::onPropertiesChanged(const QString &iface, const QVariantMap &props, const QStringList &invalidProps)
{
	if (props.contains("Metadata"))
	{
		QVariantMap metadata;
		const QVariant &var = props["Metadata"];
		if (var.type() == QVariant::Map)
		{
			metadata = var.toMap();
		}
		else if (var.canConvert<QDBusArgument>())
		{
			QDBusArgument arg = var.value<QDBusArgument>();
			arg >> metadata;
		}
		else
		{
			qWarning() << "Unknown variant type for Metadata:" << QDBusMetaType::typeToSignature(var.userType());
		}

		checkMetadata(metadata);
	}

	if (!checkData(props, "PlaybackStatus", m_status))
	{
		if (m_status == "Playing")
			m_timerUpadte.start();
		else
			m_timerUpadte.stop();

		emit statusChanged(m_status);
	}

	checkData(props, "Position", m_position);
}

void MediaPlayer2Player::updatePosition()
{
	QVariant var = m_iface->property("Position");
	if (!var.isValid())
		return ;

	qlonglong pos = var.toLongLong();
	if (pos != m_position)
	{
		m_position = pos;
		emit positionChanged(m_position);
	}
}

LRCX_BEGIN_NS

MprisPlayer::MprisPlayer(QObject *parent)
	: Player(parent)
	, m_player(NULL)
{
}

MprisPlayer::~MprisPlayer()
{
	delete m_player;
}

void MprisPlayer::open(const QString &service)
{
	if (m_player)
	{
		m_player->disconnect();
		m_player->deleteLater();
	}

	m_player = new MediaPlayer2Player(service);
	m_service = service;

	connect(m_player, SIGNAL(positionChanged(qlonglong)),
			this, SLOT(onPlayerPositionChanged(qlonglong)));

	connect(m_player, SIGNAL(durationChanged(qlonglong)),
			this, SLOT(onPlayerDurationChanged(qlonglong)));

	connect(m_player, SIGNAL(statusChanged(QString)),
			this, SLOT(onPlayerStatusChanged(QString)));

	connect(m_player, SIGNAL(metaDataChanged(QString,QVariant)),
			this, SLOT(onPlayerMetadataChanged(QString,QVariant)));
}

qint64 MprisPlayer::duration() const
{
	if (!m_player)
		return 0;

	return m_player->duratoin() / 1000;
}

qint64 MprisPlayer::position() const
{
	if (!m_player)
		return 0;

	return m_player->position() / 1000;
}

void MprisPlayer::setPosition(qint64 pos)
{
	if (!isSeekable())
		return ;

	m_player->seek(pos);
}

bool MprisPlayer::isSeekable() const
{
	if (!m_player)
		return false;
	return m_player->canSeek();
}

Player::State MprisPlayer::state() const
{
	if (!m_player)
		return Stopped;

	QString status = m_player->playbackStatus();

	return statusToState(status);
}

void MprisPlayer::play()
{
	if (m_player)
		m_player->play();
}

void MprisPlayer::pause()
{
	if (m_player)
		m_player->pause();
}

void MprisPlayer::stop()
{
	if (m_player)
		m_player->stop();
}

QVariant MprisPlayer::metaData(MetaData key)
{
	if (!m_player)
		return QVariant();

	switch (key)
	{
	case Title:
		return m_player->title();
		break;

	case Artist:
		return m_player->artist();
		break;

	case AlbumTitle:
		return m_player->album();
		break;
	}

	return QString();
}

Player::State MprisPlayer::statusToState(const QString &status) const
{
	if (status == "Playing")
		return Playing;
	else if (status == "Paused")
		return Paused;

	return Stopped;
}

void MprisPlayer::onPlayerPositionChanged(qlonglong position)
{
	emit positionChanged(position / 1000);
}

void MprisPlayer::onPlayerDurationChanged(qlonglong duration)
{
	emit durationChanged(duration / 1000);
}

void MprisPlayer::onPlayerStatusChanged(const QString &status)
{
	State state = statusToState(status);
	emit stateChanged(state);
}

void MprisPlayer::onPlayerMetadataChanged(const QString &key, const QVariant &value)
{
	static std::map<QString, Player::MetaData> s_keys =
	{
		{ "xesam:title",            Player::Title },
		{ "xesam:artist",           Player::Artist },
		{ "xesam:album",            Player::AlbumTitle },
	};

	if (s_keys.find(key) != s_keys.end())
		emit metaDataChanged(s_keys[key], value);
}

LRCX_END_NS

#include "mprisplayer.moc"
