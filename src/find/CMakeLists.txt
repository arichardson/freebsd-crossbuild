set(find_SRCS find.c function.c ls.c main.c misc.c operator.c option.c)

add_yacc_source(find_SRCS ${CHERIBSD_DIR}/usr.bin/find/getdate.y BASENAME y.tab)

if(APPLE)
    list(APPEND find_SRCS ${CHERIBSD_DIR}/lib/libc/locale/rpmatch.c)
endif()

# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_crossbuild_executable(find SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/find SOURCES ${find_SRCS})
target_link_libraries(find PRIVATE FreeBSD::libpwcache)

