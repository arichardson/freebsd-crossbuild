
add_crossbuild_executable(cap_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.bin/cap_mkdb SOURCES cap_mkdb.c)

add_crossbuild_executable(pwd_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/pwd_mkdb SOURCES pwd_mkdb.c ${CHERIBSD_DIR}/lib/libc/gen/pw_scan.c )
# TODO: add ${CHERIBSD_DIR}/lib/libc/gen/pw_scan.c
target_include_directories(pwd_mkdb PRIVATE ${CHERIBSD_DIR}/lib/libc/gen)
target_include_directories(pwd_mkdb PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/include)
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/include/pwd.h" "pwd.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/include/grp.h" "grp.h" WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/include")
target_include_directories(pwd_mkdb PRIVATE BEFORE ${CMAKE_CURRENT_BINARY_DIR}/include)
if(APPLE)
    target_compile_definitions(pwd_mkdb PRIVATE __XSI_VISIBLE=800 __POSIX_VISIBLE=200112 __BSD_VISIBLE=1)
endif()

add_crossbuild_executable(services_mkdb SOURCE_PATH ${CHERIBSD_DIR}/usr.sbin/services_mkdb SOURCES services_mkdb.c uniq.c)
target_compile_definitions(services_mkdb PRIVATE _PATH_SERVICES_DB="/var/db/services.db")
