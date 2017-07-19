#pragma once

#include_next <signal.h>

#define sys_nsig _NSIG
#define sys_signame sys_siglist
