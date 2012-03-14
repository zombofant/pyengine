/**********************************************************************
File name: Stage.hpp
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
#ifndef _PYUNI_RENDERGRAPH_STAGE_H
#define _PYUNI_RENDERGRAPH_STAGE_H

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace PyUni {
namespace RenderGraph {

class Stage;

typedef boost::shared_ptr<Stage> StageHandle;
typedef boost::weak_ptr<Stage> WeakStageHandle;

class Node {
    public:
        Node(StageHandle parent);
        virtual ~Node();
    protected:
        WeakStageHandle _parent;
    public:
        StageHandle getParent();
    public:
        virtual void execute() = 0;
};

typedef boost::shared_ptr<Node> NodeHandle;

class Stage: public Node {
    public:
        Stage(StageHandle parent);
        virtual ~Stage();
    protected:
        std::vector<NodeHandle> _nodes;
    protected:
        virtual bool allowNode(Node *node);
        void executeChildren();
    public:
        bool addNode(NodeHandle node);
        virtual void execute();
};

}
}

#endif
