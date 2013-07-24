/**********************************************************************
File name: LabelWidget.hpp
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
#ifndef _PYE_UI_LABEL_WIDGET_H
#define _PYE_UI_LABEL_WIDGET_H

#include "WidgetBase.hpp"

#include "CEngine/UI/Label.hpp"

namespace PyEngine {

class LabelledWidget: public Widget
{
public:
    LabelledWidget();

private:
    Rect _label_rect;

protected:
    Label _label;

public:
    void do_align() override;
    coord_dimensions_t get_dimensions() override;
    void invalidate_computed_style() override;
    void invalidate_context() override;
    void render() override;

};

class LabelWidget: public LabelledWidget
{
public:
    LabelWidget();
    LabelWidget(const std::string &text);

public:
    inline const std::string& get_text() const {
        return _label.get_text();
    };

    void set_text(const std::string &value);
};

}

#endif
