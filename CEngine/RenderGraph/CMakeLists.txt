
add_library(pyengine_RenderGraph STATIC
    "Stage.cpp"
    "DeferredShading.cpp"
)
add_dependencies(pyengine_RenderGraph pyengine_SceneGraph pyengine_GL)
