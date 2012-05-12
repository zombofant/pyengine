
find_path(Cairo_INCLUDE_DIRS cairo/cairo.h
    PATH_SUFFIXES include
    PATHS
        /usr
        /usr/local
)

find_library(Cairo_LIBRARY
    NAMES cairo
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)

set(Cairo_FOUND "NO")
if (Cairo_INCLUDE_DIRS AND Cairo_LIBRARY)
    set(Cairo_FOUND "YES")
    # XXX: this is kinda neccessary for pycairo -.-
    set(Cairo_INCLUDE_DIRS ${Cairo_INCLUDE_DIRS} ${Cairo_INCLUDE_DIRS}/cairo)
endif (Cairo_INCLUDE_DIRS AND Cairo_LIBRARY)
