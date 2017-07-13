# Use pkg-config to get the directories and then use these values
# in the FIND_PATH() and FIND_LIBRARY() calls
find_package(PkgConfig REQUIRED)
pkg_check_modules(PKG_BSD QUIET bsd-overlay)

set(LibBSD_DEFINITIONS ${PKG_BSD_CFLAGS_OTHER})

find_path(LibBSD_INCLUDE_DIR
        NAMES bsd.h
        PATH_SUFFIXES bsd
        HINTS ${PKG_BSD_INCLUDE_DIRS}
        )
find_library(LibBSD_LIBRARY
        NAMES bsd
        HINTS ${PKG_BSD_LIBRARY_DIRS}
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibBSD
        FOUND_VAR
        LibBSD_FOUND
        REQUIRED_VARS
        LibBSD_LIBRARY
        LibBSD_INCLUDE_DIR
        VERSION_VAR
        LibBSD_VERSION
        )

if(LibBSD_FOUND AND NOT TARGET Bootstrap::LibBSD)
    add_library(Bootstrap::LibBSD UNKNOWN IMPORTED)
    set_target_properties(Bootstrap::LibBSD PROPERTIES
            IMPORTED_LOCATION "${LibBSD_LIBRARY}"
            INTERFACE_COMPILE_OPTIONS "${LibBSD_DEFINITIONS}"
            INTERFACE_INCLUDE_DIRECTORIES "${LibBSD_INCLUDE_DIR}"
            )
endif()

mark_as_advanced(LibBSD_LIBRARY LibBSD_INCLUDE_DIR)

set(LibBSD_LIBRARIES ${LibBSD_LIBRARY})
set(LibBSD_INCLUDE_DIRS ${LibBSD_INCLUDE_DIR})

include(FeatureSummary)
set_package_properties(LibBSD PROPERTIES
        URL "https://libbsd.freedesktop.org"
        DESCRIPTION "This library provides useful functions commonly found on BSD systems, and lacking on others like GNU systems"
        )
