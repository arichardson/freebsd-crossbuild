set(sort_SRCS bwstring.c coll.c file.c mem.c radixsort.c sort.c vsort.c)

add_crossbuild_executable(sort SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/sort SOURCES ${sort_SRCS})
target_compile_definitions(sort PRIVATE -DSORT_THREADS -DWITHOUT_NLS)
target_link_libraries(sort PRIVATE FreeBSD::libmd Threads::Threads)
