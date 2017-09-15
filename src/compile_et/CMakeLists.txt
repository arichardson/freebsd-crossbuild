################
# compile_et
################
# run yacc and lex
set(compile_et_SRCS compile_et.c heimdal_vers.c)
add_yacc_source(compile_et_SRCS ${CHERIBSD_DIR}/contrib/com_err/parse.y)
add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/lex.c MAIN_DEPENDENCY ${CHERIBSD_DIR}/contrib/com_err/lex.l
        COMMAND $<TARGET_FILE:lex> -o lex.c ${CHERIBSD_DIR}/contrib/com_err/lex.l
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND compile_et_SRCS ${CMAKE_CURRENT_BINARY_DIR}/lex.c)
add_crossbuild_executable(compile_et SOURCE_PATH ${CHERIBSD_DIR}/contrib/com_err SOURCES ${compile_et_SRCS})
target_link_libraries(compile_et PRIVATE Heimdal::LibRoken)
