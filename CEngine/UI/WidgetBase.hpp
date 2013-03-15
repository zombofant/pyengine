#ifndef _PYE_UI_WIDGET_H
#define _PYE_UI_WIDGET_H

#include <memory>
#include <vector>
#include <stdexcept>

#include "Shapes.hpp"

namespace PyEngine {

class WidgetError: public std::invalid_argument
{
public:
    WidgetError(const std::string& what_arg);
    WidgetError(const char* what_arg);
};

class AbstractWidget;
class WidgetContainer;
class RootWidget;

typedef std::shared_ptr<WidgetContainer> ParentPtr;
typedef std::weak_ptr<WidgetContainer> ParentWPtr;

typedef std::shared_ptr<RootWidget> RootPtr;
typedef std::weak_ptr<RootWidget> RootWPtr;

typedef std::shared_ptr<AbstractWidget> WidgetPtr;

class AbstractWidget: public std::enable_shared_from_this<AbstractWidget>
{
public:
    AbstractWidget();
    virtual ~AbstractWidget();
protected:
    ParentWPtr _parent;
    RootWPtr _root;
private:
    bool _alignment_invalidated, _computed_style_invalidated;
    Rect _absolute_rect;
    bool _visible, _enabled, _active, _hovered, _focused;
protected:
    virtual void _parent_changed();
    virtual void _root_changed();
    void set_root(RootPtr root);
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
public:
    inline ParentPtr get_parent() const {
        return _parent.lock();
    };

    virtual RootPtr get_root() const {
        return _root.lock();
    };

    void set_parent(ParentPtr parent);
public:
    virtual void do_align();
    virtual WidgetPtr hittest(const Point& p) = 0;
    virtual void realign();
    virtual void render();
public:
    /* for updating the root widget */
    friend class ParentWidget;

    /* for sending events */
    friend class RootWidget;
};

class WidgetContainer: public std::enable_shared_from_this<WidgetContainer>
{
public:
    typedef std::vector<WidgetPtr> container_type;
    typedef typename container_type::iterator iterator;
    typedef typename container_type::const_iterator const_iterator;
public:
    WidgetContainer();
    virtual ~WidgetContainer();
protected:
    container_type _children;
protected:
    WidgetPtr _hittest(const Point& p) const;
public:
    void add(WidgetPtr child);
    iterator begin();
    const_iterator cbegin() const;
    const_iterator cend() const;
    iterator end();
    iterator find(WidgetPtr child);
    const_iterator find(WidgetPtr child) const;
    void remove(WidgetPtr child);
public:
    virtual RootPtr get_root() const = 0;
};

class ParentWidget: public AbstractWidget, public WidgetContainer {
public:
    ParentWidget();
    virtual ~ParentWidget();
protected:
    virtual void _root_changed();
public:
    void bring_to_front(WidgetPtr child);
    void send_to_back(WidgetPtr child);
public:
    virtual WidgetPtr hittest(const Point& p);
    virtual void realign();
    virtual void render();
};

}

#endif
