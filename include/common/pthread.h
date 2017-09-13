#pragma once

#include_next <pthread.h>

#define PTHREAD_DETACHED PTHREAD_CREATE_DETACHED

#ifndef PTHREAD_MUTEX_ADAPTIVE_NP
#define PTHREAD_MUTEX_ADAPTIVE_NP PTHREAD_MUTEX_RECURSIVE
#endif

#ifdef __APPLE__
#define pthread_yield pthread_yield_np
#endif