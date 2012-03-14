
add_library(pyuni_SceneGraph STATIC
    "Spatial.cpp"
    "Node.cpp"
    "SceneGraph.cpp"
    "Leaf.cpp"
    )
add_dependencies(pyuni_SceneGraph pyuni_Math pyuni_GL)
