#ifndef PYUNI_RENDERGRAPH_STAGE_H
#define PYUNI_RENDERGRAPH_STAGE_H

#include <boost/shared_ptr.hpp>

namespace PyUni {
namespace RenderGraph {

class Stage;

class Node {
    public:
        Node(Stage *parent);
        virtual ~Node();
    protected:
        Stage *_parent;
    public:
        virtual void execute() = 0;
};

typedef boost::shared_ptr<Node> NodeHandle;

class Stage: public Node {
    public:
        Stage(Stage *parent);
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
