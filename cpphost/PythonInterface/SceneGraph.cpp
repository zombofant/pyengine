/**********************************************************************
File name: SceneGraph.cpp
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
#include "SceneGraph.hpp"

#include "Helpers.hpp"

#include <boost/python/manage_new_object.hpp>

namespace PyUni {

using namespace boost::python;
using namespace PyUni::SceneGraph;

typedef MapHelper<VertexMap> VertexMapHelper;

BOOST_PYTHON_MODULE(_cuni_scenegraph)
{
    class_<VertexMap, boost::noncopyable>("VertexMap", no_init)
        .def("__len__", &VertexMap::size)
        .def("iteritems", &VertexMapHelper::iteritems,
            return_value_policy<manage_new_object>())
    ;

    class_<Spatial, SpatialHandle, boost::noncopyable>("Spatial", no_init)
    ;

    class_<Node, bases<Spatial>, NodeHandle, boost::noncopyable>("Node", no_init)
        .def("__init__", make_constructor(&Node::create))
    ;

    class_<LeafWrap, bases<Spatial>, boost::shared_ptr<LeafWrap>, boost::noncopyable>("Leaf", no_init)
    ;
}

void addSceneGraphToInittab()
{
    PyImport_AppendInittab("_cuni_scenegraph", &init_cuni_scenegraph);
}

}
