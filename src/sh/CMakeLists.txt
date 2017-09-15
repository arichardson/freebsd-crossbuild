# GENSRCS= builtins.c nodes.c syntax.c
# GENHDRS= builtins.h nodes.h syntax.h token.h
# SRCS= ${SHSRCS} ${GENSRCS} ${GENHDRS}
set(SRC_ROOT ${CHERIBSD_DIR}/bin/sh)
set(sh_SRCS alias.c arith_yacc.c arith_yylex.c cd.c bltin/echo.c error.c eval.c
    exec.c expand.c
    histedit.c input.c jobs.c ${CHERIBSD_DIR}/bin/kill/kill.c mail.c main.c memalloc.c miscbltin.c
    mystring.c options.c output.c parser.c ${CHERIBSD_DIR}/usr.bin/printf/printf.c redir.c show.c
    ${CHERIBSD_DIR}/bin/test/test.c trap.c var.c)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/builtins.c" "${CMAKE_CURRENT_BINARY_DIR}/builtins.h" DEPENDS mktemp awk sed
    MAIN_DEPENDENCY "${SRC_ROOT}/builtins.def" DEPENDENCIES "${SRC_ROOT}/mkbuiltins"
    COMMAND env PATH=${CMAKE_CURRENT_BINARY_DIR}:/bin:/usr/bin /bin/sh "${SRC_ROOT}/mkbuiltins" "${SRC_ROOT}" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND sh_SRCS ${CMAKE_CURRENT_BINARY_DIR}/builtins.c)

file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/awk INPUT ${CMAKE_CURRENT_SOURCE_DIR}/awk.in)
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/sed INPUT ${CMAKE_CURRENT_SOURCE_DIR}/sed.in)
file(GENERATE OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/mktemp INPUT ${CMAKE_CURRENT_SOURCE_DIR}/mktemp.in)

# to generate proctab.c we first need to build maketab and run that
add_bootstrap_executable(sh-mknodes SOURCES ${SRC_ROOT}/mknodes.c)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/nodes.c" "${CMAKE_CURRENT_BINARY_DIR}/nodes.h"  MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/freebsd-sh-mknodes"
        DEPENDENCIES "${SRC_ROOT}/nodetypes" "${SRC_ROOT}/nodes.c.pat"
        COMMAND sh-mknodes "${SRC_ROOT}/nodetypes" "${SRC_ROOT}/nodes.c.pat" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND sh_SRCS ${CMAKE_CURRENT_BINARY_DIR}/nodes.c)

add_bootstrap_executable(sh-syntax SOURCES ${SRC_ROOT}/mksyntax.c)
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/syntax.c" "${CMAKE_CURRENT_BINARY_DIR}/syntax.h"
        COMMAND sh-syntax WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND sh_SRCS ${CMAKE_CURRENT_BINARY_DIR}/syntax.c)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/token.h" MAIN_DEPENDENCY "${SRC_ROOT}/mktokens" DEPENDS mktemp awk sed
        COMMAND env PATH=${CMAKE_CURRENT_BINARY_DIR}:/bin:/usr/bin /bin/sh "${SRC_ROOT}/mktokens" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
list(APPEND sh_SRCS ${CMAKE_CURRENT_BINARY_DIR}/token.h)

add_crossbuild_executable(sh SOURCE_PATH ${SRC_ROOT} SOURCES ${sh_SRCS})
target_compile_definitions(sh PRIVATE -DSHELL)
target_link_libraries(sh PRIVATE edit)
target_compile_options(sh PRIVATE -Wno-macro-redefined)
add_executable(FreeBSD::sh ALIAS sh)

# # LIBADD=	edit
#
# # .PATH:	${.CURDIR}/bltin \
# # 	${.CURDIR:H}/kill \
# # 	${.CURDIR:H}/test \
# # 	${SRCTOP}/usr.bin/printf
#
# CLEANFILES+= mknodes mknodes.o \
# 	mksyntax mksyntax.o
# CLEANFILES+= ${GENSRCS} ${GENHDRS}
#
# build-tools: mknodes mksyntax
#
# .ORDER: builtins.c builtins.h
# builtins.h: .NOMETA
# builtins.c builtins.h: mkbuiltins builtins.def
# 	sh ${.CURDIR}/mkbuiltins ${.CURDIR}
#
# # XXX this is just to stop the default .c rule being used, so that the
# # intermediate object has a fixed name.
# # XXX we have a default .c rule, but no default .o rule.
# mknodes.o mksyntax.o: ${BUILD_TOOLS_META}
# 	${CC} ${CFLAGS} ${LDFLAGS} ${.IMPSRC} ${LDLIBS} -o ${.TARGET}
# mknodes: mknodes.o ${BUILD_TOOLS_META}
# mksyntax: mksyntax.o ${BUILD_TOOLS_META}
#
# .ORDER: nodes.c nodes.h
# nodes.h: .NOMETA
# nodes.c nodes.h: mknodes nodetypes nodes.c.pat
# 	${BTOOLSPATH:U.}/mknodes ${.CURDIR}/nodetypes ${.CURDIR}/nodes.c.pat
#
# .ORDER: syntax.c syntax.h
# syntax.h: .NOMETA
# syntax.c syntax.h: mksyntax
# 	${BTOOLSPATH:U.}/mksyntax
#
# token.h: mktokens
# 	sh ${.CURDIR}/mktokens
#
# .if ${MK_TESTS} != "no"
# SUBDIR+=    tests
# .endif
#
# WANT_DUMP=	yes
#
# .include <bsd.prog.mk>
