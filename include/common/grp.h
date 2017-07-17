#pragma once

#include_next <grp.h>

int
pwcache_groupdb(
        int		(*a_setgroupent)(int),
        void		(*a_endgrent)(void),
        struct group *	(*a_getgrnam)(const char *),
        struct group *	(*a_getgrgid)(gid_t));

int
gid_from_group(const char *name, gid_t *gid);

int
setgroupent(int stayopen);

int
gid_from_group(const char *name, gid_t *gid);

#ifndef __APPLE__
const
#endif
char *
group_from_gid(gid_t gid, int noname);
