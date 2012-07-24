/**********************************************************************
File name: RenderGraph.cpp
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
#include "RenderGraph.hpp"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include "RenderGraph/DeferredShading.hpp"

namespace PyUni {

using namespace boost::python;
using namespace RenderGraph;

NodeWrap::NodeWrap(StageHandle parent):
    Node::Node(parent)
{

}

StageWrap::StageWrap(StageHandle parent):
    Stage::Stage(parent)
{
    
}

BOOST_PYTHON_MODULE(_cuni_rendergraph)
{
    class_<NodeWrap, boost::shared_ptr<NodeWrap>, boost::noncopyable>(
            "Node", init<StageHandle>())
        .def("execute",
            pure_virtual(&Node::execute))
        .add_property("Parent",
            &Node::getParent)
    ;
    implicitly_convertible<boost::shared_ptr<NodeWrap>, NodeHandle >();
    // implicitly_castable<boost::weak_ptr<Node>, NodeHandle >();

    class_<StageWrap, bases<Node>, boost::shared_ptr<StageWrap>, boost::noncopyable>(
            "Stage", init<StageHandle>())
        .def("execute",
            &Stage::execute,
            &StageWrap::__bp_execute)
    ;
    implicitly_convertible<boost::shared_ptr<StageWrap>, StageHandle >();
    // implicitly_castable<boost::weak_ptr<Node>, StageHandle >();

    class_<DeferredShadingStage, bases<Stage>, boost::shared_ptr<DeferredShadingStage>, boost::noncopyable>(
            "DeferredShadingStage", init<StageHandle, GLenum, GLenum, NodeHandle>())
        .add_property("GeometryFBO",
            &DeferredShadingStage::getGeometryFBO,
            &DeferredShadingStage::setGeometryFBO)
        .add_property("ResultFBO",
            &DeferredShadingStage::getResultFBO,
            &DeferredShadingStage::setResultFBO)
        .add_property("GeometryNode",
            &DeferredShadingStage::getGeometryNode,
            &DeferredShadingStage::setGeometryNode)
    ;
    
    implicitly_convertible<boost::shared_ptr<DeferredShadingStage>, StageHandle >();
}

void addRenderGraphToInittab()
{
    PyImport_AppendInittab("_cuni_rendergraph", &init_cuni_rendergraph);
}

}
