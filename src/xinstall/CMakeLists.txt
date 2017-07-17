# cmake won't allow targets called install because that conflicts with the default isntall rule -> xinstall
add_crossbuild_executable(xinstall SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/xinstall SOURCES xinstall.c ${CHERIBSD_DIR}/contrib/mtree/getid.c)
set_target_properties(xinstall PROPERTIES OUTPUT_NAME freebsd-install)  # but install it as freebsd-install
target_link_libraries(xinstall PRIVATE FreeBSD::libmd FreeBSD::libpwcache FreeBSD::libnetbsd util)
target_include_directories(xinstall PRIVATE ${CHERIBSD_DIR}/contrib/mtree)
add_executable(FreeBSD::install ALIAS xinstall)
if(LINUX)
    target_compile_definitions(xinstall PRIVATE HAVE_STRUCT_STAT_ST_FLAGS=0 EFTYPE=EINVAL EPROCLIM=ENOMEM)
    target_link_libraries(xinstall PRIVATE libc-vis)
else()
    target_compile_definitions(xinstall PRIVATE -DHAVE_STRUCT_STAT_ST_FLAGS=1)
endif()
