cmake_minimum_required(VERSION 3.8)
project(freebsd-crossbuild)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

set(CMAKE_C_STANDARD 11)
add_compile_options(-Werror=implicit-function-declaration)
include_directories(include)
if ("${CMAKE_SYSTEM_NAME}" MATCHES "Darwin")
    add_compile_options(-include ${CMAKE_SOURCE_DIR}/include/mac/pre-include.h)
    # needed for the pwcache APIs
    add_definitions(-D_DARWIN_C_SOURCE=1)
endif()

function(prepend_path list prefix) # _list
    set(_tmp "")
    foreach(f ${${list}})
        LIST(APPEND _tmp "${prefix}/${f}")
    endforeach(f)
    set(${list} "${_tmp}" PARENT_SCOPE)
endfunction()

add_library(pwcache STATIC contrib/libc-pwcache/pwcache.c)
target_include_directories(pwcache PRIVATE contrib/libc-pwcache)

# TODO: build the other sources?
add_library(netbsd STATIC contrib/libnetbsd/util.c)
target_include_directories(netbsd PRIVATE contrib/libc-pwcache)


# TODO: mapages

################
# mtree
################
set(mtree_SRCS compare.c crc.c create.c excludes.c getid.c misc.c mtree.c only.c spec.c specspec.c verify.c)
prepend_path(mtree_SRCS contrib/mtree/)
list(APPEND mtree_SRCS contrib/mknod/pack_dev.c)

add_executable(mtree ${mtree_SRCS})
target_include_directories(mtree PRIVATE contrib/mtree contrib/libnetbsd contrib/mknod)
target_link_libraries(mtree PRIVATE pwcache netbsd)
# TODO: add FreeBSD lib/libmd ?
target_compile_definitions(mtree PUBLIC -DNO_MD5 -DNO_RMD160 -DNO_SHA1 -DNO_SHA2)

################
# rpcgen
################
set(rpcgen_SRCS rpc_main.c rpc_clntout.c rpc_cout.c rpc_hout.c rpc_parse.c rpc_sample.c rpc_scan.c
                rpc_svcout.c rpc_tblout.c rpc_util.c)
prepend_path(rpcgen_SRCS contrib/rpggen/)
add_executable(rpcgen ${rpcgen_SRCS})
target_include_directories(mtree PRIVATE contrib/rpcgen)


################
# compile_et
################
find_package(LibRoken REQUIRED)
# run yacc and lex
set(compile_et_SRCS contrib/com_err/compile_et.c heimdal_vers.c)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parse.c MAIN_DEPENDENCY contrib/com_err/parse.y
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/parse.h
        COMMAND yacc -d -o parse.c ${CMAKE_SOURCE_DIR}/contrib/com_err/parse.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lex.c MAIN_DEPENDENCY contrib/com_err/lex.l
        COMMAND lex  -o lex.c ${CMAKE_SOURCE_DIR}/contrib/com_err/lex.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND compile_et_SRCS ${CMAKE_CURRENT_BINARY_DIR}/parse.c ${CMAKE_CURRENT_BINARY_DIR}/lex.c)
add_executable(compile_et ${compile_et_SRCS})
target_include_directories(compile_et PRIVATE contrib/com_err ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(compile_et Heimdal::LibRoken)

install(TARGETS mtree rpcgen compile_et RUNTIME DESTINATION bin)

#CFLAGS+=	-I${SRCTOP}/contrib/mknod
#.PATH:		${SRCTOP}/contrib/mknod
#SRCS+=		pack_dev.c
# LIBADD=		netbsd md util
# LINKS=		${BINDIR}/mtree ${BINDIR}/nmtree
# MLINKS=		mtree.8 nmtree.8
