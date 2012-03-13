#ifndef PYUNI_RENDERGRAPH_DEFERRED_SHADING_H
#define PYUNI_RENDERGRAPH_DEFERRED_SHADING_H

#include <GL/glew.h>
#include "Stage.hpp"

namespace PyUni {
namespace RenderGraph {

class DeferredShadingStage: public Stage
{
    public:
        DeferredShadingStage(Stage *parent, GLuint geometryFBO,
            GLuint resultFBO, NodeHandle geometryNode);
    private:
        GLuint _geometryFBO, _resultFBO;
        NodeHandle _geometryNode;
    public:
        virtual void execute();
};

}
}

#endif
