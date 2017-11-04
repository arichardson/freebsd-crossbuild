#pragma once
#include <unistd.h>
#include <sys/syscall.h>   /* For SYS_xxx definitions */


// https://stackoverflow.com/questions/4867839/how-can-i-tell-if-pthread-self-is-the-main-first-thread-in-the-process
static inline int pthread_main_np(void) { return syscall(SYS_gettid) == getpid(); }
