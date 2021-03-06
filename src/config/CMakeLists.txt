
# Build /usr/sbin/config
set(SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/config)

# need to set this as a single argument to make sure CMAKE escapes it properly
set(file2c_arg "char kernconfstr[] = {' ',0};")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kernconf.c" MAIN_DEPENDENCY ${SOURCE_PATH}/kernconf.tmpl
        COMMAND FreeBSD::file2c 'char kernconfstr[] = {\' \',0}\;' < "${SOURCE_PATH}/kernconf.tmpl" > "${CMAKE_CURRENT_BINARY_DIR}/kernconf.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(config_SRCS
    main.c
    mkmakefile.c
    mkheaders.c
    mkoptions.c
    ${CMAKE_CURRENT_BINARY_DIR}/kernconf.c
)
add_yacc_source(config_SRCS ${SOURCE_PATH}/config.y BASENAME y.tab)
add_lex_source(config_SRCS ${SOURCE_PATH}/lang.l)

add_crossbuild_executable(config SOURCE_PATH ${SOURCE_PATH} SOURCES ${config_SRCS})
target_link_libraries(config PRIVATE FreeBSD::libsbuf)

