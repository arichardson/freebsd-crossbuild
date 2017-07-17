#pragma once
#include_next <wctype.h>

#define        iswascii(wc)            (((wc) & ~0x7F) == 0)
