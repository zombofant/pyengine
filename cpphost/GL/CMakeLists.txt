add_library(pyuni_GL
    "Base.cpp"
    "GenericBuffer.cpp"
    "GeometryBuffer.cpp"
    "IndexBuffer.cpp"
    "StateManagement.cpp"
)
add_dependencies(pyuni_GL pyuni_Math)
