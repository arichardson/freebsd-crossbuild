set(expr_SRCS ${CHERIBSD_DIR}/lib/libc/gen/check_utility_compat.c)

add_yacc_source(expr_SRCS ${CHERIBSD_DIR}/bin/expr/expr.y BASENAME y.tab)
add_crossbuild_executable(expr SOURCES ${expr_SRCS})
target_compile_options(expr PRIVATE -fwrapv)
