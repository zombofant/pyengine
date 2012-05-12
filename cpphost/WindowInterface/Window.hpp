/**********************************************************************
File name: Window.hpp
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

#ifndef _PYUNI_WINDOW_H
#define _PYUNI_WINDOW_H

#include <boost/shared_ptr.hpp>

namespace PyUni {
class Window {
    public:
        Window();
        virtual ~Window();

        virtual void flip() = 0;
        virtual void switchTo() = 0;

        virtual void setTitle(const char *title) = 0;
    private:
        bool _glewInitialized;
    public:
        void initializeGLEW();

};
}

#endif

// Local Variables:
// c-file-style: "k&r"
// c-basic-offset: 4
// End:
