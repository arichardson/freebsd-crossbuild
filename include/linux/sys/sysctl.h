#pragma once
/* Linux sysctl.h has an identifier named __unused */
#undef __unused
#include_next <sys/sysctl.h>
#define __unused __attribute__((__unused__))
