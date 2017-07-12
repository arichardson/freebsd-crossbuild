cmake_minimum_required(VERSION 3.8)
project(freebsd-crossbuild)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Werror=implicit-function-declaration)
include_directories(include)


function(prepend_path list prefix) # _list
    set(_tmp "")
    foreach(f ${ARGN})
        LIST(APPEND _tmp "${prefix}/${f}")
    endforeach(f)
    set(${list} "${_tmp}" PARENT_SCOPE)
endfunction()

add_library(pwcache STATIC contrib/libc-pwcache/pwcache.c)
target_include_directories(pwcache PRIVATE contrib/libc-pwcache)

# TODO: build the other sources?
add_library(netbsd STATIC contrib/libnetbsd/util.c)
target_include_directories(netbsd PRIVATE contrib/libc-pwcache)


# TODO: mapages:
# MAN=		mtree.5 mtree.8
set(mtree_SRCS compare.c crc.c create.c excludes.c getid.c misc.c mtree.c only.c spec.c specspec.c verify.c)
prepend_path(mtree_SRCS contrib/mtree/ ${mtree_SRCS})
list(APPEND mtree_SRCS contrib/mknod/pack_dev.c)

add_executable(mtree ${mtree_SRCS})
target_include_directories(mtree PUBLIC contrib/mtree contrib/libnetbsd contrib/mknod)
target_link_libraries(mtree PRIVATE pwcache netbsd)

# TODO: add FreeBSD lib/libmd ?
target_compile_definitions(mtree PUBLIC -DNO_MD5 -DNO_RMD160 -DNO_SHA1 -DNO_SHA2)
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    target_compile_options(mtree PUBLIC -include ${CMAKE_SOURCE_DIR}/include/mac/pre-include.h)
    # needed for the pwcache APIs
    target_compile_definitions(mtree PUBLIC -D_DARWIN_C_SOURCE=1)
    target_compile_definitions(pwcache PUBLIC -D_DARWIN_C_SOURCE=1)
endif()

install(TARGETS mtree RUNTIME DESTINATION bin)

#CFLAGS+=	-I${SRCTOP}/contrib/mknod
#.PATH:		${SRCTOP}/contrib/mknod
#SRCS+=		pack_dev.c
# LIBADD=		netbsd md util
# LINKS=		${BINDIR}/mtree ${BINDIR}/nmtree
# MLINKS=		mtree.8 nmtree.8
