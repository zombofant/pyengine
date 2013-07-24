find_package(PkgConfig)
pkg_check_modules(_SIGC++ sigc++-2.0)
find_path(SIGC++_INCLUDE_DIR
    NAMES
        sigc++/sigc++.h
    PATHS
        ${_SIGC++_INCLUDEDIR}
        /usr/include
        /usr/local/include
    PATH_SUFFIXES
        sigc++-2.0
)
find_path(SIGC++_CONFIG_INCLUDE_DIR
    NAMES
        sigc++config.h
    PATHS
        ${_SIGC++_INCLUDEDIR}
        ${_SIGC++_LIBDIR}
        /usr/include
        /usr/local/include
        /usr/lib
        /usr/lib64
        /usr/local/lib
    PATH_SUFFIXES
        sigc++-2.0/include
)
find_library(SIGC++_LIBRARY
    NAMES
        sigc-2.0
    PATHS
        ${_SIGC++_LIBDIR}
        /usr/lib
        /usr/lib64
        /usr/local/lib
        /usr/local/lib64
)

set(SIGC++_FOUND "NO")
if (SIGC++_LIBRARY AND SIGC++_INCLUDE_DIR)
    set(SIGC++_FOUND "YES")
endif (SIGC++_LIBRARY AND SIGC++_INCLUDE_DIR)
