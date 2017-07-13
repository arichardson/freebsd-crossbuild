cmake_minimum_required(VERSION 3.8)
project(freebsd-crossbuild)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

include(AddCrossbuildExecutable)

set(CMAKE_C_STANDARD 11)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
add_compile_options(-Werror=implicit-function-declaration -Werror=implicit-int -Werror=return-type)
include_directories(include)

function(prepend_path list prefix) # _list
    set(_tmp "")
    foreach(f ${${list}})
        LIST(APPEND _tmp "${prefix}/${f}")
    endforeach(f)
    set(${list} "${_tmp}" PARENT_SCOPE)
endfunction()

add_crossbuild_library(pwcache STATIC contrib/libc-pwcache/pwcache.c)
target_include_directories(pwcache PRIVATE contrib/libc-pwcache)

# TODO: build the other sources?
add_crossbuild_library(netbsd STATIC contrib/libnetbsd/util.c)

set(CHERIBSD_DIR "$ENV{HOME}/cheri/cheribsd" CACHE PATH "Path to the CheriBSD source repository")
if (NOT EXISTS "${CHERIBSD_DIR}/Makefile")
    message(FATAL_ERROR "Could not determine location of CheriBSD sources, set -DCHERIBSD_DIR=/path/to/cheribsd/")
endif()

# yacc and awk first as they are needed by other targets
add_subdirectory(src/yacc)
add_subdirectory(src/awk)

add_subdirectory(src/compile_et)
add_subdirectory(src/mtree)
add_subdirectory(src/rpcgen)
