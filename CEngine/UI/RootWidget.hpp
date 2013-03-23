/**********************************************************************
File name: RootWidget.hpp
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
#ifndef _PYE_UI_ROOTWIDGET_H
#define _PYE_UI_ROOTWIDGET_H

#include "CEngine/WindowInterface/EventSink.hpp"

#include "WidgetBase.hpp"

namespace PyEngine {

class RootWidget: public AbstractWidget,
                  public WidgetContainer,
                  public EventSink,
                  public std::enable_shared_from_this<RootWidget> {
public:
    RootWidget();
    virtual ~RootWidget();
public:
    virtual RootPtr get_root();
public:
    virtual void dispatch_hide();
    virtual void dispatch_key_down(unsigned int key,
                                   unsigned int modifiers);
    virtual void dispatch_key_up(unsigned int key,
                                 unsigned int modifiers);
    virtual void dispatch_mouse_click(int x, int y,
                                      unsigned int button,
                                      unsigned int modifiers,
                                      unsigned int nth);
    virtual void dispatch_mouse_down(int x, int y,
                                     unsigned int button,
                                     unsigned int modifiers);
    virtual void dispatch_mouse_move(int x, int y,
                                     int dx, int dy,
                                     unsigned int buttons,
                                     unsigned int modifiers);
    virtual void dispatch_mouse_up(int x, int y,
                                   unsigned int button,
                                   unsigned int modifiers);
    virtual void dispatch_resize(unsigned int w, unsigned int h);
    virtual void dispatch_scroll(int x, int y,
                                 int scrollx, int scrolly);
    virtual void dispatch_show();
    virtual void dispatch_text_input(const char* text);
    virtual void dispatch_wm_quit();
};

}

#endif
