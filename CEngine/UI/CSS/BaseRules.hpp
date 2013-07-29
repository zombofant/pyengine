/**********************************************************************
File name: BaseRules.hpp
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
#ifndef _PYE_UI_BASE_RULES_H
#define _PYE_UI_BASE_RULES_H

#include <memory>
#include <list>

#include "Shapes.hpp"
#include "Fill.hpp"
#include "Style.hpp"

namespace PyEngine { namespace UI {

/**
 * Abstract base class for a CSS rule. Each rule type must inherit
 * from this one and will override the apply_to() method accordingly.
 */
struct AbstractRule
{
public:
    AbstractRule() = default;
    AbstractRule(const AbstractRule &ref) = default;
    AbstractRule& operator=(const AbstractRule &ref) = default;
    AbstractRule(AbstractRule &&ref) = default;
    AbstractRule& operator=(AbstractRule &&ref) = default;
    virtual ~AbstractRule();

public:
    /**
     * Apply the rule to the given Style instance.
     */
    virtual void apply_to(Style &to) const = 0;

    /**
     * Check whether the rule has any effect.
     *
     * @return false if the rule will, with the current settings, under
     *  no circumstances change a style object which is passed to
     *  apply_to(), true otherwise
     */
    virtual bool has_effect() const = 0;
};

typedef std::unique_ptr<AbstractRule> RulePtr;

struct RuleGroup: public AbstractRule
{
public:
    virtual ~RuleGroup() override = default;

private:
    std::list<RulePtr> _rules;

public:
    virtual void apply_to(Style &to) const override;
    virtual bool has_effect() const override;

    void add(RulePtr &&rule);
};

/**
 * Base rule for adjusting box lengths.
 */
struct BoxRule: public AbstractRule
{
public:
    BoxRule();
    virtual ~BoxRule() override = default;

private:
    bool _set_top, _set_right, _set_bottom, _set_left;
    css_coord_int_t _top, _right, _bottom, _left;

protected:
    void apply_to_box(CSSBox &to) const;

public:
    inline const css_coord_int_t& bottom() const {
        return _bottom;
    };

    inline const css_coord_int_t& left() const {
        return _left;
    };

    inline const css_coord_int_t& right() const {
        return _right;
    };

    inline const css_coord_int_t& top() const {
        return _top;
    };


public:
    void set_all(const css_coord_int_t &v);
    void set_all(const css_coord_int_t &top,
                 const css_coord_int_t &right,
                 const css_coord_int_t &bottom,
                 const css_coord_int_t &left);
    void set_bottom(const css_coord_int_t &v);
    void set_left(const css_coord_int_t &v);
    void set_right(const css_coord_int_t &v);
    void set_top(const css_coord_int_t &v);

    void unset_bottom();
    void unset_left();
    void unset_right();
    void unset_top();

public:
    virtual bool has_effect() const override;
};

/**
 * Base rule to set a fill.
 */
struct FillRule: public AbstractRule
{
public:
    typedef CSSFillPtr value_type;

public:
    FillRule();
    virtual ~FillRule() override = default;

private:
    CSSFillPtr _fill;

public:
    inline const CSSFillPtr& value() const {
        return _fill;
    };

public:
    void set(const CSSFillPtr &v);
    void unset();

public:
    virtual bool has_effect() const override;

};

/**
 * Template for a rule which only uses a (possibly inheritable)
 * primitive CSS type.
 */
template <typename value_t>
struct PrimitiveRule: public AbstractRule
{
public:
    typedef value_t value_type;

public:
    PrimitiveRule():
        AbstractRule(),
        _set_value(false),
        _value()
    {

    };

    virtual ~PrimitiveRule() override = default;

private:
    bool _set_value;
    value_t _value;

public:
    inline const value_t& value() const {
        return _value;
    };

public:
    void set(const value_t &v) {
        _set_value = true;
        _value = v;
    };

    void unset() {
        _set_value = false;
    };

public:
    virtual bool has_effect() const override {
        return _set_value;
    };

};

/**
 * A rule which enforces a non-negative numeric CSS value (or inherit).
 */
template <typename value_t, typename subrule_t = PrimitiveRule<value_t>>
struct NonNegativeValueRule: public subrule_t
{
public:
    typedef value_t value_type;

public:
    NonNegativeValueRule():
        subrule_t()
    {

    };

public:
    void set(const value_t &v) {
        if (!value_is_inherit(v) && (v < 0)) {
            throw std::out_of_range("Value must be non-negative. Got "
                + std::to_string(v));
        }
        subrule_t::set(v);
    }

};

std::unique_ptr<RuleGroup> rule_group(std::initializer_list<AbstractRule*> rules);
std::unique_ptr<RuleGroup> rule_group(std::initializer_list<RulePtr> rules);

}
}

#endif
