#pragma once

#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdint.h>

#ifndef __va_list
#include <stdarg.h>
#define __va_list va_list
#endif
#ifndef __restrict
#define __restrict restrict
#endif

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef size_t vaddr_t;

// https://stackoverflow.com/questions/1598773/is-there-a-standard-function-in-c-that-would-return-the-length-of-an-array/1598827#1598827
// TODO: this should also check that it is an array
#define nitems(x) ((sizeof(x)/sizeof(0[x])) / ((size_t)(!(sizeof(x) % sizeof(0[x])))))

// needed by libsbuf
#define	rounddown(x, y)	(((x)/(y))*(y))
#define	rounddown2(x, y) ((x)&(~((y)-1)))          /* if y is power of two */
//#define	roundup(x, y)	((((x)+((y)-1))/(y))*(y))  /* to any y */
#define	roundup2(x, y)	(((x)+((y)-1))&(~((y)-1))) /* if y is powers of two */
#define powerof2(x)	((((x)-1)&(x))==0)
