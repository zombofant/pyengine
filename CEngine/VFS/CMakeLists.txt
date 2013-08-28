add_library(pyengine_VFS
    "Common.cpp"
    "Errors.cpp"
    "Utils.cpp"
    "FileSystem.cpp"
    "Mount.cpp"
)

add_dependencies(pyengine_VFS pyengine_IO)
