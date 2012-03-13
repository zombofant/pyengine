/**********************************************************************
File name: Spatial.hpp
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

#ifndef _PYUNI_SCENEGRAPH_SPATIAL_H
#define _PYUNI_SCENEGRAPH_SPATIAL_H

#include "Matrices.hpp"

namespace PyUni {
namespace SceneGraph {

class Spatial
{
    public:
        virtual ~Spatial();

        //Bound worldBound;
        Matrix4 localTransformation;
        Matrix4 worldTransformation;

        void updateGeometry(bool initiator=true);

        inline Spatial* getParent() { return parent; }
        inline void setParent(Spatial *p) { parent = p; }

        virtual void onDraw();
        virtual void draw();

    protected:
        Spatial();

        virtual void updateWorldData();

        Spatial *parent;
};

}
}
#endif

