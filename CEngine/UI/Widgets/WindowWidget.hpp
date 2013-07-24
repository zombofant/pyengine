/**********************************************************************
File name: WindowWidget.hpp
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
#ifndef _PYE_UI_WINDOW_WIDGET_H
#define _PYE_UI_WINDOW_WIDGET_H

#include "CEngine/UI/Events.hpp"

#include "BoxWidget.hpp"
#include "LabelWidget.hpp"

namespace PyEngine {

class TitleBar: public HBox
{
public:
    TitleBar();

private:
    LabelWidget *_caption;

public:
    inline const std::string &get_caption() const {
        return _caption->get_text();
    };

    void set_caption(const std::string &value);

public:
    bool ev_mouse_down(int x, int y, MouseButton button,
                       KeyModifiers modifiers) override;

};

class Window: public VBox
{
public:
    typedef size_t ModalResult;

public:
    Window();

private:
    TitleBar *_titlebar;
    ModalResult _modal_result;
    NotifyEvent _on_modal_close;
    NotifyEvent _on_close_hook;

protected:
    void modal_close();

public:
    inline ModalResult get_modal_result() const {
        return _modal_result;
    };

    inline const std::string &get_caption() const {
        return _titlebar->get_caption();
    };

    inline NotifyEvent& on_close_hook() {
        return _on_close_hook;
    };

    inline NotifyEvent& on_modal_close() {
        return _on_modal_close;
    };

    void set_caption(const std::string &text);
    void set_modal_result(ModalResult result);

public:
    void close();

public:
    virtual bool ev_activate();
    virtual bool ev_deactivate();
    virtual bool ev_show_modal();

};

}

#endif
