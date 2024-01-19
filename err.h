#ifndef COMMON_ERR
#define COMMON_ERR

#include "common.h"

void common_err_alloc(void)
{
	fprintf(stderr, "COMMON: allocation error\n");
	exit(EXIT_FAILURE);
}

#endif // ifndef COMMON_ERR
