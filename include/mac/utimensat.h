#include <errno.h>
#include <stdlib.h>

#define UTIME_NOW	((1l << 30) - 1l)
#define UTIME_OMIT	((1l << 30) - 2l)

// https://opensource.apple.com/source/sudo/sudo-83/sudo/lib/util/utimens.c.auto.html


//#if defined(HAVE_ST_MTIM)
//# ifdef HAVE_ST__TIM
//#  define ATIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_atim.st__tim)
//#  define MTIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_mtim.st__tim)
//# else
//#  define ATIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_atim)
//#  define MTIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_mtim)
//# endif
//#elif defined(HAVE_ST_MTIMESPEC)
# define ATIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_atimespec)
# define MTIME_TO_TIMEVAL(_x, _y)	TIMESPEC_TO_TIMEVAL((_x), &(_y)->st_mtimespec)
//#else
//# define ATIME_TO_TIMEVAL(_x, _y)      do { (_x)->tv_sec = (_y)->st_atime; (_x)->tv_usec = 0; } while (0)
//# define MTIME_TO_TIMEVAL(_x, _y)      do { (_x)->tv_sec = (_y)->st_mtime; (_x)->tv_usec = 0; } while (0)
//#endif /* HAVE_ST_MTIM */


/*
 * Convert the pair of timespec structs passed to futimens() / utimensat()
 * to a pair of timeval structs, handling UTIME_OMIT and UTIME_NOW.
 * Returns 0 on success and -1 on failure (setting errno).
 */
static inline int
utimens_ts_to_tv(int fd, const char *file, const struct timespec *ts,
                 struct timeval *tv)
{
    TIMESPEC_TO_TIMEVAL(&tv[0], &ts[0]);
    TIMESPEC_TO_TIMEVAL(&tv[1], &ts[1]);
    if (ts[0].tv_nsec == UTIME_OMIT || ts[1].tv_nsec == UTIME_OMIT) {
        struct stat sb;

        if (fd != -1) {
            /* For futimens() */
            if (fstat(fd, &sb) == -1)
                return -1;
        } else {
            /* For utimensat() */
            if (stat(file, &sb) == -1)
                return -1;
        }
        if (ts[0].tv_nsec == UTIME_OMIT)
            ATIME_TO_TIMEVAL(&tv[0], &sb);
        if (ts[1].tv_nsec == UTIME_OMIT)
            MTIME_TO_TIMEVAL(&tv[1], &sb);
    }
    if (ts[0].tv_nsec == UTIME_NOW || ts[1].tv_nsec == UTIME_NOW) {
        struct timeval now;

        if (gettimeofday(&now, NULL) == -1)
            return -1;
        if (ts[0].tv_nsec == UTIME_NOW)
            tv[0] = now;
        if (ts[1].tv_nsec == UTIME_NOW)
            tv[1] = now;
    }
    return 0;
}

// extern int utimensat(int fd, const char *file, const struct timespec *ts, int flag);

/*
 * Emulate utimensat() via utimes()
 */
static inline int
utimensat(int fd, const char *file, const struct timespec *ts, int flag)
{
    struct timeval tv[2], *times = NULL;

    if (fd != AT_FDCWD || flag != 0) {
        errno = ENOTSUP;
        abort();
        return -1;
    }

    if (ts != NULL) {
        if (utimens_ts_to_tv(-1, file, ts, tv) == -1)
            return -1;
        times = tv;
    }
    return utimes(file, times);
}