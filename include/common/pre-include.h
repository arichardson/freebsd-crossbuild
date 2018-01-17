#pragma once

typedef unsigned long u_long;
typedef unsigned int u_int;
typedef size_t vaddr_t;
typedef uintptr_t __uintptr_t;
typedef int __nl_item;

int     check_utility_compat(const char *);

#define _open open
#define _close close
#define _write write
#define _read read
