#include "util.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

void die(int code, char *where, char *fmt, ...) {
	va_list args;
	va_start(args, fmt);

	fprintf(stderr, "%s: ", where);

	vfprintf(stderr, fmt, args);
	fputc('\n', stderr);

	va_end(args);

	exit(code);
}

void die_errno(char *where) {
	int code = errno;

	perror(where);
	exit(code);
}
