
find_path(GLEW_INCLUDE_DIRS glew.h
    PATH_SUFFIXES include/GL include
    PATHS
        /usr
        /usr/local
)

find_library(GLEW_LIBRARY
    NAMES GLEW
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)
set(GLEW_FOUND "NO")
if (GLEW_INCLUDE_DIR)
    if (GLEW_LIBRARY)
        set(GLEW_FOUND "YES")
    endif (GLEW_LIBRARY)
endif (GLEW_INCLUDE_DIR)
