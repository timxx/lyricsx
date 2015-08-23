#include "log.h"

#include <cstdarg>

LRCX_BEGIN_NS

void Log::log(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);

	vprintf(fmt, args);

	va_end(args);
}

LRCX_END_NS
