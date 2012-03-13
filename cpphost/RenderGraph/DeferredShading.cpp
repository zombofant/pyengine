#include "DeferredShading.hpp"

namespace PyUni {
namespace RenderGraph {

/* PyUni::RenderGraph::DeferredShadingStage */

DeferredShadingStage::DeferredShadingStage(Stage *parent,
    GLuint geometryFBO, GLuint resultFBO, NodeHandle geometryNode):
    Stage::Stage(parent),
    _geometryFBO(geometryFBO),
    _resultFBO(resultFBO),
    _geometryNode(geometryNode)
{

}

void DeferredShadingStage::execute()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _geometryFBO);
    _geometryNode->execute();
    glBindFramebuffer(GL_FRAMEBUFFER, _resultFBO);
    executeChildren();
}


}
}
