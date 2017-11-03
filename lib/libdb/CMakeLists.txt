############################################
# libmd (for mtree and install)
###########################################
set(libdb_SRCS
    btree/bt_close.c
    btree/bt_conv.c
    btree/bt_debug.c
    btree/bt_delete.c
    btree/bt_get.c
    btree/bt_open.c
    btree/bt_overflow.c
    btree/bt_page.c
    btree/bt_put.c
    btree/bt_search.c
    btree/bt_seq.c
    btree/bt_split.c
    btree/bt_utils.c

    hash/hash_bigkey.c
    hash/hash_buf.c
    hash/hash.c
    hash/hash_func.c
    hash/hash_log2.c
    hash/hash_page.c
    hash/ndbm.c

    mpool/mpool.c

    recno/rec_close.c
    recno/rec_delete.c
    recno/rec_get.c
    recno/rec_open.c
    recno/rec_put.c
    recno/rec_search.c
    recno/rec_seq.c
    recno/rec_utils.c

    db/db.c
)

set(DB_DIR )
prepend_path(libdb_SRCS ${CHERIBSD_DIR}/lib/libc/db)

symlink_freebsd_headers(include db.h mpool.h ndbm.h)

add_crossbuild_library(db STATIC ${libdb_SRCS})
target_include_directories(db PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include)
# target_include_directories(db PRIVATE ${CHERIBSD_DIR}/lib/libc/include)
target_compile_definitions(db PUBLIC __BSD_VISIBLE=1)
target_compile_definitions(db PRIVATE -D__DBINTERFACE_PRIVATE)
