
find_path(Pango_INCLUDE_DIRS pango-1.0
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

set(Pango_LIBRARIES ${PANGO_LIBRARY} ${PANGOCAIRO_LIBRARY})
set(Pango_FOUND "NO")
if (Pango_INCLUDE_DIRS AND Pango_LIBRARIES)
    set(Pango_FOUND "YES")
endif (Pango_INCLUDE_DIRS AND Pango_LIBRARIES)
