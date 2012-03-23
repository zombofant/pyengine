add_library(pyuni_GL
    "Base.cpp"
    "GenericBuffer.cpp"
    "GeometryBuffer.cpp"
    "IndexBuffer.cpp"
    "StateManagement.cpp"
    "GeometryBufferView.cpp"
    "GeometryObject.cpp"
    "AbstractImage.cpp"
    "TextureAtlas.cpp"
)
add_dependencies(pyuni_GL pyuni_Math pyuni_Misc)
