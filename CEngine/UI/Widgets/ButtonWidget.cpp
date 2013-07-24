/**********************************************************************
File name: ButtonWidget.cpp
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
#include "ButtonWidget.hpp"

namespace PyEngine {

/* PyEngine::AbstractButton */

AbstractButton::AbstractButton():
    LabelledWidget(),
    _on_click()
{
    _flags = WidgetFlag::FOCUSABLE;
}

void AbstractButton::ev_click()
{
    _on_click(this);
}

bool AbstractButton::ev_mouse_click(int x, int y, MouseButton button,
                                    KeyModifiers modifiers,
                                    unsigned int nth)
{
    Rect& rect = absolute_rect();
    if ((0 <= x) && (x < rect.get_width()) &&
        (0 <= y) && (y < rect.get_height()))
    {
        ev_click();
        return true;
    }
    return false;
}

}
