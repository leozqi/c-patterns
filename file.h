#ifndef COMMON_FILE
#define COMMON_FILE

#include "common.h"
#include "err.h"

/**
 * SPDX-License-Identifier: Unlicense
 * Source: https://raw.githubusercontent.com/brenns10/lsh/master/src/main.c
 *
 * Gets single line.
 *
 */
char *common_get_line(FILE *stream)
{
	int buffsize = COMMON_LINE_SZ;
	int pos = 0;
	char *buff = malloc(sizeof(char) * buffsize);
	int c;

	if (!buff) common_err_alloc();

	while (true) {
		c = fgetc(stream);

		if (c == EOF || c == '\n') {
			buff[pos] = '\0';
			return buff;
		} else {
			buff[pos] = c;
		}
		pos++;

		if (pos >= buffsize) {
			buffsize += COMMON_LINE_SZ;
			buff = realloc(buff, buffsize);

			if (!buff) common_err_alloc();
		}
	}
} // common_read_line

#endif // ifndef COMMON_FILE
