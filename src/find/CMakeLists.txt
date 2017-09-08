set(find_SRCS find.c function.c ls.c main.c misc.c operator.c option.c ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" MAIN_DEPENDENCY ${CHERIBSD_DIR}/usr.bin/find/getdate.y
        COMMAND Bootstrap::yacc -d ${CHERIBSD_DIR}/usr.bin/find/getdate.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_crossbuild_executable(find SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/find SOURCES ${find_SRCS})
target_link_libraries(find PRIVATE FreeBSD::libpwcache)

