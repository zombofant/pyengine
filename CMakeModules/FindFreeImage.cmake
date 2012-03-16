
find_path(FREEIMAGE_INCLUDE_DIRS FreeImage.h
    PATH_SUFFIXES include
    PATHS
        /usr
        /usr/local
)

find_library(FREEIMAGE_LIBRARY
    NAMES freeimage
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)
set(FREEIMAGE_FOUND "NO")
if (FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARY)
    set(FREEIMAGE_FOUND "YES")
endif (FREEIMAGE_INCLUDE_DIR AND FREEIMAGE_LIBRARY)
