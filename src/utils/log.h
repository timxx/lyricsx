#ifndef __UTILS_LOG_H__
#define __UTILS_LOG_H__

#include "utils_export.h"

LRCX_BEGIN_NS

struct UTILS_EXPORT Log
{
	static void log(const char *fmt, ...);
};

LRCX_END_NS

#define utils_log(x) Log::log(x)
#define utils_log_error(x)  Log::log("%s +%d: %s\n", __PRETTY_FUNCTION__, __LINE__, x)

#endif // __UTILS_LOG_H__
