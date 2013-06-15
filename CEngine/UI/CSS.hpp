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

#include <iostream>
#include <stdexcept>
#include <memory>
#include <limits>
#include <bitset>

namespace PyEngine {

typedef int coord_int_t;
typedef double coord_float_t;

struct Auto
{
    Auto() = default;

    inline operator coord_int_t() const {
        return std::numeric_limits<coord_int_t>::min();
    };

    inline operator coord_float_t() const {
        return std::numeric_limits<coord_float_t>::signaling_NaN();
    };

    template <typename coord_t>
    static inline bool is_auto(coord_t value) {
        return (coord_t)Auto() == value;
    }
};

class UnresolvedInheritable: std::runtime_error
{
public:
    UnresolvedInheritable(const std::string& what_arg);
    UnresolvedInheritable(const char* what_arg);
};

enum inherit_t {
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

    CSSInheritable(inherit_t inherit):
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
        _value(_inherit ? value_type() : ref._value)
    {

    };

    template<typename... arg_ts>
    CSSInheritable(arg_ts... args):
        _inherit(false),
        _value(args...)
    {

    }

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
    void require_not_inherit() const
    {
        if (_inherit) {
            throw UnresolvedInheritable("Inheritable is unresolved.");
        }
    }

public:
    bool is_inherit() const {
        return _inherit;
    };

    operator value_type() {
        require_not_inherit();
        return _value;
    };

    operator const value_type&() const {
        require_not_inherit();
        return _value;
    };

    value_type& get() {
        require_not_inherit();
        return _value;
    }

    const value_type& get() const {
        require_not_inherit();
        return _value;
    }

    template <typename T2>
    void deinherit_with(const CSSInheritable<T2>& other)
    {
        if (!is_inherit()) {
            return;
        }
        if (other.is_inherit()) {
            return;
        }
        _inherit = false;
        _value = other.get();
    }
};

typedef CSSInheritable<coord_int_t> css_coord_int_t;
typedef CSSInheritable<coord_float_t> css_coord_float_t;

enum CSSStateFlag {
    Hovered,
    Focused,
    Active,
    Disabled
};

typedef std::bitset<std::numeric_limits<CSSStateFlag>::max()> _CSSStateBitset;

class CSSState: public _CSSStateBitset
{
public:
    CSSState();
    CSSState(std::initializer_list<CSSStateFlag> flags);
public:
    inline CSSState& set(CSSStateFlag flag, bool value = true) {
        this->_CSSStateBitset::set(size_t(flag), value);
        return *this;
    };

    inline CSSState& set(std::initializer_list<CSSStateFlag> flags) {
        for (auto flag: flags) {
            this->_CSSStateBitset::set(size_t(flag));
        }
        return *this;
    };

    inline CSSState& reset(CSSStateFlag flag) {
        this->_CSSStateBitset::reset(size_t(flag));
        return *this;
    };

    inline CSSState& reset(std::initializer_list<CSSStateFlag> flags) {
        for (auto flag: flags) {
            this->_CSSStateBitset::reset(size_t(flag));
        }
        return *this;
    };

    inline CSSState& flip(CSSStateFlag flag) {
        this->_CSSStateBitset::flip(size_t(flag));
        return *this;
    };
};

template <const char* name>
struct css_attrib_element_name
{
    css_attrib_element_name() = default;
    css_attrib_element_name(const css_attrib_element_name& ref) = default;
    css_attrib_element_name& operator=(const css_attrib_element_name& ref) = default;
    virtual ~css_attrib_element_name() {};
public:
    virtual const char* element_name() {
        return name;
    };
};

namespace {

template <typename t>
struct value_is_inherit_impl
{
    static bool check(const t &v)
    {
        return false;
    }
};

template <typename t>
struct value_is_inherit_impl<CSSInheritable<t>>
{
    static bool check(const CSSInheritable<t> &v)
    {
        return v.is_inherit();
    }
};

}

template <typename T>
inline bool value_is_inherit(const T& v)
{
    return value_is_inherit_impl<T>::check(v);
}

inline std::ostream& operator<<(std::ostream& stream, const Auto &auto_)
{
    return stream << "<css auto>";
}

}

namespace std {

template <typename T1>
inline ostream& operator<<(
    ostream& stream,
    const PyEngine::CSSInheritable<T1> &v)
{
    if (v.is_inherit()) {
        return stream << "<css inherit>";
    }
    return stream << v.get();
}

}

namespace {

template <typename T1, typename T2>
struct cmp_css_inheritable;

template <typename T1, typename T2>
struct cmp_css_inheritable<PyEngine::CSSInheritable<T1>, T2>
{
    static inline bool eq(const PyEngine::CSSInheritable<T1> &me, const T2 &oth)
    {
        return me.get() == oth;
    }

    static inline bool ne(const PyEngine::CSSInheritable<T1> &me, const T2 &oth)
    {
        return !eq(me, oth);
    }
};

template <typename T1, typename T2>
struct cmp_css_inheritable<PyEngine::CSSInheritable<T1>, PyEngine::CSSInheritable<T2>>
{
    static inline bool eq(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::CSSInheritable<T2> &oth)
    {
        if (me.is_inherit() || oth.is_inherit()) {
            return false;
        }
        return me.get() == oth.get();
    }

    static inline bool ne(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::CSSInheritable<T2> &oth)
    {
        if (me.is_inherit() || oth.is_inherit()) {
            return false;
        }
        return me.get() != oth.get();
    }
};

template <typename T1>
struct cmp_css_inheritable<PyEngine::CSSInheritable<T1>, PyEngine::inherit_t>
{
    static inline bool eq(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::inherit_t &oth)
    {
        return me.is_inherit();
    }

    static inline bool ne(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::inherit_t &oth)
    {
        return !eq(me, oth);
    }
};

template <typename T1>
struct cmp_css_inheritable<PyEngine::CSSInheritable<T1>, PyEngine::Auto>
{
    static inline bool eq(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::Auto &oth)
    {
        return me.get() == static_cast<const T1&>(oth);
    }

    static inline bool ne(const PyEngine::CSSInheritable<T1> &me,
                          const PyEngine::Auto &oth)
    {
        return !eq(me, oth);
    }
};

}

template <typename T1, typename T2>
bool operator==(const PyEngine::CSSInheritable<T1> &me, const T2 &oth)
{
    return cmp_css_inheritable<PyEngine::CSSInheritable<T1>, T2>::eq(me, oth);
}

template <typename T1, typename T2>
bool operator!=(const PyEngine::CSSInheritable<T1> &me, const T2 &oth)
{
    return cmp_css_inheritable<PyEngine::CSSInheritable<T1>, T2>::ne(me, oth);
}

#endif
