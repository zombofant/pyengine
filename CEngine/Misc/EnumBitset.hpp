/**********************************************************************
File name: EnumBitset.hpp
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
#ifndef _PYE_MISC_ENUM_BITSET_H
#define _PYE_MISC_ENUM_BITSET_H

#include <bitset>

namespace PyEngine {

template <typename enum_t, enum_t min_value, enum_t max_value>
class EnumBitset
{
public:
    typedef std::bitset<(max_value - min_value)+1> bitset_t;
    typedef typename bitset_t::reference reference;

public:
    EnumBitset():
        _data()
    {

    };

    EnumBitset(const EnumBitset &ref):
        _data(ref._data)
    {

    };

    EnumBitset& operator=(const EnumBitset &ref)
    {
        _data = ref._data;
        return *this;
    };

protected:
    EnumBitset(bitset_t &&init):
        _data(std::move(init))
    {

    };

private:
    bitset_t _data;

public:
    bool operator==(const EnumBitset &oth) const
    {
        return _data == oth._data;
    }

    bool operator!=(const EnumBitset &oth) const
    {
        return _data != oth._data;
    }

    inline reference operator[](const enum_t value)
    {
        return _data[value-min_value];
    }

    inline bool test(const enum_t value) const
    {
        return _data.test(value-min_value);
    }

    inline bool all() const
    {
        return _data.all();
    }

    inline bool any() const
    {
        return _data.any();
    }

    inline bool none() const
    {
        return _data.none();
    }

    inline size_t count() const
    {
        return _data.count();
    }

    inline EnumBitset& operator&=(const EnumBitset &oth)
    {
        _data &= oth._data;
        return *this;
    }

    inline EnumBitset operator&(const EnumBitset &oth) const
    {
        EnumBitset result(*this);
        result &= oth;
        return result;
    }

    inline EnumBitset& operator|=(const EnumBitset &oth)
    {
        _data |= oth._data;
        return *this;
    }

    inline EnumBitset operator|(const EnumBitset &oth) const
    {
        EnumBitset result(*this);
        result |= oth;
        return result;
    }

    inline EnumBitset& operator^=(const EnumBitset &oth)
    {
        _data ^= oth._data;
    }

    inline EnumBitset operator^(const EnumBitset &oth) const
    {
        EnumBitset result(*this);
        result ^= oth;
        return result;
    }

    inline EnumBitset operator~() const
    {
        return EnumBitset(std::move(~_data));
    }

    inline EnumBitset& set()
    {
        _data.set();
        return *this;
    }

    inline EnumBitset& set(enum_t bit, bool value = true)
    {
        _data.set(bit-min_value, value);
        return *this;
    }
    inline EnumBitset& reset()
    {
        _data.reset();
        return *this;
    }

    inline EnumBitset& reset(enum_t bit)
    {
        _data.reset(bit-min_value);
        return *this;
    }

    inline EnumBitset& flip()
    {
        _data.flip();
        return *this;
    }

    inline EnumBitset& flip(enum_t bit)
    {
        _data.flip(bit-min_value);
        return *this;
    }

    std::string to_string() const
    {
        return _data.to_string();
    }

    unsigned long to_ulong() const
    {
        return _data.to_ulong();
    }

    unsigned long long to_ullong() const
    {
        return _data.to_ullong();
    }

    friend struct std::hash<EnumBitset>;
};

}

namespace std {

template <typename enum_t, enum_t min_value, enum_t max_value>
struct hash<PyEngine::EnumBitset<enum_t, min_value, max_value> >
{
    typedef PyEngine::EnumBitset<enum_t, min_value, max_value>
        argument_type;
    typedef size_t result_type;

    inline result_type operator()(const argument_type &ref) const
    {
        return std::hash<typename argument_type::bitset_t>()(ref._data);
    }
};

}

#endif
