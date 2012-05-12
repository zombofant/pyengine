
add_library(pyuni_PythonInterface STATIC
    "Helpers.cpp"
    "CairoHelpers.cpp"
    "Pango.cpp"
    "Package.cpp"
    "Window.cpp"
    "GL.cpp"
    "Resources.cpp"
    "RenderGraph.cpp"
    "SceneGraph.cpp"
    )
add_dependencies(pyuni_PythonInterface pyuni_WindowInterface pyuni_SceneGraph pyuni_RenderGraph pyuni_GL pyuni_Resources)

