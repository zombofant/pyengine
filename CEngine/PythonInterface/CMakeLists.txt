
add_library(pyengine_PythonInterface STATIC
    "Helpers.cpp"
    "Log.cpp"
    "CairoHelpers.cpp"
    "Pango.cpp"
    "Package.cpp"
    "Window.cpp"
    "GL.cpp"
    "Resources.cpp"
    "RenderGraph.cpp"
    "SceneGraph.cpp"
    )
add_dependencies(pyengine_PythonInterface pyengine_IO pyengine_WindowInterface pyengine_SceneGraph pyengine_RenderGraph pyengine_GL pyengine_Resources)

