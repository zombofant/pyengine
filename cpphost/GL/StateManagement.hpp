/**********************************************************************
File name: StateManagement.hpp
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
#ifndef _PYUNI_GL_STATE_MANAGEMENT_H
#define _PYUNI_GL_STATE_MANAGEMENT_H

#include <boost/shared_ptr.hpp>

#include "Math/Matrices.hpp"

#include "Base.hpp"
#include "IndexBuffer.hpp"

namespace PyUni {
namespace GL {

class Group;

typedef boost::shared_ptr<Group> GroupHandle;

class Group {
    public:
        Group(int order = 0);
    protected:
        const int _order;
        StaticIndexBuffer *_indexBuffer;
        StaticIndexBufferHandle _ibHandle;
    protected:
        void drawGeometry() const;
    public:
        int compare(const Group &other) const;
        StaticIndexBufferHandle getIndexBuffer() { return _ibHandle; };
    public:
        bool operator == (const Group &other) const;
        bool operator != (const Group &other) const;
        bool operator <  (const Group &other) const;
        bool operator <= (const Group &other) const;
        bool operator >  (const Group &other) const;
        bool operator >= (const Group &other) const;
    public:
        virtual void execute();
};

class ParentGroup: public Group {
    public:
        ParentGroup(int order = 0);
    protected:
        std::vector<GroupHandle> _children;
    protected:
        void executeChildren();
    public:
        void add(GroupHandle handle);
        void remove(GroupHandle handle);
    public:
        virtual void execute();
        virtual void setUp();
        virtual void tearDown();
};

class StateGroup: public ParentGroup {
    public:
        StateGroup(StructHandle glObject, int order = 0);
    private:
        StructHandle _glObjectHandle;
        Struct *_glObject;
    public:
        virtual void setUp();
        virtual void tearDown();
};

class TransformGroup: public ParentGroup {
    public:
        TransformGroup(const Matrix4f *matrix, int order = 0);
    private:
        const Matrix4f *_matrix;
    public:
        virtual void setUp();
        virtual void tearDown();
};

}
}

#endif
