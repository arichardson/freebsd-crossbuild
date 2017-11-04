add_subdirectory(libdb)
add_subdirectory(libmd)

if(APPLE)
    add_crossbuild_library(reallocarray STATIC ${CHERIBSD_DIR}/lib/libc/stdlib/reallocarray.c)
    add_crossbuild_library(fake-_DYNAMIC STATIC fake-_dynamic-symbol.c)
endif()