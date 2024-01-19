/************************************************
 *
 * Simple implementations of common CLI utils.
 *
 * @file       ls.c
 * @author     Leo Qi <leo@leozqi.com>
 * @version    0.1
 * @license    Apache-2.0
 *
 ***********************************************/

#include <stdio.h>
#include <stdbool.h>
#include <dirent.h>
#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/ioctl.h>
#include <stdint.h>
#include <stdarg.h>

#ifndef PATH_MAX
#define PATH_MAX 255
#endif

#define LS_ERR_ALLOC ("ls.c: alloc error")
#define LS_ERR_UNABLE_TO_READ ("ls.c: unable to read directory")
#define LS_ERR_NOT_EXISTS ("ls.c: directory does not exist")

enum { LS_a = 1 << 0, LS_l = 1 << 1, LS_m = 1 << 2 };

static uint16_t _add_flags(uint16_t flags, char const *c)
{
	while (*c) {
		switch (*c) {
		case 'a':
			flags |= LS_a;
			break;
		case 'l':
			flags |= LS_l;
			break;
		case 'm':
			flags |= LS_m;
			break;
		}
		c++;
	}
	return flags;
}

static char const *_get_mode(struct stat *p_stat)
{
	char *s = malloc(16 + sizeof(""));

	if (S_ISDIR(p_stat->st_mode)) {
		s[0] = 'd';
	} else if (S_ISBLK(p_stat->st_mode)) {
		s[0] = 'b';
	} else if (S_ISCHR(p_stat->st_mode)) {
		s[0] = 'c';
	} else if (S_ISLNK(p_stat->st_mode)) {
		s[0] = 'l';
	} else if (S_ISFIFO(p_stat->st_mode)) {
		s[0] = 'p';
	} else {
		s[0] = '-';
	}

	s[1] = p_stat->st_mode & S_IRUSR ? 'r' : '-';
	s[2] = p_stat->st_mode & S_IWUSR ? 'w' : '-';
	s[3] = p_stat->st_mode & S_IXUSR ? 'x' : '-';
	s[4] = p_stat->st_mode & S_IRGRP ? 'r' : '-';
	s[5] = p_stat->st_mode & S_IWGRP ? 'w' : '-';
	s[6] = p_stat->st_mode & S_IXGRP ? 'x' : '-';
	s[7] = p_stat->st_mode & S_IROTH ? 'r' : '-';
	s[8] = p_stat->st_mode & S_IWOTH ? 'w' : '-';
	s[9] = p_stat->st_mode & S_IXOTH ? 'x' : '-';
	s[10] = '\0';

	return (char const *)s;
}

static size_t _tprint(size_t term_w, size_t ln_w, char const *s)
{
	size_t sz = strlen(s);
	if (ln_w + sz > term_w) {
		fprintf(stdout, "\n%s", s);
		ln_w = sz;
	} else {
		fputs(s, stdout);
		ln_w += sz;
	}
	return ln_w;
}

static void _handle_error(int err, int tofree, ...)
{
	va_list args;
	va_start(args, tofree);

	void *ptr;
	char *err_msg;

	for (int i = 0; i < tofree; ++i) {
		ptr = va_arg(args, void *);
		if (ptr)
			free(ptr);
	}

	va_end(args);

	switch (err) {
	case ENOENT:
		err_msg = LS_ERR_NOT_EXISTS;
		break;
	case ENOMEM:
		err_msg = LS_ERR_ALLOC;
		break;
	default:
		err_msg = LS_ERR_UNABLE_TO_READ;
	}

	fprintf(stderr, "%s\n", err_msg);
}

static bool _ls(char const *dir, uint16_t const flags)
{
	char *p_buffer = NULL;
	struct stat *p_stat = NULL;

	struct dirent *d;
	DIR *dh = opendir(dir);
	if (!dh) {
		_handle_error(errno, 0);
		return false;
	}

	struct winsize w;
	ioctl(0, TIOCGWINSZ, &w);
	size_t term_w = w.ws_col;
	size_t ln_w = 0;

	bool first = true;

	if (flags & LS_l) {
		p_buffer = malloc(PATH_MAX + 1);
		p_stat = malloc(sizeof(struct stat));
		if (!p_buffer || !p_stat) {
			_handle_error(ENOMEM, 2, p_buffer, p_stat);
			return false;
		}

		if (lstat(dir, p_stat) < 0) {
			_handle_error(errno, 2, p_buffer, p_stat);
			return false;
		}

		fprintf(stdout, "total %u\n", (unsigned int)p_stat->st_blocks);
	}

	while ((d = readdir(dh))) {
		if ((!(flags & LS_a)) && d->d_name[0] == '.')
			continue;

		if (first) {
			first = false;
		} else {
			if (flags & LS_l) {
				fputs("\n", stdout);
				ln_w = 0;
			} else if (flags & LS_m) {
				fputs(", ", stdout);
				ln_w += 2;
			} else {
				fputs(" ", stdout);
				ln_w += 1;
			}
		}

		if (!(flags & LS_l)) {
			ln_w = _tprint(term_w, ln_w, d->d_name);
		} else {
			sprintf(p_buffer, "%s/%s", dir, d->d_name);

			if (lstat(p_buffer, p_stat) < 0) {
				_handle_error(errno, 2, p_buffer, p_stat);
				return false;
			}
			fprintf(stdout, "%10s  %s", _get_mode(p_stat),
				d->d_name);
			/*
			fprintf(stdout, "%12s %4s %8s %5s %5s %2s %3s %6s %25s",
				_get_mode(p_stat),
			);*/
		}
	}
	fputs("\n", stdout);

	if (p_buffer)
		free(p_buffer);
	if (p_stat)
		free(p_stat);

	return 0;
}

int main(int argc, char const *argv[])
{
	size_t i = 0;
	bool first = true;
	uint16_t flags = 0;

	for (i = 1; i < (size_t)argc; ++i) {
		if (argv[i][0] != '-')
			break;

		flags = _add_flags(flags, (char const *)argv[i]);
	}

	if (i >= (size_t)argc) {
		_ls(realpath(".", NULL), flags);
	} else if (i + 1 == (size_t)argc) {
		_ls(realpath(argv[i], NULL), flags);
	} else {
		for (; i < (size_t)argc; ++i) {
			if (first) {
				first = false;
			} else {
				fputs("\n", stdout);
			}
			fprintf(stdout, "%s:\n", argv[i]);
			_ls(realpath(argv[i], NULL), flags);
		}
	}

	return 0;
}
