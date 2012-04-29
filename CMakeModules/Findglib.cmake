
find_path(glib_INCLUDE_DIRS glib.h
    PATH_SUFFIXES include/glib-2.0 include
    PATHS
        /usr
        /usr/local
)

find_path(glib_INCLUDE_DIRS2 glibconfig.h
    PATH_SUFFIXES lib64/glib-2.0/include lib/glib-2.0/include
    PATHS
        /usr
        /usr/local
)

find_library(glib_LIBRARY
    NAMES g
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)

set(glib_FOUND "NO")
if (glib_INCLUDE_DIRS AND glib_LIBRARY)
    set(glib_FOUND "YES")
    set(glib_INCLUDE_DIRS ${glib_INCLUDE_DIRS} ${glib_INCLUDE_DIRS2})
endif (glib_INCLUDE_DIRS AND glib_LIBRARY)
