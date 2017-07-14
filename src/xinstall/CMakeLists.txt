# cmake won't allow targets called install because that conflicts with the default isntall rule -> xinstall
add_crossbuild_executable(xinstall SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/xinstall SOURCES xinstall.c ${CHERIBSD_DIR}/contrib/mtree/getid.c)
set_target_properties(xinstall PROPERTIES OUTPUT_NAME freebsd-install)  # but install it as freebsd-install
target_link_libraries(xinstall PRIVATE FreeBSD::libmd FreeBSD::libpwcache util)
target_include_directories(xinstall PRIVATE ${CHERIBSD_DIR}/contrib/mtree)
add_executable(FreeBSD::install ALIAS xinstall)