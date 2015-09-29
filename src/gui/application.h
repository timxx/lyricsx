#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "config.h"

#include <QApplication>

class QSettings;

LRCX_BEGIN_NS

class Application : public QApplication
{
	Q_OBJECT
public:
	enum AppSettings
	{
		AS_WindowPostion,
		AS_WindowMaximized,
		AS_LrcLastDir,
		AS_MusicLastDir,
	};

public:
	Application(int &argc, char **argv);
	~Application();

	QVariant settings(AppSettings as) const;
	void setSetting(AppSettings as, const QVariant &value);

private:
	const QString &asToString(AppSettings as) const;

private:
	std::shared_ptr<QSettings> m_settings;
};

#define xApp qobject_cast<Application *>(qApp)

LRCX_END_NS

#endif // __APPLICATION_H__
