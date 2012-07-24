add_library(pyengine_Resources
    "Image.cpp"
    "Font.cpp"
)
add_dependencies(pyengine_Resources pyengine_GL pyengine_IO)
