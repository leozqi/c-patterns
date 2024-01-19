#ifndef COMMON_STRING
#define COMMON_STRING

#include "common.h"
#include "err.h"

/**
 * Checks if string empty
 *
 */
int common_string_empty(char *str)
{
	if (str == NULL) {
		return 1;
	} else if (str[0] == '\0') {
		return 1;
	} else {
		return 0;
	}
} // common_string_empty

#endif // ifndef COMMON_STRING
