
find_path(FREEIMAGE_PLUS_INCLUDE_DIRS FreeImagePlus.h
    PATH_SUFFIXES include
    PATHS
        /usr
        /usr/local
)

find_library(FREEIMAGE_PLUS_LIBRARY
    NAMES freeimageplus
    PATH_SUFFIXES
        lib64 lib
    PATHS
        /usr
        /usr/local
)
set(FREEIMAGE_PLUS_FOUND "NO")
if (FREEIMAGE_PLUS_INCLUDE_DIR AND FREEIMAGE_PLUS_LIBRARY)
    set(FREEIMAGE_PLUS_FOUND "YES")
endif (FREEIMAGE_PLUS_INCLUDE_DIR AND FREEIMAGE_PLUS_LIBRARY)
