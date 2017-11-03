
function(add_yacc_source _srcs _yfile)
    cmake_parse_arguments(_ays "" "BASENAME" "YFLAGS" ${ARGN})
    if(_ays_BASENAME)
        set(_basename ${_ays_BASENAME})
    else()
        get_filename_component(_basename ${_yfile} NAME_WE)
    endif()
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c" "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h" MAIN_DEPENDENCY ${_yfile}
            COMMAND Bootstrap::yacc -d -o ${_basename}.c ${_ays_YFLAGS} ${_yfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(_new_srcs ${${_srcs}}
            ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c
            ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.h)
    set(${_srcs} ${_new_srcs} PARENT_SCOPE)
endfunction()


function(add_lex_source _srcs _lfile) # _flags
    cmake_parse_arguments(_als "" "BASENAME" "LFLAGS" ${ARGN})
    if(_als_BASENAME)
        set(_basename ${_als_BASENAME})
    else()
        get_filename_component(_basename ${_lfile} NAME_WE)
    endif()
    add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c" MAIN_DEPENDENCY ${_lfile}
            COMMAND Bootstrap::lex -o ${_basename}.c ${_als_LFLAGS} ${_lfile}
            WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
    set(_new_srcs ${${_srcs}} ${CMAKE_CURRENT_BINARY_DIR}/${_basename}.c)
    set(${_srcs} ${_new_srcs} PARENT_SCOPE)
endfunction()


# create a link from headers in ${CHERIBSD_DIR}/${_dir} to the current build directory (with the same directory structure)
function(symlink_freebsd_headers _dir) # _files
    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${_dir})
    foreach(_hdr ${ARGN})
        message(STATUS "Linking \${FreeBSD SRC}/${_dir}/${_hdr} to ${CMAKE_CURRENT_BINARY_DIR}/${_dir}")
        execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink "${CHERIBSD_DIR}/${_dir}/${_hdr}" "${_hdr}"
                        WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/${_dir}")
    endforeach()
endfunction()

function(add_freebsd_deps _target)
    if("${CMAKE_SYSTEM_NAME}" MATCHES "Linux")
        # link to libbsd
        target_link_libraries(${_target} PRIVATE Bootstrap::LibBSD)
        set(_os_dir linux)
        target_compile_definitions(${_target} PRIVATE LIBBSD_OVERLAY=1 _GNU_SOURCE=1)
        # target_include_directories(${_target} PRIVATE SYSTEM /usr/include/freebsd)
        # TODO: _GNU_SOURCE=1 _ISOC11_SOURCE=1 _XOPEN_SOURCE=800 ?
    elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(_os_dir mac)
        # needed to get access to some APIs (e.g. pwcache)
        set(_min_mac_version -mmacosx-version-min=10.12)
        target_compile_definitions(${_target} PRIVATE -D_DARWIN_C_SOURCE=1)
        target_compile_options(${_target} PRIVATE ${_min_mac_version})
        target_link_libraries(${_target} PRIVATE -Wl,-macosx_version_min,10.12)
    endif()
    target_compile_definitions(${_target} PRIVATE HAVE_NBTOOL_CONFIG_H=1)
    target_compile_options(${_target} PRIVATE -include ${CMAKE_SOURCE_DIR}/include/${_os_dir}/pre-include.h -Wno-pointer-sign)
    target_include_directories(${_target} SYSTEM BEFORE PRIVATE ${CMAKE_SOURCE_DIR}/include/${_os_dir} ${CMAKE_SOURCE_DIR}/include/common)
endfunction()

# Usage: add_crossbuild_executable(target_name SOURCE_PATH ${CHERIBSD_ROOT}/contrib/foo SOURCES ${SOUCES} ...)
function(_add_freebsd_executable _target) #  _srcs
    # add the crossbuilt version
    # only build the bootstrap libraries if BUILDING_BOOTSTRAP is set
    string(TOUPPER "${_target}" _target_upper_tmp)
    string(REPLACE "-" "_" _target_upper "${_target_upper_tmp}")
    cmake_parse_arguments(ACE "" "SOURCE_PATH" "SOURCES;LINK_LIBRARIES" ${ARGN})
    if(NOT ACE_SOURCES)
        message(FATAL_ERROR "add_crossbuild_executable() needs at least a SOURCES parameter")
    endif()
    set(_real_srcs "")
    foreach(_src ${ACE_SOURCES})
        if(ACE_SOURCE_PATH AND NOT IS_ABSOLUTE "${_src}")
            if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
                message(STATUS "${_src} exists")
                set(_src "${CMAKE_CURRENT_SOURCE_DIR}/${_src}")
            else()
                set(_src "${ACE_SOURCE_PATH}/${_src}")
            endif()
        endif()
        list(APPEND _real_srcs ${_src})
    endforeach()
    # message(STATUS "${_real_srcs}")
    add_executable(${_target} ${_real_srcs})
    add_executable(FreeBSD::${_target} ALIAS ${_target})
    set_target_properties(${_target} PROPERTIES OUTPUT_NAME freebsd-${_target})
    if(ACE_SOURCE_PATH)
        target_include_directories(${_target} PRIVATE ${ACE_SOURCE_PATH})
    endif()
    add_freebsd_deps(${_target})
    if(ACE_LINK_LIBRARIES)
        target_link_libraries(${_target} PRIVATE ${ACE_LINK_LIBRARIES})
    endif()
endfunction()

function(add_crossbuild_executable _target) #  _srcs
    _add_freebsd_executable(${_target} ${ARGN})
    install(TARGETS ${_target} RUNTIME DESTINATION bin)
endfunction()

# The same as add_crossbuild_executable but doesn't install the binary
function(add_bootstrap_executable _target) #  _srcs
    _add_freebsd_executable(${_target} ${ARGN})
endfunction()

function(add_crossbuild_library _target)
    add_library(${_target} ${ARGN})
    add_library(FreeBSD::lib${_target} ALIAS ${_target})
    add_freebsd_deps(${_target})
endfunction()
