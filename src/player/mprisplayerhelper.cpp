#include "mprisplayerhelper.h"

#include <QtDBus>
#include <QFileInfo>
#include <QSettings>

const char *MPRIS2_IFACE = "org.mpris.MediaPlayer2";
const char *MPRIS2_PATH  = "/org/mpris/MediaPlayer2";

class MediaPlayer2
{
public:
	MediaPlayer2(const QString &service)
		: m_iface(new QDBusInterface(service, MPRIS2_PATH, MPRIS2_IFACE))
	{
	}

	~MediaPlayer2()
	{
		delete m_iface;
	}

	bool isValid() const
	{
		return m_iface && m_iface->isValid();
	}

	QString Identity() const
	{
		return m_iface->property("Identity").toString();
	}

	QString DesktopEntry() const
	{
		return m_iface->property("DesktopEntry").toString();
	}

private:
	QDBusInterface *m_iface;
};

static QString _absDesktopFile(const QString &desktop)
{
	static QStringList s_dirs;

	if (s_dirs.isEmpty())
	{
		QString env = qgetenv("XDG_DATA_DIRS");

		if (!env.isEmpty())
		{
			s_dirs = env.split(':', QString::SkipEmptyParts);
		}

		// do we need user home directory ?
		const char *predef_dirs[] =
		{
			"/usr/local/share",
			"/usr/share",
		};

		const int count = sizeof(predef_dirs) / sizeof(predef_dirs[0]);
		for (int i = 0; i < count; ++i)
		{
			if (!s_dirs.contains(predef_dirs[i]))
				s_dirs << predef_dirs[i];
		}
	}

	foreach (const QString &dir, s_dirs)
	{
		QFileInfo fi(dir + "/applications/" + desktop + ".desktop");
		if (fi.exists())
			return fi.absoluteFilePath();
	}

	return QString();
}

static QString _getIconFromDesktop(const QString &desktop)
{
	QString file = _absDesktopFile(desktop);
	if (file.isEmpty())
		return QString();

	QSettings sett(file, QSettings::IniFormat);
	sett.beginGroup("Desktop Entry");
	QString icon = sett.value("Icon").toString();

	return icon;
}

LRCX_BEGIN_NS

MprisPlayerHelper::MprisPlayerHelper(QObject *parent)
	: QObject(parent)
	, m_dbusWatcher(nullptr)
{
	QDBusConnection conn = QDBusConnection::sessionBus();
	if (conn.isConnected())
	{
		m_dbusWatcher = new QDBusServiceWatcher(QString(),
											conn,
											QDBusServiceWatcher::WatchForOwnerChange,
											this);
		connect(m_dbusWatcher, SIGNAL(serviceOwnerChanged(QString,QString,QString)),
				this,  SLOT(onServiceOwnerChanged(QString,QString,QString)));

		QDBusReply<QStringList> reply = conn.interface()->registeredServiceNames();
		if (reply.isValid())
		{
			QStringList services = reply.value();
			foreach (const QString& service, services)
				onServiceOwnerChanged(service, QString(), QString("foo"));
		}
	}
}

MprisPlayerHelper::~MprisPlayerHelper()
{

}

const std::vector<MprisPlayerInfo> &MprisPlayerHelper::getPlayers() const
{
	return m_players;
}

void MprisPlayerHelper::appendPlayer(const QString &service)
{
	MediaPlayer2 player(service);

	MprisPlayerInfo info;
	info.name = player.Identity();
	info.icon = _getIconFromDesktop(player.DesktopEntry());
	info.service = service;
	m_players.push_back(info);

	emit playerAdded(info);
}

void MprisPlayerHelper::removePlayer(const QString &service)
{
	auto it = m_players.begin();
	for (; it != m_players.end(); ++it)
	{
		if (it->service == service)
		{
			emit playerClosed(*it);
			break;
		}
	}

	if (it != m_players.end())
		m_players.erase(it);
}

void MprisPlayerHelper::onServiceOwnerChanged(const QString &service, const QString &oldOwner, const QString &newOwner)
{
	if (!service.startsWith(QLatin1String(MPRIS2_IFACE)))
		return ;

	if (oldOwner.isEmpty() && !newOwner.isEmpty())
	{
		appendPlayer(service);
	}
	else if (!oldOwner.isEmpty() && newOwner.isEmpty())
	{
		removePlayer(service);
	}
}

LRCX_END_NS
