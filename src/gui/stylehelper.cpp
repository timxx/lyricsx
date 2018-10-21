/**
 * Copyright (c) 2018 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#include "stylehelper.h"
#include <QApplication>
#include <QScreen>

#ifndef Q_OS_MAC

static double defaultDpiX()
{
	auto *screen = qApp->primaryScreen();
	if (screen)
		return qRound(screen->logicalDotsPerInchX());

	return 96.0;
}

static double dpiXRate()
{
	static double s_rate = defaultDpiX() / 96.0;
	return s_rate;
}

#endif


LRCX_BEGIN_NS

double StyleHelper::dpiScaled(double value)
{
	// it is said that on Mac the DPI is always 72
#ifdef Q_OS_MAC
	return value;
#else
	return value * dpiXRate();
#endif
}

QSize StyleHelper::dpiScaled(const QSize &size)
{
	return QSize(dpiScaled(size.width()),
				 dpiScaled(size.height()));
}

QSizeF StyleHelper::dpiScaled(const QSizeF &size)
{
	return QSizeF(dpiScaled(size.width()),
				 dpiScaled(size.height()));
}

LRCX_END_NS
