#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#include "util.h"
#include "debug.h"

static const char* const TypeStringByEnum(print_type type)
{
	switch (type)
	{
	case error:
		return "Error";
	case warning:
		return "Warning";
	default:
		return "Info";
	}
}

void D_Printf(const char* const format, print_type type, ...)
{
	UTIL_Verify(type < 5);

	char buffer[256];
	const char* const typestr = TypeStringByEnum(type);

	va_list args;
	va_start(args, type);
	vsprintf(buffer, format, args);
	va_end(args);

	printf("[%s] %s\n", typestr, buffer);
}