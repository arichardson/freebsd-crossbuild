################
# compile_et
################
find_package(LibRoken REQUIRED)
# run yacc and lex
set(compile_et_SRCS compile_et.c heimdal_vers.c)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/parse.c MAIN_DEPENDENCY ${CHERIBSD_DIR}/contrib/com_err/parse.y
        BYPRODUCTS ${CMAKE_CURRENT_BINARY_DIR}/parse.h
        COMMAND Bootstrap::yacc -d -o parse.c ${CHERIBSD_DIR}/contrib/com_err/parse.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lex.c MAIN_DEPENDENCY ${CHERIBSD_DIR}/contrib/com_err/lex.l
        COMMAND lex -o lex.c ${CHERIBSD_DIR}/contrib/com_err/lex.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND compile_et_SRCS ${CMAKE_CURRENT_BINARY_DIR}/parse.c ${CMAKE_CURRENT_BINARY_DIR}/lex.c)
add_crossbuild_executable(compile_et SOURCE_PATH ${CHERIBSD_DIR}/contrib/com_err SOURCES ${compile_et_SRCS})
target_link_libraries(compile_et PRIVATE Heimdal::LibRoken)