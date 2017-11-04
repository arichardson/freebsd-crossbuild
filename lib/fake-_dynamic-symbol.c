// ELF systems have a valid _DYNAMIC symbol when dynamically linked but on
// MacOS this is not the case. Some FreeBSD sources assume they can test for
// _DYNAMIC so just define it here
#ifndef __APPLE__
#error "Should only be compiled on Apple platforms"
#endif
int _DYNAMIC = 1;