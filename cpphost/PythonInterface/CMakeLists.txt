
add_library(pyuni_PythonInterface STATIC
    "Package.cpp"
    "Window.cpp"
    "GL.cpp"
    )
add_dependencies(pyuni_PythonInterface pyuni_WindowInterface pyuni_RenderGraph pyuni_GL)

