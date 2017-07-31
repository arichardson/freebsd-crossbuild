#pragma once

#include_next <unistd.h>

#include <sys/signal.h>

static inline int
eaccess(const char *path, int mode) {
    return faccessat(AT_FDCWD, path, mode, AT_EACCESS);
}

static inline char *
strchrnul(const char *s, int c)
{
    char *ptr = strchr(s, c);
    if (!ptr)
        ptr = strchr(s, '\0');
    return ptr;
}