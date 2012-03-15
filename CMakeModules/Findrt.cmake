
set(rt_INCLUDE_DIRS)

find_library(rt_LIBRARY
    NAMES rt
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)
set(rt_FOUND "NO")
if (rt_LIBRARY)
    set(rt_FOUND "YES")
endif (rt_LIBRARY)
