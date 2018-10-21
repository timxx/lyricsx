/**
 * Copyright (c) 2018 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/


#ifndef __STYLE_HELPER_H__
#define __STYLE_HELPER_H__

#include "config.h"

#include <QSize>

LRCX_BEGIN_NS

namespace StyleHelper
{
	double dpiScaled(double value);

	QSize dpiScaled(const QSize &size);
	QSizeF dpiScaled(const QSizeF &size);
}

LRCX_END_NS

#endif // __STYLE_HELPER_H__
