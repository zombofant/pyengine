/**********************************************************************
File name: CSS.hpp
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
#ifndef _PYE_UI_CSS_H
#define _PYE_UI_CSS_H

#include <stdexcept>
#include <memory>
#include <limits>

namespace PyEngine {

typedef int coord_int_t;
typedef double coord_float_t;

struct Auto
{
    Auto();
    inline operator coord_int_t() const {
        return std::numeric_limits<coord_int_t>::min();
    };
    inline operator coord_float_t() const {
        return std::numeric_limits<coord_float_t>::signaling_NaN();
    };

    template <typename coord_t>
    static inline bool is_auto(coord_t value) {
        return (coord_t)Auto() == value;
    };
};

class UnresolvedInheritable: std::runtime_error
{
public:
    UnresolvedInheritable(const std::string& what_arg);
    UnresolvedInheritable(const char* what_arg);
};

enum _Inherit {
    Inherit
};

template <typename _value_type>
struct CSSInheritable
{
public:
    typedef _value_type value_type;
public:
    CSSInheritable():
        _inherit(false),
        _value()
    {

    };

    CSSInheritable(_Inherit inherit):
        _inherit(inherit == Inherit),
        _value()
    {

    };

    CSSInheritable(value_type &&value):
        _inherit(false),
        _value(std::move(value))
    {

    };

    CSSInheritable(const value_type &value):
        _inherit(false),
        _value(value)
    {

    };

    CSSInheritable(const CSSInheritable<value_type>& ref):
        _inherit(ref._inherit),
        _value((_inherit ? ref._value : value_type()))
    {

    };

    template<typename... arg_ts>
    explicit CSSInheritable(arg_ts... args):
        _inherit(false),
        _value(args...)
    {

    };

    CSSInheritable& operator=(const CSSInheritable<value_type>& ref)
    {
        _inherit = ref._inherit;
        if (!_inherit) {
            _value = ref._value;
        } else {
            _value = value_type();
        }
        return *this;
    };
private:
    bool _inherit;
    value_type _value;
private:
    template <typename other_value_type>
    auto eq_impl(const other_value_type& oth, long v = 0) const
        -> decltype(oth.is_inherit())
    {
        if (oth.is_inherit() || is_inherit()) {
            return false;
        }
        return (const typename other_value_type::value_type&)oth ==
            (const value_type&)(*this);
    };

    template <typename other_value_type>
    bool eq_impl(const other_value_type& oth, int v = 0) const
    {
        return oth == (const value_type&)_value;
    };

    template <typename other_value_type>
    auto ne_impl(const other_value_type& oth, long v = 0) const
        -> decltype(oth.is_inherit())
    {
        if (oth.is_inherit() || is_inherit()) {
            return false;
        }
        return (const typename other_value_type::value_type&)oth !=
            (const value_type&)(*this);
    };

    template <typename other_value_type>
    bool ne_impl(const other_value_type& oth, int v = 0) const
    {
        return oth != (const value_type&)_value;
    };

public:
    bool is_inherit() const {
        return _inherit;
    };

    operator value_type() {
        if (_inherit) {
            throw UnresolvedInheritable("Inheritable is unresolved.");
        }
        return _value;
    };

    operator const value_type&() const {
        if (_inherit) {
            throw UnresolvedInheritable("Inheritable is unresolved.");
        }
        return _value;
    };

    template <typename other_value_type>
    bool operator==(const other_value_type& oth) const {
        return eq_impl(oth, 0L);
    };

    template <typename other_value_type>
    bool operator!=(const other_value_type& oth) const {
        return ne_impl(oth, 0L);
    };
};

typedef CSSInheritable<coord_int_t> css_coord_int_t;
typedef CSSInheritable<coord_float_t> css_coord_float_t;

}

#endif
