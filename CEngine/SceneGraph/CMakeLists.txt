
add_library(pyengine_SceneGraph STATIC
    "Spatial.cpp"
    "Node.cpp"
    "SceneGraph.cpp"
    "Leaf.cpp"
    )
add_dependencies(pyengine_SceneGraph pyengine_Math pyengine_GL)
