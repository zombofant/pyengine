
find_path(CAIRO_INCLUDE_DIRS cairo
    PATH_SUFFIXES include
    PATHS
        /usr
        /usr/local
)

find_library(CAIRO_LIBRARY
    NAMES cairo
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)

set(Cairo_FOUND "NO")
if (CAIRO_INCLUDE_DIRS AND CAIRO_LIBRARY)
    set(Cairo_FOUND "YES")
endif (CAIRO_INCLUDE_DIRS AND CAIRO_LIBRARY)
