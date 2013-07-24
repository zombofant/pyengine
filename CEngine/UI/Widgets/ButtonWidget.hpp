/**********************************************************************
File name: ButtonWidget.hpp
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
#ifndef _PYE_UI_BUTTON_WIDGET_H
#define _PYE_UI_BUTTON_WIDGET_H

#include "CEngine/UI/Events.hpp"

#include "LabelWidget.hpp"

namespace PyEngine {

class AbstractButton: public LabelledWidget
{
public:
    AbstractButton();

private:
    NotifyEvent _on_click;

public:
    inline const std::string& get_caption() const {
        return _label.get_text();
    };

    inline NotifyEvent& on_click() {
        return _on_click;
    };

    void set_caption(const std::string &value);

public:
    virtual void ev_click();
    virtual bool ev_mouse_click(int x, int y, MouseButton button,
                                KeyModifiers modifiers,
                                unsigned int nth) override;
};

class Button: public AbstractButton
{
public:
    Button();

};

}

#endif
