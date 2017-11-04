set(GETUSERSHELL_SRCS
    ${CHERIBSD_DIR}/lib/libc/gen/getusershell.c
    ${CHERIBSD_DIR}/lib/libc/net/nsdispatch.c
)
add_yacc_source(GETUSERSHELL_SRCS ${CHERIBSD_DIR}/lib/libc/net/nsparser.y YFLAGS "-p_nsyy")
add_lex_source(GETUSERSHELL_SRCS ${CHERIBSD_DIR}/lib/libc/net/nslexer.l LFLAGS "-P_nsyy")

add_crossbuild_library(getusershell STATIC ${GETUSERSHELL_SRCS})
target_include_directories(getusershell PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/lib/libc/include)
symlink_freebsd_headers(include nsswitch.h)
symlink_freebsd_headers(lib/libc/include nss_tls.h)
target_link_libraries(getusershell PUBLIC Threads::Threads dl)
symlink_freebsd_headers(include nsswitch.h)


### cap_mkdb ###

add_crossbuild_executable(cap_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/cap_mkdb SOURCES cap_mkdb.c ${CHERIBSD_DIR}/lib/libc/gen/getcap.c)
target_link_libraries(cap_mkdb PRIVATE FreeBSD::libdb getusershell)
if(LINUX)
    target_link_libraries(cap_mkdb PRIVATE Heimdal::LibRoken)
endif()

### pwd_mkdb ###

add_crossbuild_executable(pwd_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/pwd_mkdb SOURCES pwd_mkdb.c ${CHERIBSD_DIR}/lib/libc/gen/pw_scan.c )
# TODO: add ${CHERIBSD_DIR}/lib/libc/gen/pw_scan.c
target_include_directories(pwd_mkdb PRIVATE ${CHERIBSD_DIR}/lib/libc/gen)
target_include_directories(pwd_mkdb PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/include/pwd.h" "pwd.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/include/grp.h" "grp.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(pwd_mkdb PRIVATE BEFORE ${CMAKE_CURRENT_BINARY_DIR}/include)
target_compile_definitions(pwd_mkdb PRIVATE __XSI_VISIBLE=800 __POSIX_VISIBLE=200112 __BSD_VISIBLE=1)
if(APPLE)
    target_link_libraries(pwd_mkdb PRIVATE FreeBSD::libreallocarray fake-_DYNAMIC)
endif()
target_link_libraries(pwd_mkdb PRIVATE FreeBSD::libdb getusershell)


### services_mkdb ###

add_crossbuild_executable(services_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/services_mkdb SOURCES services_mkdb.c uniq.c)
target_compile_definitions(services_mkdb PRIVATE _PATH_SERVICES_DB="/var/db/services.db")
target_link_libraries(services_mkdb PRIVATE FreeBSD::libdb getusershell)
