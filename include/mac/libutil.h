#pragma once

#include <stdio.h>
#include <sys/socket.h>

#if __has_include(</usr/include/util.h>)
#include </usr/include/util.h>
#elif __has_include_next(<libutil.h>)
#include_next <libutil.h>
#else
#endif

