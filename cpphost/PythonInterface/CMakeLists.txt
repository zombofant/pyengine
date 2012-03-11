
add_library(pyuni_PythonInterface STATIC
    "Module.cpp"
    )
add_dependencies(pyuni_PythonInterface pyuni_WindowInterface)

