add_library(pyengine_VFS
    "Errors.cpp"
    "Utils.cpp"
    "FileSystem.cpp"
    "Mount.cpp"
)

add_dependencies(pyengine_VFS pyengine_IO)
