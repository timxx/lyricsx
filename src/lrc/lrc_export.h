#ifndef __LRC_EXPORT_H__
#define __LRC_EXPORT_H__

#include "config.h"

#ifdef lrc_EXPORTS
#  define LRC_EXPORT LRCX_EXPORT
#else
#  define LRC_EXPORT LRCX_IMPORT
#endif

#endif // __LRC_EXPORT_H__
