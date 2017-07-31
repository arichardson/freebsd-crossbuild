#pragma once

#include_next <signal.h>
#include <sys/signal.h>

#define sys_nsig __DARWIN_NSIG
#define sys_signame sys_siglist
