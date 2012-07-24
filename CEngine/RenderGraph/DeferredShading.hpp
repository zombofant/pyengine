/**********************************************************************
File name: DeferredShading.hpp
This file is part of: Pythonic Universe

LICENSE

The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations under
the License.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public license (the  "GPL License"), in which case  the
provisions of GPL License are applicable instead of those above.

FEEDBACK & QUESTIONS

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYUNI_RENDERGRAPH_DEFERRED_SHADING_H
#define _PYUNI_RENDERGRAPH_DEFERRED_SHADING_H

#include <GL/glew.h>

#include "Stage.hpp"

namespace PyUni {
namespace RenderGraph {

class DeferredShadingStage: public Stage
{
    public:
        DeferredShadingStage(StageHandle parent, GLuint geometryFBO,
            GLuint resultFBO, NodeHandle geometryNode);
    private:
        GLuint _geometryFBO, _resultFBO;
        NodeHandle _geometryNode;
    public:
        virtual void execute();
    public:
        GLuint getGeometryFBO() { return _geometryFBO; }
        NodeHandle getGeometryNode() { return NodeHandle(_geometryNode); }
        GLuint getResultFBO() { return _resultFBO; }

        void setGeometryFBO(const GLuint value) { _geometryFBO = value; }
        void setGeometryNode(NodeHandle value) { _geometryNode = value; }
        void setResultFBO(const GLuint value) { _resultFBO = value; }
};

}
}

#endif
