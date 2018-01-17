#pragma once

#define HAVE_STRLCPY 1
#define HAVE_ERR_H 1
#define HAVE_GETADDRINFO 1

#if 0
#include <sys/cdefs.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <fcntl.h>
#include <time.h>
#include <stdio.h>
#include <stdint.h>

#include "../common/pre-include.h"


typedef __darwin_gid_t	__gid_t;
typedef __darwin_uid_t	__uid_t;
typedef __darwin_time_t	__time_t;
typedef __darwin_size_t __size_t;

#include <AvailabilityMacros.h>
#include <Availability.h>

#endif /* !ASSEMBLER */