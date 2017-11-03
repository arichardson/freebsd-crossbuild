// we are not building this as part of libc so an empty file works fine

#pragma once

#define _fstat(fd, buf) fstat(fd, buf)
#define _fsync(fd) fsync(fd)
#define _writev(a1, a2, a3) writev(a1, a2, a3)
