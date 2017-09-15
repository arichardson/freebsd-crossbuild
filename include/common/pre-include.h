#pragma once

#ifndef __unused
#define __unused __attribute__((unused))
#endif
#define __format_arg(fmtarg)    __attribute__((__format_arg__ (fmtarg)))
typedef unsigned long u_long;
typedef unsigned int u_int;
typedef size_t vaddr_t;
typedef uintptr_t __uintptr_t;
typedef int __nl_item;

#ifndef __va_list
#include <stdarg.h>
#define __va_list va_list
#endif
#ifndef __restrict
#define __restrict restrict
#endif

#ifndef __pure
#define __pure
#endif

// https://stackoverflow.com/questions/1598773/is-there-a-standard-function-in-c-that-would-return-the-length-of-an-array/1598827#1598827
#define nitems(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

#if !defined(__cplusplus)
#define __min_size(x)   static (x)
#else
#define __min_size(x)   (x)
#endif

int     check_utility_compat(const char *);

#define _open open
#define _close close
#define _write write
#define _read read
