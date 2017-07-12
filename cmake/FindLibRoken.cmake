find_path(LIBROKEN_INCLUDE_DIR NAMES getarg.h
        HINTS /usr/local/opt/heimdal/include)
find_library(LIBROKEN_LIBRARY NAMES roken
        HINTS /usr/local/opt/heimdal/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(LibRoken
        FOUND_VAR LIBROKEN_FOUND
        REQUIRED_VARS LIBROKEN_INCLUDE_DIR LIBROKEN_LIBRARY
        # VERSION_VAR LIBGIT2_VERSION
        )

if(LIBROKEN_FOUND AND NOT TARGET Heimdal::LibRoken)
    add_library(Heimdal::LibRoken UNKNOWN IMPORTED)
    set_target_properties(Heimdal::LibRoken PROPERTIES
            IMPORTED_LOCATION "${LIBROKEN_LIBRARY}"
            # INTERFACE_COMPILE_OPTIONS "${LIBROKEN_DEFINITIONS}"
            INTERFACE_INCLUDE_DIRECTORIES "${LIBROKEN_INCLUDE_DIR}"
            )
endif()

mark_as_advanced(LIBROKEN_INCLUDE_DIR LIBROKEN_LIBRARY)

include(FeatureSummary)
set_package_properties(LibRoken PROPERTIES
        URL "https://github.com/heimdal/heimdal"
        )