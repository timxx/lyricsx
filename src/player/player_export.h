/**
 * Copyright (c) 2016 Weitian Leung
 *
 * This file is part of LyricsX.
 *
 * This file is distributed under the MIT License.
 * See the LICENSE file for details.
 *
*/

#ifndef __PLAYER_EXPORT_H__
#define __PLAYER_EXPORT_H__

#include "config.h"

#ifdef player_EXPORTS
#  define PLAYER_EXPORT LRCX_EXPORT
#else
#  define PLAYER_EXPORT LRCX_IMPORT
#endif

#endif // __PLAYER_EXPORT_H__
