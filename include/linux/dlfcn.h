#pragma once

#include_next <dlfcn.h>

#define dlfunc(handle, sym) dlsym(handle, sym)
