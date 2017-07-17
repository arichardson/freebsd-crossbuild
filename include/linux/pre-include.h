#pragma once

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#include <limits.h>
#include <db.h>
#include <fcntl.h>


#ifndef __va_list
#include <stdarg.h>
#define __va_list va_list
#endif
#ifndef __restrict
#define __restrict restrict
#endif

#define __unused __attribute__((unused))

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef size_t vaddr_t;
typedef uintptr_t __uintptr_t;

#define _SIZE_T_DECLARED
#define MAXBSIZE        65536   /* must be power of 2 */

// https://stackoverflow.com/questions/1598773/is-there-a-standard-function-in-c-that-would-return-the-length-of-an-array/1598827#1598827
// TODO: this should also check that it is an array
#define nitems(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// needed by libsbuf
#define	rounddown(x, y)	(((x)/(y))*(y))
#define	rounddown2(x, y) ((x)&(~((y)-1)))          /* if y is power of two */
//#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#define	roundup2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
// #define powerof2(x)	((((x)-1)&(x))==0)

#if !defined(__cplusplus)
#define __min_size(x)   static (x)
#else
#define __min_size(x)   (x)
#endif

char *
fflagstostr(u_long flags);

int
strtofflags(char **stringp, u_long *setp, u_long *clrp);

// #define st_atim st_atimespec
// #define st_mtim st_mtimespec
// #define st_ctim st_ctimespec

// typedef __darwin_gid_t	__gid_t;
// typedef __darwin_uid_t	__uid_t;
// typedef __darwin_time_t	__time_t;
// typedef __darwin_size_t __size_t;
