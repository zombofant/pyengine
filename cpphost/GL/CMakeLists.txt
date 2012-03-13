add_library(pyuni_GL
    "Base.cpp"
    "GenericBuffer.cpp"
    "GeometryBuffer.cpp"
    "IndexBuffer.cpp"
    "StateManagement.cpp"
    "GeometryBufferView.cpp"
)
add_dependencies(pyuni_GL pyuni_Math)
