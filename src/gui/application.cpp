/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "application.h"

#include <QSettings>
#include <QIcon>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>

LRCX_BEGIN_NS

Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
{
	setApplicationName("lrcx");
	setWindowIcon(QIcon::fromTheme(QLatin1String("lyricsx")));

	QString translation_dirs[] =
	{
		applicationDirPath() + "/translations",
		QStringLiteral(LRCX_TRANSLAIONS_DIR),
	};

	for (const QString &dir : translation_dirs)
	{
		QTranslator *translator = new QTranslator;
		if (translator->load(QLocale::system(), QString(), QString(), dir))
			installTranslator(translator);
		else
			delete translator;
	}

	// Qt translations
	{
		QTranslator *translator = new QTranslator;
		QString path = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
		QString name = QLocale::system().name();
		if(translator->load(QLocale::system(), "qt", "_", QLibraryInfo::location(QLibraryInfo::TranslationsPath)))
			installTranslator(translator);
		else
			delete translator;
	}

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
		{ AS_LrcLastDir,         "LrcLastDir" },
		{ AS_MusicLastDir,       "MusicLastDir" },
		{ AS_EditorFontFamily,   "EditorFont" },
		{ AS_EditorFontStyle,    "EditorFontStyle" },
		{ AS_EditorFontSize,     "EditorFontSize" },
		{ AS_LrcEditor,          "LrcEditor" },
	};

	return s_map[as];
}

LRCX_END_NS
