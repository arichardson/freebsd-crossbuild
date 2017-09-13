#pragma once
#include_next <stdlib.h>

int rpmatch(const char *response);
long long
strtonum(const char *numstr, long long minval, long long maxval,
         const char **errstrp);
