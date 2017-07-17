#pragma once

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
#include <db.h>
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

#define EFTYPE EINVAL
#define MAXLOGNAME      33              /* max login name length (incl. NUL) */
