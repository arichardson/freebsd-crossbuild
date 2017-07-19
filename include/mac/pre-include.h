#pragma once

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#define st_atim st_atimespec
#define st_mtim st_mtimespec
#define st_ctim st_ctimespec
#define st_btim st_btimespec
#define st_atimensec st_atimespec.tv_nsec
#define st_mtimensec st_mtimespec.tv_nsec
#define st_ctimensec st_ctimespec.tv_nsec
#define st_birthtimensec st_birthtimespec.tv_nsec
#include <sys/time.h>

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#include "../common/pre-include.h"


typedef __darwin_gid_t	__gid_t;
typedef __darwin_uid_t	__uid_t;
typedef __darwin_time_t	__time_t;
typedef __darwin_size_t __size_t;

// needed by libsbuf
#define	rounddown(x, y)	(((x)/(y))*(y))
#define	rounddown2(x, y) ((x)&(~((y)-1)))          /* if y is power of two */
//#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#define	roundup2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#define powerof2(x)	((((x)-1)&(x))==0)

#include "utimensat.h"
