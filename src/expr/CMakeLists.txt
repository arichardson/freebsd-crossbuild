set(expr_SRCS ${CMAKE_CURRENT_BINARY_DIR}/y.tab.c ${CHERIBSD_DIR}/lib/libc/gen/check_utility_compat.c)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" MAIN_DEPENDENCY ${CHERIBSD_DIR}/bin/expr/expr.y
        COMMAND Bootstrap::yacc -d ${CHERIBSD_DIR}/bin/expr/expr.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
#         COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# list(APPEND awk_SRCS ${CMAKE_CURRENT_BINARY_DIR}/ytab.c)

add_crossbuild_executable(expr SOURCES ${expr_SRCS})
target_compile_options(expr PRIVATE -fwrapv)
