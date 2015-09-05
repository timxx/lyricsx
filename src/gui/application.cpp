#include "application.h"

#include <QSettings>
#include <QIcon>

LRCX_BEGIN_NS

Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
{
	setApplicationName("lrcx");
	setWindowIcon(QIcon(":/icons/lrcx.png"));

	m_settings.reset(new QSettings(QSettings::NativeFormat,
								   QSettings::UserScope,
								   QLatin1String("lrcx")
								   )
					 );
}

Application::~Application()
{
}

QVariant Application::settings(AppSettings as) const
{
	const QString &key = asToString(as);

	return m_settings->value(key);
}

void Application::setSetting(AppSettings as, const QVariant &value)
{
	const QString &key = asToString(as);
	m_settings->setValue(key, value);
}

const QString& Application::asToString(AppSettings as) const
{
	static std::map<AppSettings, QString> s_map =
	{
		{ AS_WindowPostion,      "WindowPos" },
		{ AS_WindowMaximized,    "WindowMax" },
	};

	return s_map[as];
}

LRCX_END_NS
