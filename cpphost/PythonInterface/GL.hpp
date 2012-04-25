/**********************************************************************
File name: GL.hpp
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
#ifndef _PYUNI_PYTHON_GL_H
#define _PYUNI_PYTHON_GL_H

#include <string>

#include <boost/python.hpp>

#include "GL/Base.hpp"
#include "GL/StateManagement.hpp"
#include "GL/GeometryBuffer.hpp"
#include "GL/GeometryBufferView.hpp"
#include "GL/CairoUtils.hpp"

namespace PyUni {

using namespace PyUni::GL;

class StructWrap: public Struct, public boost::python::wrapper<Struct>
{
    public:
        virtual void bind()
        {
            this->get_override("bind")();
        }
        
        virtual void unbind()
        {
            this->get_override("unbind")();
        }
};

class ClassWrap: public Class, public boost::python::wrapper<Class>
{
    public:
        virtual void bind()
        {
            this->get_override("bind")();
        }
        
        virtual void unbind()
        {
            this->get_override("unbind")();
        }
};

class GroupWrap: public Group, public boost::python::wrapper<Group>
{
    public:
        GroupWrap(int order = 0):
            Group::Group(order) {}
    public:
        virtual void execute()
        {
            if (boost::python::override f = this->get_override("execute"))
            {
                f();
            }
            else
            {
                this->Group::execute();
            }
        }

        void __bp_execute()
        {
            this->Group::execute();
    }
};

class ParentGroupWrap: public ParentGroup, public boost::python::wrapper<ParentGroup>
{
    public:
        ParentGroupWrap(int order = 0):
            ParentGroup::ParentGroup(order) {}
    public:
        virtual void execute()
        {
            if (boost::python::override f = this->get_override("execute"))
            {
                f();
            }
            else
            {
                this->ParentGroup::execute();
            }
        }

        virtual void setUp()
        {
            if (boost::python::override f = this->get_override("setUp"))
            {
                f();
            }
            else
            {
                this->ParentGroup::setUp();
            }
        }

        virtual void tearDown()
        {
            if (boost::python::override f = this->get_override("tearDown"))
            {
                f();
            }
            else
            {
                this->ParentGroup::tearDown();
            }
        }

        void __bp_execute()
        {
            this->ParentGroup::execute();
        }

        void __bp_setUp()
        {
            this->ParentGroup::setUp();
        }

        void __bp_tearDown()
        {
            this->ParentGroup::tearDown();
        }
};

void addGLToInittab();

}

#endif
