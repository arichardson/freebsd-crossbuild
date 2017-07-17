################
# mtree
###############
set(SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/makefs)
set(makefs_SRCS cd9660.c ffs.c makefs.c mtree.c walk.c)
set(ffs_SRCS ffs/ffs_alloc.c ffs/ffs_balloc.c ffs/ffs_bswap.c ffs/ffs_subr.c ffs/ufs_bmap.c ffs/buf.c ffs/mkfs.c)
set(cd9660_SRCS cd9660_strings.c cd9660_debug.c cd9660_eltorito.c cd9660_write.c cd9660_conversion.c iso9660_rrip.c cd9660_archimedes.c)
prepend_path(cd9660_SRCS cd9660)
list(APPEND makefs_SRCS
        ${CHERIBSD_DIR}/contrib/mtree/getid.c
        ${CHERIBSD_DIR}/contrib/mtree/misc.c
        ${CHERIBSD_DIR}/contrib/mtree/spec.c
)
list(APPEND makefs_SRCS ${CMAKE_SOURCE_DIR}/contrib/mknod/pack_dev.c)

list(APPEND makefs_SRCS ${cd9660_SRCS} ${ffs_SRCS})

list(APPEND makefs_SRCS ${CHERIBSD_DIR}/sys/ufs/ffs/ffs_tables.c)


# add the missing headers from sys to the include path
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/ufs" "ufs" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")

add_crossbuild_executable(makefs SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/makefs SOURCES ${makefs_SRCS})
target_include_directories(makefs PRIVATE ${CMAKE_SOURCE_DIR}/contrib/libnetbsd)
target_include_directories(makefs PRIVATE ${CHERIBSD_DIR}/contrib/mtree)
target_include_directories(makefs PRIVATE ${CHERIBSD_DIR}/sys/ufs/ffs)
target_include_directories(makefs PRIVATE ${CHERIBSD_DIR}/sys/fs/cd9660)
target_include_directories(makefs PRIVATE ${CHERIBSD_DIR}/contrib/mknod)
target_include_directories(makefs PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_link_libraries(makefs PRIVATE FreeBSD::libnetbsd FreeBSD::libsbuf util FreeBSD::libpwcache)
if(LINUX)
    target_link_libraries(makefs PRIVATE libc-vis)
endif()

