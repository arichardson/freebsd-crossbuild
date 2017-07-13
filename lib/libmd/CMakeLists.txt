############################################
# libmd (for mtree and install)
###########################################
set(libmd_SRCS md4c.c md5c.c
    rmd160c.c
    sha0c.c sha1c.c
)
prepend_path(libmd_SRCS ${CHERIBSD_DIR}/lib/libmd/)
list(APPEND libmd_SRCS ${CHERIBSD_DIR}/sys/crypto/sha2/sha256c.c ${CHERIBSD_DIR}/sys/crypto/sha2/sha512c.c)
list(APPEND libmd_SRCS ${CHERIBSD_DIR}/sys/crypto/skein/skein.c ${CHERIBSD_DIR}/sys/crypto/skein/skein_block.c)
# TODO: generate these the same way cheribsd does
set(libmd_pregenerated_SRCS md4hl.c md5hl.c rmd160hl.c sha0hl.c sha1hl.c sha256hl.c sha384hl.c sha512hl.c sha512thl.c skein256hl.c skein512hl.c skein1024hl.c)

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include/sys)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/sys/md5.h" "sys/md5.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/sys/md4.h" "sys/md4.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
#execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/crypto/sha2/sha256.h" "sha256.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
#execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/crypto/sha2/sha384.h" "sha384.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
#execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/sys/crypto/sha2/sha512.h" "sha512.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
add_crossbuild_library(md STATIC ${libmd_SRCS} ${libmd_pregenerated_SRCS})
target_include_directories(md PUBLIC ${CHERIBSD_DIR}/lib/libmd/ ${CMAKE_CURRENT_BINARY_DIR}/include)
target_include_directories(md PUBLIC ${CHERIBSD_DIR}/sys/crypto/sha2 ${CHERIBSD_DIR}/sys/crypto/skein)