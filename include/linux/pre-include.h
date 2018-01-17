#pragma once
#ifndef __ASSEMBLER__

#include <features.h>

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include <limits.h>
#include <fcntl.h>

#include "../common/pre-include.h"

#define _SIZE_T_DECLARED
#define MAXBSIZE        65536   /* must be power of 2 */

// needed by libsbuf
#define	rounddown(x, y)	(((x)/(y))*(y))
#define	rounddown2(x, y) ((x)&(~((y)-1)))          /* if y is power of two */
//#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#define	roundup2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
// #define powerof2(x)	((((x)-1)&(x))==0)

char *
fflagstostr(u_long flags);

int
strtofflags(char **stringp, u_long *setp, u_long *clrp);

// #define st_atim st_atimespec
// #define st_mtim st_mtimespec
// #define st_ctim st_ctimespec

# define st_atimensec st_atim.tv_nsec	/* Backward compatibility.  */
# define st_mtimensec st_mtim.tv_nsec
# define st_ctimensec st_ctim.tv_nsec

#ifndef GID_MAX
#define GID_MAX 65536
#endif


#ifndef UID_MAX
#define UID_MAX 65536
#endif

#define OFF_MAX ((off_t)((1ULL << (sizeof(off_t) / 8)) - 1))
#define QUAD_MAX INT64_MAX

#define EFTYPE EINVAL
#define MAXLOGNAME      33              /* max login name length (incl. NUL) */

// cap_mkdb
int cgetnext(char **bp, char **db_array);
int cgetmatch(const char *buf, const char *name);
int cgetclose(void);

#define	__printf0like(fmtarg, firstvararg) \
	    __attribute__((__format__ (__printf0__, fmtarg, firstvararg)))


#define       ALIGN(p)                _ALIGN(p)
/*
 * Round p (pointer or byte index) up to a correctly-aligned value
 * for all data types (int, long, ...).   The result is unsigned int
 * and must be cast to any desired pointer type.
 */
#define	_ALIGNBYTES	(sizeof(long) - 1)
#define	_ALIGN(p)	(((__uintptr_t)(p) + _ALIGNBYTES) & ~_ALIGNBYTES)

#endif /* !ASSEMBLER */