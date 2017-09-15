
set(awk_SRCS b.c lex.c lib.c main.c parse.c ${CMAKE_CURRENT_BINARY_DIR}/ytab.c ${CMAKE_CURRENT_BINARY_DIR}/proctab.c run.c tran.c)
add_yacc_source(awk_SRCS ${CHERIBSD_DIR}/contrib/one-true-awk/awkgram.y BASENAME ytab)

add_definitions(-DHAS_ISBLANK -DFOPEN_MAX=64)


# to generate proctab.c we first need to build maketab and run that
add_executable(awk-maketab ${CHERIBSD_DIR}/contrib/one-true-awk/maketab.c ${CMAKE_CURRENT_BINARY_DIR}/ytab.h)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/proctab.c" # MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
        COMMAND awk-maketab > "${CMAKE_CURRENT_BINARY_DIR}/proctab.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_crossbuild_executable(awk SOURCE_PATH ${CHERIBSD_DIR}/contrib/one-true-awk SOURCES ${awk_SRCS})
target_link_libraries(awk PRIVATE m)
add_executable(FreeBSD::awk ALIAS awk)
