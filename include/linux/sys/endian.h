#pragma once

#include_next <sys/endian.h>

/*
 * General byte order swapping functions.
 */
#define bswap16(x)      __builtin_bswap16(x)
#define bswap32(x)      __builtin_bswap32(x)
#define bswap64(x)      __builtin_bswap64(x)
