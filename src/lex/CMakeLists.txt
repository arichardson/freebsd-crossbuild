set(SOURCE_PATH ${CHERIBSD_DIR}/contrib/flex)
set(lex_SRCS
    buf.c ccl.c dfa.c ecs.c filter.c gen.c main.c misc.c
    nfa.c options.c regex.c scanflags.c
    scanopt.c sym.c tables.c tables_shared.c
    tblcmp.c yylex.c
    ${CMAKE_CURRENT_BINARY_DIR}/scan.c
    ${CMAKE_CURRENT_BINARY_DIR}/skel.c
    ${CMAKE_CURRENT_BINARY_DIR}/parse.c
)


add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/parse.c" "${CMAKE_CURRENT_BINARY_DIR}/parse.h" MAIN_DEPENDENCY ${SOURCE_PATH}/parse.y
        COMMAND Bootstrap::yacc -d -o parse.c ${SOURCE_PATH}/parse.y
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

# Create scan.c with the host system's lex
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/scan.c" MAIN_DEPENDENCY ${SOURCE_PATH}/scan.l
        COMMAND /usr/bin/lex -o ${CMAKE_CURRENT_BINARY_DIR}/scan.c ${SOURCE_PATH}/scan.l
        WORKING_DIRECTORY ${SOURCE_PATH})

if(APPLE)
    # Mac m4 binary doesn't understand stuff
    set(M4_BINARY /usr/local/opt/m4/bin/m4)
    if(NOT EXISTS ${M4_BINARY})
        message(FATAL_ERROR "The apple m4 won't work, install m4 using `brew install m4`")
    endif()
else()
    set(M4_BINARY m4)
endif()
check_include_file(alloca.h HAVE_ALLOCA_H)
check_include_file(malloc.h HAVE_MALLOC_H)
check_include_file(memory.h HAVE_MEMORY_H)
check_include_file(netinet/in.h HAVE_NETINET_IN_H)
check_include_file(vfork.h HAVE_VFORK_H)
check_symbol_exists(__func__ "" HAVE_DECL___FUNC__)
configure_file(config.h.cmake ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/skel.c" MAIN_DEPENDENCY ${SOURCE_PATH}/flex.skl
        COMMAND $<TARGET_FILE:sed> "s/m4_/m4postproc_/g; s/m4preproc_/m4_/g" ${SOURCE_PATH}/flex.skl | ${M4_BINARY} -I${SOURCE_PATH} -P -DFLEX_MAJOR_VERSION=2 -DFLEX_MINOR_VERSION=5 -DFLEX_SUBMINOR_VERSION=37 | $<TARGET_FILE:sed> "s/m4postproc_/m4_/g" | sh ${SOURCE_PATH}/mkskel.sh > ${CMAKE_CURRENT_BINARY_DIR}/skel.c
        VERBATIM WORKING_DIRECTORY ${SOURCE_PATH})



# add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/config.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
#         COMMAND cp -f "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/config.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_crossbuild_executable(lex SOURCE_PATH ${SOURCE_PATH} SOURCES ${lex_SRCS})
target_include_directories(lex PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${SOURCE_PATH})
target_compile_definitions(lex PRIVATE -DHAVE_CONFIG_H)
