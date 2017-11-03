# Build the host tools necessary for compiling FreeBSD on Linux and Mac:
# Many base system binaries have slightly different flags and semantics so
# we need to build the FreeBSD versions which behave the way the FreeBSD
# build system expects

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
    set(CMAKE_REQUIRED_FLAGS "-include ${CMAKE_SOURCE_DIR}/include/linux/pre-include.h")
elseif(APPLE)
    set(CMAKE_REQUIRED_FLAGS "-include ${CMAKE_SOURCE_DIR}/include/mac/pre-include.h")
endif()
find_package(LibRoken REQUIRED)
find_package(Threads REQUIRED)


set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options(-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type -Werror=undef)
include_directories(include)

# Config checks:
#file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/include)
include_directories(${CMAKE_BINARY_DIR}/include)
set(CMAKE_REQUIRED_DEFINITIONS -D_GNU_SOURCE=1 -D_DARWIN_C_SOURCE=1)
include(CheckStructHasMember)
include(CheckIncludeFile)
include(CheckSymbolExists)
check_struct_has_member("struct stat" st_flags sys/stat.h HAVE_STRUCT_STAT_ST_FLAGS LANGUAGE C)
check_struct_has_member("struct stat" st_birthtime sys/stat.h HAVE_STRUCT_STAT_BIRTHTIME LANGUAGE C)
check_struct_has_member("struct stat" st_mtimensec sys/stat.h HAVE_STRUCT_STAT_ST_MTIMENSEC LANGUAGE C)
set(STATFS_HEADERS)
check_include_file(netdb.h HAVE_NETDB_H)
check_include_file(sys/statfs.h HAVE_SYS_STATFS_H)
if(HAVE_SYS_STATFS_H)
    list(APPEND STATFS_HEADERS sys/statfs.h)
endif()
check_include_file(sys/mount.h HAVE_SYS_MOUNT_H)
if(HAVE_SYS_MOUNT_H)
    list(APPEND STATFS_HEADERS sys/mount.h)
endif()
check_struct_has_member("struct statfs" f_fstypename "${STATFS_HEADERS};sys/param.h" HAVE_STRUCT_STATFS_F_FSTYPENAME LANGUAGE C)
check_struct_has_member("struct statvfs" f_iosize "sys/statvfs.h" HAVE_STRUCT_STATVFS_F_IOSIZE LANGUAGE C)

check_symbol_exists(fstatvfs sys/statvfs.h HAVE_FSTATVFS)
check_symbol_exists(pwcache_userdb pwd.h HAVE_PWCACHE_USERDB)

configure_file(nbtool_config.h.cmake ${CMAKE_BINARY_DIR}/include/nbtool_config.h)



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
set(pwcache_SRCS ${CHERIBSD_DIR}/contrib/libc-pwcache/pwcache.c)
if(LINUX)
    list(APPEND pwcache_SRCS ${CMAKE_SOURCE_DIR}/include/linux/setent.c)
endif()
add_crossbuild_library(pwcache STATIC ${pwcache_SRCS})
# target_compile_definitions(pwcache PRIVATE HAVE_NBTOOL_CONFIG_H=0)
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
    ${CHERIBSD_DIR}/lib/libnetbsd/util.c
    ${CHERIBSD_DIR}/lib/libnetbsd/strsuftoll.c
    ${CHERIBSD_DIR}/lib/libnetbsd/efun.c
    ${CHERIBSD_DIR}/lib/libnetbsd/sockaddr_snprintf.c
)
if(LINUX)
    list(APPEND libnetbsd_SRCS ${CHERIBSD_DIR}/lib/libc/gen/strtofflags.c)
endif()
add_crossbuild_library(netbsd STATIC ${libnetbsd_SRCS})

target_include_directories(netbsd PUBLIC ${CHERIBSD_DIR}/lib/libnetbsd)

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
add_simple_crossbuild_program(mktemp usr.bin/mktemp mktemp.c)
add_subdirectory(src/yacc)
add_subdirectory(src/awk)
add_subdirectory(src/sed)
add_subdirectory(src/lex)
add_subdirectory(src/expr)

add_simple_crossbuild_program(cat bin/cat cat.c)
add_simple_crossbuild_program(rmdir bin/rmdir rmdir.c)

add_simple_crossbuild_program(freebsd-test bin/test test.c)
set_target_properties(freebsd-test PROPERTIES OUTPUT_NAME freebsd-test)  # but install it as freebsd-test
install(PROGRAMS $<TARGET_FILE:freebsd-test> DESTINATION bin RENAME "freebsd-[")

# # different syntax on Linux
add_subdirectory(src/mkdb)
add_subdirectory(src/compile_et)
add_subdirectory(src/file2c)
add_subdirectory(src/find)
add_simple_crossbuild_program(gensnmptree contrib/bsnmp/gensnmptree/ SOURCES gensnmptree.c)
check_symbol_exists(strlcpy string.h HAVE_STRLCPY)
if(HAVE_STRLCPY)
    target_compile_definitions(gensnmptree PRIVATE HAVE_STRLCPY=1)
endif()
check_include_file(err.h HAVE_ERR_H)
if(HAVE_ERR_H)
    target_compile_definitions(gensnmptree PRIVATE HAVE_ERR_H=1)
endif()




target_include_directories(gensnmptree PRIVATE ${CHERIBSD_DIR}/contrib/bsnmp/lib)
add_subdirectory(src/grep)
add_subdirectory(src/config)
add_subdirectory(src/makefs)
add_subdirectory(src/mtree)
add_subdirectory(src/rpcgen)
add_subdirectory(src/sh)
add_subdirectory(src/sort)
add_simple_crossbuild_program(strfile usr.bin/fortune/strfile strfile.c)
add_simple_crossbuild_program(tsort usr.bin/tsort tsort.c)
target_include_directories(tsort PRIVATE ${CHERIBSD_DIR}/contrib/uthash/src)
if(LINUX)
    target_link_libraries(tsort PRIVATE db)
endif()
add_simple_crossbuild_program(uuencode usr.bin/uuencode uuencode.c)
add_simple_crossbuild_program(uudecode usr.bin/uudecode uudecode.c)
if(LINUX OR APPLE)
    target_link_libraries(uuencode PRIVATE resolv)
    target_link_libraries(uudecode PRIVATE resolv)
endif()
add_simple_crossbuild_program(unifdef usr.bin/unifdef unifdef.c)
set(xargs_SRCS xargs.c strnsubst.c)
if(APPLE)
    list(APPEND xargs_SRCS ${CMAKE_SOURCE_DIR}/include/mac/strtonum.c)
endif()
add_simple_crossbuild_program(xargs usr.bin/xargs ${xargs_SRCS})


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

# ExternalProject_Add(bmake
#     URL http://www.crufty.net/ftp/pub/sjg/bmake-20170812.tar.gz
#     CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR>
#     BUILD_IN_SOURCE 1
#     USES_TERMINAL_BUILD 1
#     BUILD_COMMAND make
#     INSTALL_COMMAND make install)
# install(PROGRAMS ${CMAKE_BINARY_DIR}/bmake-prefix/bin/bmake DESTINATION bin RENAME freebsd-bmake)

install(PROGRAMS src/sysctl.py DESTINATION bin RENAME freebsd-sysctl)



feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
