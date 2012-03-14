/**********************************************************************
File name: RenderGraph.hpp
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
#ifndef _PYUNI_PYTHON_RENDER_GRAPH_H
#define _PYUNI_PYTHON_RENDER_GRAPH_H

#include <boost/python.hpp>
#include "RenderGraph/Stage.hpp"

namespace PyUni {

class NodeWrap: public RenderGraph::Node, public boost::python::wrapper<RenderGraph::Node>
{
    public:
        NodeWrap(RenderGraph::StageHandle parent);
    public:
        virtual void execute()
        {
            this->get_override("execute")();
        }
};

class StageWrap: public RenderGraph::Stage, public boost::python::wrapper<RenderGraph::Stage>
{
    public:
        StageWrap(RenderGraph::StageHandle parent);
    public:
        void __bp_execute()
        {
            Stage::execute();
        }
        
        virtual void execute()
        {
            if (boost::python::override f = this->get_override("execute"))
            {
                f();
            }
            else
            {
                Stage::execute();
            }
        }
};

void addRenderGraphToInittab();

}

#endif
