#pragma once

static inline int
__libc_sigprocmask(int how, const sigset_t *set, sigset_t *oldset) {
  return sigprocmask(how, set, oldset);
};

#define __isthreaded 0

#define __weak_reference(a, b)
