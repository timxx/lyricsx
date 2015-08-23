#ifndef __UTILS_EXPORT_H__
#define __UTILS_EXPORT_H__

#include "config.h"

#ifdef utils_EXPORTS
#  define UTILS_EXPORT LRCX_EXPORT
#else
#  define UTILS_EXPORT LRCX_IMPORT
#endif

#endif // __UTILS_EXPORT_H__
