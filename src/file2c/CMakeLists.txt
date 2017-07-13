add_crossbuild_executable(file2c SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/file2c SOURCES file2c.c)
add_executable(FreeBSD::file2c ALIAS file2c)