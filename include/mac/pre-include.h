#pragma once

#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/time.h>
#include <stdio.h>

#ifndef __va_list
#include <stdarg.h>
#define __va_list va_list
#endif
#ifndef __restrict
#define __restrict restrict
#endif

typedef unsigned long u_long;
typedef unsigned int u_int;
