set(yacc_SRCS closure.c error.c graph.c lalr.c lr0.c main.c mkpar.c mstring.c output.c reader.c yaccpar.c symtab.c verbose.c warshall.c main.c)

file(READ "${CHERIBSD_DIR}/contrib/byacc/VERSION" _yyversion)
add_definitions(-DHAVE_FCNTL_H=1 -DHAVE_MKSTEMP=1 -DMAXTABLE=INT_MAX -DMIXEDCASE_FILENAMES=1 -DYYPATCH=${_yyversion})

add_crossbuild_executable(yacc SOURCE_PATH ${CHERIBSD_DIR}/contrib/byacc SOURCES ${yacc_SRCS})
add_executable(Bootstrap::yacc ALIAS yacc)
