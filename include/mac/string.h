#pragma once
#include_next <string.h>

static inline const char *
strchrnul(const char *s, int c)
{
  const char *ptr = strchr(s, c);
  if (!ptr)
    ptr = strchr(s, '\0');
  return ptr;
}