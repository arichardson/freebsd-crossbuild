
set(awk_SRCS b.c lex.c lib.c main.c parse.c ${CMAKE_CURRENT_BINARY_DIR}/ytab.c ${CMAKE_CURRENT_BINARY_DIR}/proctab.c run.c tran.c)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" MAIN_DEPENDENCY ${CHERIBSD_DIR}/contrib/one-true-awk/awkgram.y
        COMMAND Bootstrap::yacc -d ${CHERIBSD_DIR}/contrib/one-true-awk/awkgram.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
        COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/ytab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
        COMMAND ln -sf "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h" "${CMAKE_CURRENT_BINARY_DIR}/ytab.h" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

list(APPEND awk_SRCS ${CMAKE_CURRENT_BINARY_DIR}/ytab.c)
add_crossbuild_executable(awk SOURCE_PATH ${CHERIBSD_DIR}/contrib/one-true-awk SOURCES ${awk_SRCS})
add_definitions(-DHAS_ISBLANK -DFOPEN_MAX=64)


# to generate proctab.c we first need to build maketab and run that
add_executable(awk-maketab ${CHERIBSD_DIR}/contrib/one-true-awk/maketab.c ${CMAKE_CURRENT_BINARY_DIR}/ytab.h)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/proctab.c" # MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
        COMMAND awk-maketab > "${CMAKE_CURRENT_BINARY_DIR}/proctab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
