
add_library(pyuni_PythonInterface STATIC
    "Package.cpp"
    "Window.cpp"
    "GL.cpp"
    "RenderGraph.cpp"
    "SceneGraph.cpp"
    )
add_dependencies(pyuni_PythonInterface pyuni_WindowInterface pyuni_SceneGraph pyuni_RenderGraph pyuni_GL)

