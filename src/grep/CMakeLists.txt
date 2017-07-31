set(grep_SRCS file.c grep.c queue.c util.c)

add_crossbuild_executable(grep SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/grep SOURCES ${grep_SRCS})
# TODO: regex/   fastmatch.c hashtable.c tre-compile.c tre-fastmatch.c
target_compile_definitions(grep PRIVATE -DWITHOUT_FASTMATCH)
# TODO: if needed could add those
target_compile_definitions(grep PRIVATE -DWITHOUT_LZMA -DWITHOUT_BZIP2)
# should not be needed
target_compile_definitions(grep PRIVATE -DWITHOUT_NLS)
target_link_libraries(grep PRIVATE z)

# TODO: these should rather be symlink
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-egrep")
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-fgrep")
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-rgrep")
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-zgrep")
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-zegrep")
install(PROGRAMS $<TARGET_FILE:grep> DESTINATION bin RENAME "freebsd-zfgrep")
