// we are not building this as part of libc so an empty file works fine

#pragma once

#define _fstat(fd, buf) fstat(fd, buf)
#define _fsync(fd) fsync(fd)
#define _writev(a1, a2, a3) writev(a1, a2, a3)


#define _pthread_rwlock_rdlock pthread_rwlock_rdlock
#define _pthread_rwlock_wrlock pthread_rwlock_wrlock
#define _pthread_rwlock_unlock pthread_rwlock_unlock
#define _pthread_key_create pthread_key_create
#define _pthread_getspecific pthread_getspecific
#define _pthread_setspecific pthread_setspecific

#define _pthread_once pthread_once


#define nsdispatch _nsdispatch

#define __USE_GNU 1
#include <dlfcn.h>
#define libc_dlopen(a1, a2) dlopen(a1, a2)
