cmake_minimum_required(VERSION 3.8)
project(freebsd-crossbuild)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake ${CMAKE_MODULE_PATH})

include(AddCrossbuildExecutable)
include(FeatureSummary)

if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
    find_package(LibBSD REQUIRED)
    set(LINUX 1)
endif()

set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options(-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type)
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
#############################
# libnetbsd (needed by mtree)
#############################
add_crossbuild_library(netbsd STATIC
    contrib/libnetbsd/util.c
    contrib/libnetbsd/strsuftoll.c
    contrib/libnetbsd/efun.c
    contrib/libnetbsd/sockaddr_snprintf.c
)
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
add_subdirectory(src/sed)


add_simple_crossbuild_program(cat bin/cat cat.c)

add_subdirectory(src/mkdb)
add_subdirectory(src/compile_et)
add_subdirectory(src/file2c)
add_subdirectory(src/config)
add_subdirectory(src/makefs)
add_subdirectory(src/mtree)
add_subdirectory(src/rpcgen)
add_subdirectory(src/xinstall)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
