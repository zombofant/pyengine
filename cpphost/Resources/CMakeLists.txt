add_library(pyuni_Resources
    "Image.cpp"
    "Font.cpp"
)
add_dependencies(pyuni_Resources pyuni_GL pyuni_IO)
