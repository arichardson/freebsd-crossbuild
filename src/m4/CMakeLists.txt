# Build /usr/sbin/config
set(SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/config)

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" MAIN_DEPENDENCY ${SOURCE_PATH}/config.y
        BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/y.tab.h"
        COMMAND FreeBSD::yacc -d "${SOURCE_PATH}/config.y" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/config.c" MAIN_DEPENDENCY "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
        COMMAND cp -f "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/config.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/lang.c" MAIN_DEPENDENCY ${SOURCE_PATH}/lang.l
        COMMAND $<TARGET_FILE:lex> -olang.c "${SOURCE_PATH}/lang.l" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
# need to set this as a single argument to make sure CMAKE escapes it properly
set(file2c_arg "char kernconfstr[] = {' ',0};")
add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/kernconf.c" MAIN_DEPENDENCY ${SOURCE_PATH}/kernconf.tmpl
        COMMAND FreeBSD::file2c 'char kernconfstr[] = {\' \',0}\;' < "${SOURCE_PATH}/kernconf.tmpl" > "${CMAKE_CURRENT_BINARY_DIR}/kernconf.c" WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

set(config_SRCS
main.c
mkmakefile.c
mkheaders.c
mkoptions.c
${CMAKE_CURRENT_BINARY_DIR}/config.c
${CMAKE_CURRENT_BINARY_DIR}/lang.c
${CMAKE_CURRENT_BINARY_DIR}/kernconf.c
)
add_crossbuild_executable(config SOURCE_PATH ${SOURCE_PATH} SOURCES ${config_SRCS})
target_link_libraries(config PRIVATE FreeBSD::libsbuf)

