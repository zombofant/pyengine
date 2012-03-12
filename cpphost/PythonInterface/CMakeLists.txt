
add_library(pyuni_PythonInterface STATIC
    "Package.cpp"
    "Window.cpp"
    )
add_dependencies(pyuni_PythonInterface pyuni_WindowInterface)

