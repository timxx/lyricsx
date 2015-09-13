#ifndef __PLAYER_EXPORT_H__
#define __PLAYER_EXPORT_H__

#include "config.h"

#ifdef player_EXPORTS
#  define PLAYER_EXPORT LRCX_EXPORT
#else
#  define PLAYER_EXPORT LRCX_IMPORT
#endif

#endif // __PLAYER_EXPORT_H__
