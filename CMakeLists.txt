cmake_minimum_required(VERSION 3.8)
project(freebsd-crossbuild LANGUAGES C)

if(NOT "${CMAKE_C_COMPILER_ID}" MATCHES "Clang")
    message(FATAL_ERROR "This code needs to be compiled with clang, set CMAKE_C_COMPILER=/path/to/clang")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(AddCrossbuildExecutable)
include(FeatureSummary)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    find_package(LibBSD REQUIRED)
    set(LINUX 1)
endif()
find_package(LibRoken REQUIRED)


set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options(-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=undef)
include_directories(include)



set(CHERIBSD_DIR "$ENV{HOME}/cheri/cheribsd" CACHE PATH "Path to the CheriBSD source repository")
if (NOT EXISTS "${CHERIBSD_DIR}/Makefile")
    message(FATAL_ERROR "Could not determine location of CheriBSD sources, set -DCHERIBSD_DIR=/path/to/cheribsd/")
endif()

function(prepend_path list prefix) # _list
    set(_tmp "")
    foreach(f ${${list}})
        LIST(APPEND _tmp "${prefix}/${f}")
    endforeach(f)
    set(${list} "${_tmp}" PARENT_SCOPE)
endfunction()
################################
# libc-pwcache (needed by mtree)
################################
add_crossbuild_library(pwcache STATIC contrib/libc-pwcache/pwcache.c)
target_include_directories(pwcache PRIVATE contrib/libc-pwcache)

if(LINUX)
    add_crossbuild_library(libc-vis STATIC ${CHERIBSD_DIR}/contrib/libc-vis/vis.c ${CHERIBSD_DIR}/contrib/libc-vis/unvis.c)
    target_include_directories(libc-vis PUBLIC ${CHERIBSD_DIR}contrib/libc-vis)
    target_compile_options(libc-vis PRIVATE -DHAVE_VIS=1 -DHAVE_SVIS=0)
endif()
#############################
# libnetbsd (needed by mtree)
#############################
set(libnetbsd_SRCS
    contrib/libnetbsd/util.c
    contrib/libnetbsd/strsuftoll.c
    contrib/libnetbsd/efun.c
    contrib/libnetbsd/sockaddr_snprintf.c
)
if(LINUX)
    list(APPEND libnetbsd_SRCS ${CHERIBSD_DIR}/lib/libc/gen/strtofflags.c)
endif()
add_crossbuild_library(netbsd STATIC ${libnetbsd_SRCS})

target_include_directories(netbsd PUBLIC contrib/libnetbsd)

############################
# libsbuf (needed by config)
############################
add_crossbuild_library(sbuf STATIC ${CHERIBSD_DIR}/sys/kern/subr_sbuf.c ${CHERIBSD_DIR}/sys/kern/subr_prf.c)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/sys)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/sys/sbuf.h" "sys/sbuf.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/sys/ctype.h" "sys/ctype.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(sbuf PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)

add_subdirectory(lib/libmd)

function(add_simple_crossbuild_program _name _path) # _other_sources
    add_crossbuild_executable(${_name} SOURCE_PATH ${CHERIBSD_DIR}/${_path} SOURCES ${ARGN})
endfunction()

# yacc and awk first as they are needed by other targets
add_subdirectory(src/yacc)
add_subdirectory(src/awk)
# add_subdirectory(src/lex)
add_subdirectory(src/sed)
add_subdirectory(src/expr)

add_simple_crossbuild_program(cat bin/cat cat.c)
add_simple_crossbuild_program(rmdir bin/rmdir rmdir.c)

add_simple_crossbuild_program(freebsd-test bin/test test.c)
set_target_properties(freebsd-test PROPERTIES OUTPUT_NAME freebsd-test)  # but install it as freebsd-test
install(PROGRAMS $<TARGET_FILE:freebsd-test> DESTINATION bin RENAME "freebsd-[")

# # different syntax on Linux
add_simple_crossbuild_program(mktemp usr.bin/mktemp mktemp.c)
add_subdirectory(src/mkdb)
add_subdirectory(src/compile_et)
add_subdirectory(src/grep)
add_subdirectory(src/file2c)
add_subdirectory(src/config)
add_subdirectory(src/makefs)
add_subdirectory(src/mtree)
add_subdirectory(src/rpcgen)
add_subdirectory(src/sh)
add_simple_crossbuild_program(tsort usr.bin/tsort tsort.c)
target_include_directories(tsort PRIVATE ${CHERIBSD_DIR}/contrib/uthash/src)
add_subdirectory(src/xinstall)

add_simple_crossbuild_program(gencat usr.bin/gencat gencat.c)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/include/nl_types.h" "nl_types.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(gencat PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")

include(ExternalProject)
ExternalProject_Add(mandoc
    URL http://mdocml.bsd.lv/snapshots/mandoc-1.14.3.tar.gz
    CONFIGURE_COMMAND <SOURCE_DIR>/configure
    BUILD_IN_SOURCE 1
    USES_TERMINAL_BUILD 1
    BUILD_COMMAND make
    INSTALL_COMMAND make install DESTDIR=<INSTALL_DIR>)
install(PROGRAMS ${CMAKE_BINARY_DIR}/mandoc-prefix/usr/local/bin/mandoc DESTINATION bin RENAME freebsd-mandoc)
install(PROGRAMS ${CMAKE_BINARY_DIR}/mandoc-prefix/usr/local/sbin/makewhatis DESTINATION bin RENAME freebsd-makewhatis)

install(PROGRAMS src/sysctl.py DESTINATION bin RENAME freebsd-sysctl)



feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
