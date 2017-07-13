#pragma once

#include <sys/cdefs.h>
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
