add_library(pyuni_Resources
    "PNGIOWrapper.cpp"
    "Image.cpp"
    "FTIOWrapper.cpp"
    "Font.cpp"
)
add_dependencies(pyuni_Resources pyuni_GL)
