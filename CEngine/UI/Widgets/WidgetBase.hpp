/**********************************************************************
File name: WidgetBase.hpp
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
#ifndef _PYE_UI_WIDGET_H
#define _PYE_UI_WIDGET_H

#include <memory>
#include <vector>
#include <stdexcept>
#include <list>
#include <set>

#include "CEngine/UI/Key.hpp"
#include "CEngine/UI/Static.hpp"

#include "CEngine/UI/CSS/CSS.hpp"
#include "CEngine/UI/CSS/Shapes.hpp"

namespace PyEngine {

class WidgetError: public std::invalid_argument
{
public:
    WidgetError(const std::string& what_arg);
    WidgetError(const char* what_arg);
};

class AbstractWidget;
class RootWidget;
class ParentWidget;

typedef std::shared_ptr<ParentWidget> ParentPtr;
typedef std::weak_ptr<ParentWidget> ParentWPtr;

typedef std::shared_ptr<RootWidget> RootPtr;
typedef std::weak_ptr<RootWidget> RootWPtr;

typedef std::shared_ptr<AbstractWidget> WidgetPtr;

typedef std::list<WidgetPtr> HitChain;
typedef std::set<WidgetPtr> HitSet;

class AbstractWidget
{
public:
    AbstractWidget();
    AbstractWidget(const AbstractWidget &ref) = delete;
    AbstractWidget& operator=(const AbstractWidget &ref) = delete;
    virtual ~AbstractWidget();

protected:
    ParentWPtr _parent;
    RootWPtr _root;
    bool _alignment_invalidated, _computed_style_invalidated;
    Rect _absolute_rect;
    bool _visible;
    CSSState _state;
    WidgetFlags _flags;

protected:
    virtual void _parent_changed();
    virtual void _root_changed();
    void set_root(RootPtr root);
    virtual bool _hittest(const Point &p) const = 0;

public:
    inline void invalidate_alignment() {
        _alignment_invalidated = true;
    };

    inline void invalidate_computed_style() {
        _computed_style_invalidated = true;
    };

public:
    inline const Rect& get_absolute_rect() const {
        return _absolute_rect;
    };

    inline Rect& absolute_rect() {
        return _absolute_rect;
    };

    inline void set_absolute_rect(const Rect& value) {
        _absolute_rect = value;
    };

    inline WidgetFlags& flags() {
        return _flags;
    };

    inline const WidgetFlags& flags() const {
        return _flags;
    };

public:
    inline ParentPtr get_parent() const {
        return _parent.lock();
    };

    virtual RootPtr get_root() {
        return _root.lock();
    };

    void set_parent(ParentPtr parent);
public:
    virtual void do_align();
    virtual const char* element_name() const;
    virtual WidgetPtr hittest(const Point &p) = 0;
    virtual bool hittest_with_chain(const Point &p, HitChain &chain) = 0;
    virtual void realign();
    virtual void render();

    inline CSSState& state() {
        return _state;
    };

    inline const CSSState& state() const {
        return _state;
    };

public:
    virtual bool ev_activate();
    virtual bool ev_caret_motion(CaretMotionDirection direction,
                                 CaretMotionStep step,
                                 bool select);
    virtual bool ev_deactivate();
    virtual bool ev_key_down(Key::Key key, KeyModifiers modifiers);
    virtual bool ev_key_up(Key::Key key, KeyModifiers modifiers);
    virtual bool ev_mouse_click(int x, int y, MouseButton button,
                                KeyModifiers modifiers, unsigned int nth);
    virtual bool ev_mouse_down(int x, int y, MouseButton button,
                               KeyModifiers modifiers);
    virtual bool ev_mouse_enter();
    virtual bool ev_mouse_leave();
    virtual bool ev_mouse_move(int x, int y, int dx, int dy,
                               MouseButton button, KeyModifiers modifiers);
    virtual bool ev_mouse_up(int x, int y, MouseButton button,
                             KeyModifiers modifiers);
    virtual bool ev_resize();
    virtual bool ev_scroll(int scrollx, int scrolly);
    virtual bool ev_text_input(const char* buf);

public:
    /* for updating the root widget */
    friend class ParentWidget;

    /* for sending events */
    friend class RootWidget;
};

class ParentWidget: public AbstractWidget,
                    public std::enable_shared_from_this<ParentWidget>
{
public:
    typedef std::vector<WidgetPtr> container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;

public:
    ParentWidget();
    virtual ~ParentWidget();

protected:
    container_type _children;

protected:
    bool _hittest(const Point &p) const override;
    WidgetPtr _hittest_children(const Point &p) const;
    virtual void _root_changed();

public:
    void add(WidgetPtr child);
    iterator begin();
    void bring_to_front(WidgetPtr child);
    const_iterator cbegin() const;
    const_iterator cend() const;
    iterator end();
    iterator find(WidgetPtr child);
    const_iterator find(WidgetPtr child) const;
    void remove(WidgetPtr child);
    void send_to_back(WidgetPtr child);

public:
    virtual RootPtr get_root() const = 0;
    WidgetPtr hittest(const Point &p) override;
    bool hittest_with_chain(const Point &p, HitChain &chain) override;
    void realign() override;
    void render() override;

};

class Widget: public AbstractWidget,
              public std::enable_shared_from_this<Widget>
{
public:
    Widget();
    virtual ~Widget();

protected:
    virtual bool _hittest(const Point &p) const override;

public:
    WidgetPtr hittest(const Point &p) override;
    bool hittest_with_chain(const Point &p, HitChain &chain) override;

};

}

#endif
