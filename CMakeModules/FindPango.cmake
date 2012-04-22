
find_path(PANGO_INCLUDE_DIRS pango-1.0
    PATH_SUFFIXES include
    PATHS
        /usr
        /usr/local
)

find_library(PANGO_LIBRARY
    NAMES pango-1.0
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)

find_library(PANGOCAIRO_LIBRARY
    NAMES pangocairo-1.0
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)

set(PANGO_LIBRARIES ${PANGO_LIBRARY} ${PANGOCAIRO_LIBRARY})
set(PANGO_FOUND "NO")
if (PANGO_INCLUDE_DIRS AND PANGO_LIBRARIES)
    set(PANGO_FOUND "YES")
endif (PANGO_INCLUDE_DIRS AND PANGO_LIBRARIES)
