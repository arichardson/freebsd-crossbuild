
set(sed_SRCS compile.c main.c misc.c process.c)
add_crossbuild_executable(sed SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/sed SOURCES ${sed_SRCS})
add_executable(Bootstrap::sed ALIAS sed)
add_executable(FreeBSD::sed ALIAS sed)
