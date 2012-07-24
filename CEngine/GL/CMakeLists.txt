add_library(pyengine_GL
    "Base.cpp"
    "GenericBuffer.cpp"
    "GeometryBuffer.cpp"
    "IndexBuffer.cpp"
    "StateManagement.cpp"
    "GeometryBufferView.cpp"
    "GeometryObject.cpp"
    "AbstractImage.cpp"
    "TextureAtlas.cpp"
    "CairoUtils.cpp"
)
add_dependencies(pyengine_GL pyengine_Math pyengine_Misc)
