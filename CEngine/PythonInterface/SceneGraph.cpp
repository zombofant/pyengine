/**********************************************************************
File name: SceneGraph.cpp
This file is part of: Pythonic Engine

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

For feedback and questions about pyengine please e-mail one of the
authors named in the AUTHORS file.
**********************************************************************/
#include "SceneGraph.hpp"

#include <boost/python/manage_new_object.hpp>

#include "Helpers.hpp"

namespace PyEngine {

using namespace boost::python;
using namespace PyEngine::SceneGraph;

typedef MapHelper<VertexMap, VertexMapHandle> VertexMapHelper;

BOOST_PYTHON_MODULE(_cuni_scenegraph)
{
    class_<PyEngine::SceneGraph::SceneGraph, bases<>, PyEngine::SceneGraph::SceneGraphHandle, boost::noncopyable>("SceneGraph", no_init)
        .def("__init__", make_constructor(&PyEngine::SceneGraph::SceneGraph::create))
        .def("update", &PyEngine::SceneGraph::SceneGraph::update)
        .def("draw", &PyEngine::SceneGraph::SceneGraph::draw)
        .add_property("RootNode", &PyEngine::SceneGraph::SceneGraph::getRootNode)
    ;

    VertexMapHelper::ItemsIteratorRegT::wrap("__VertexMap_IterItems");
    VertexMapHelper::KeysIteratorRegT::wrap("__VertexMap_IterKeys");
    VertexMapHelper::ValuesIteratorRegT::wrap("__VertexMap_IterValues");

    class_<VertexMap, VertexMapHandle, boost::noncopyable>("VertexMap", no_init)
        .def("__len__", &VertexMap::size)
        .def("__getitem__", &VertexMapHelper::__getitem__)
        .def("__setitem__", &VertexMapHelper::__setitem__)
        .def("iteritems", &VertexMapHelper::iteritems,
            return_value_policy<manage_new_object>())
        .def("iterkeys", &VertexMapHelper::iterkeys,
            return_value_policy<manage_new_object>())
        .def("itervalues", &VertexMapHelper::itervalues,
            return_value_policy<manage_new_object>())
    ;

    class_<Spatial, SpatialHandle, boost::noncopyable>("Spatial", no_init)
        .def("translate", &Spatial::translate)
        .def("setTranslation", &Spatial::setTranslation)
        .def("rotate", &Spatial::rotate)
        .def("setRotation", &Spatial::setRotation)
        .def("scale", &Spatial::scale)
        .def("setScale", &Spatial::setScale)
        .def("draw", pure_virtual(&Spatial::draw))
        .def("resetTransformation", &Spatial::resetTransformation)
        .def("applyTransformation", &Spatial::applyTransformation)
    ;

    class_<Node, bases<Spatial>, NodeHandle, boost::noncopyable>("Node", no_init)
        .def("__init__", make_constructor(&Node::create))
        .def("addChild", &Node::addChild)
        .def("removeChild", &Node::removeChild)
   ;

    class_<LeafWrap, bases<Spatial>, boost::shared_ptr<LeafWrap>, boost::noncopyable>("Leaf")
        .add_property("VertexMap", &Leaf::getVertexMap)
    ;
    implicitly_convertible<boost::shared_ptr<LeafWrap>, SpatialHandle>();
}

void addSceneGraphToInittab()
{
    PyImport_AppendInittab("_cuni_scenegraph", &init_cuni_scenegraph);
}

}
