/**********************************************************************
File name: Selectors.hpp
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
#ifndef _PYE_UI_SELECTORS_H
#define _PYE_UI_SELECTORS_H

#include <array>
#include <memory>

#include "CSS.hpp"

namespace PyEngine {

class Specifity: public std::array<unsigned int, 4>
{
public:
    Specifity();
    Specifity(unsigned int a, unsigned int b,
              unsigned int c, unsigned int d);
    Specifity(const Specifity& ref);
    Specifity& operator=(const Specifity& ref);
public:
    Specifity& operator+=(const Specifity& other);
    Specifity operator+(const Specifity& other);
};

class Selector;
typedef std::shared_ptr<Selector> SelectorPtr;

class AbstractWidget;
typedef std::shared_ptr<AbstractWidget> AbstractWidgetPtr;

class Selector: public std::enable_shared_from_this<Selector>
{
public:
    Selector();
    Selector(SelectorPtr chained);
    virtual ~Selector();
protected:
    Specifity _specifity;
    SelectorPtr _chained;
protected:
    virtual AbstractWidgetPtr _test_widget(AbstractWidgetPtr widget) const = 0;
public:
    virtual bool operator==(const Selector& oth_) const;
    inline bool operator!=(const Selector& oth_) const {
        return !(operator==(oth_));
    };

    inline const Specifity& specifity() const {
        return _specifity;
    };

    AbstractWidgetPtr test_widget(AbstractWidgetPtr widget) const;
};

class ParentSelector: public Selector
{
public:
    ParentSelector(SelectorPtr parent);
    ParentSelector(SelectorPtr parent, SelectorPtr chained);
protected:
    SelectorPtr _parent;
public:
    virtual bool operator==(const Selector& oth_) const;
};

class ChildOf: public ParentSelector
{
public:
    ChildOf(SelectorPtr parent);
    ChildOf(SelectorPtr parent, SelectorPtr chained);
protected:
    virtual AbstractWidgetPtr _test_widget(AbstractWidgetPtr widget) const = 0;
};

class DirectChildOf: public ParentSelector
{
public:
    DirectChildOf(SelectorPtr parent);
    DirectChildOf(SelectorPtr parent, SelectorPtr chained);
protected:
    virtual AbstractWidgetPtr _test_widget(AbstractWidgetPtr widget) const = 0;
};

class Is: public Selector
{
public:
    Is(const std::string& element_name);
private:
    const std::string _element_name;
protected:
    virtual AbstractWidgetPtr _test_widget(AbstractWidgetPtr widget) const = 0;
};

class State: public Selector
{
public:
    State(CSSStateFlag flag);
    State(CSSStateFlag flag, SelectorPtr chained);
protected:
    CSSState _states;
protected:
    virtual AbstractWidgetPtr _test_widget(AbstractWidgetPtr widget) const = 0;
};

}

#endif
