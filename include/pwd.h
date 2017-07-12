#pragma once

#include_next <pwd.h>

int
pwcache_userdb(
        int		(*a_setpassent)(int),
        void		(*a_endpwent)(void),
        struct passwd *	(*a_getpwnam)(const char *),
        struct passwd *	(*a_getpwuid)(uid_t));

int
uid_from_user(const char *name, uid_t *uid);
