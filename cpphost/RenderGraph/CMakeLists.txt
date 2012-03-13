
add_library(pyuni_RenderGraph STATIC
    "Stage.cpp"
    "DeferredShading.cpp"
)
add_dependencies(pyuni_RenderGraph pyuni_SceneGraph)
