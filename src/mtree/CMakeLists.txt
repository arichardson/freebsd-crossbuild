################
# mtree
################
set(mtree_SRCS compare.c crc.c create.c excludes.c getid.c misc.c mtree.c only.c spec.c specspec.c verify.c)
list(APPEND mtree_SRCS ${CMAKE_SOURCE_DIR}/contrib/mknod/pack_dev.c)
add_crossbuild_executable(mtree SOURCE_PATH ${CHERIBSD_DIR}/contrib/mtree SOURCES ${mtree_SRCS})
target_include_directories(mtree PRIVATE ${CMAKE_SOURCE_DIR}/contrib/libnetbsd ${CMAKE_SOURCE_DIR}/contrib/mknod)
# TODO: add FreeBSD lib/libmd for the hashes?
target_link_libraries(mtree PRIVATE FreeBSD::libpwcache FreeBSD::libnetbsd)
target_compile_definitions(mtree PUBLIC -DNO_MD5 -DNO_RMD160 -DNO_SHA1 -DNO_SHA2)
