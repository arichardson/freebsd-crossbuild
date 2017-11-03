#pragma once

static inline int
__libc_sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  return sigprocmask(how, set, oldset);
};
#define  _fstat(fd, buf) fstat(fd, buf)
