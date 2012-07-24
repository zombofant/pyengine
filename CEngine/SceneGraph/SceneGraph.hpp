/**********************************************************************
File name: SceneGraph.hpp
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

For feedback and questions about pyuni please e-mail one of the authors
named in the AUTHORS file.
**********************************************************************/
#ifndef _PYE_SCENEGRAPH_SCENEGRAPH_H
#define _PYE_SCENEGRAPH_SCENEGRAPH_H

#include <boost/shared_ptr.hpp>

#include "Node.hpp"

namespace PyEngine {
namespace SceneGraph {

class SceneGraph;

typedef boost::shared_ptr<SceneGraph> SceneGraphHandle;

class SceneGraph
{
    public:
        SceneGraph();
        ~SceneGraph();

        void update(double deltaT = 0);
        void draw();

        inline NodeHandle getRootNode() const { return _root; }

        static SceneGraphHandle create();

    protected:
        NodeHandle _root;
};

}
}
#endif

