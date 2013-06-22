/**********************************************************************
File name: SetOperations.hpp
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
#ifndef _PYE_MISC_SET_OPERATIONS_H
#define _PYE_MISC_SET_OPERATIONS_H

#include <set>

namespace PyEngine {

template <typename InputIterator, typename set_type>
struct SetDifferenceIterator
{
    typedef typename std::iterator_traits<InputIterator>::value_type value_type;
    typedef typename std::iterator_traits<InputIterator>::pointer pointer;
    typedef typename std::iterator_traits<InputIterator>::reference reference;
    typedef std::input_iterator_tag iterator_category;

    SetDifferenceIterator():
        _aiter(),
        _aend(),
        _b(),
        _bend()
    {

    }

    SetDifferenceIterator(InputIterator start, InputIterator end,
                          const set_type &b):
        _aiter(start),
        _aend(end),
        _b(&b),
        _bend(_b->cend())
    {
        // if the current iterator position isn't a hit, we need to
        // move forward until we find what we need.
        if (_b->find(*_aiter) != _bend) {
            ++(*this);
        }
    }

    SetDifferenceIterator(const SetDifferenceIterator &ref):
        _aiter(ref._aiter),
        _aend(ref._aend),
        _b(ref._b),
        _bend(ref._bend)
    {

    }

    SetDifferenceIterator& operator=(const SetDifferenceIterator &ref)
    {
        _aiter = ref._aiter;
        _aend = ref._aend;
        _b = ref._b;
        _bend = ref._bend;
        return *this;
    }

private:
    InputIterator _aiter;
    InputIterator _aend;
    const set_type *_b;
    typename set_type::const_iterator _bend;

public:
    reference operator*()
    {
        return *_aiter;
    }

    reference operator->()
    {
        return *_aiter;
    }

    SetDifferenceIterator& operator++()
    {
        while (_aiter != _aend) {
            ++_aiter;
            if (_b->find(*_aiter) == _bend) {
                break;
            }
        }
        if (_aiter == _aend) {
            _aiter = InputIterator();
            _aend = InputIterator();
        }
        return *this;
    }

    SetDifferenceIterator operator++(int) const
    {
        SetDifferenceIterator result(*this);
        ++(*this);
        return result;
    }

    bool operator==(const SetDifferenceIterator &oth) const
    {
        return (_aiter == oth._aiter);
    }

    bool operator!=(const SetDifferenceIterator &oth) const
    {
        return !(*this == oth);
    }
};

template <typename value_type, typename OutputIterator>
OutputIterator set_difference(
    const std::set<value_type> &a,
    const std::set<value_type> &b,
    OutputIterator out)
{
    typedef std::set<value_type> set_t;
    typedef SetDifferenceIterator<
        typename set_t::const_iterator,
        set_t> iterator_t;
    typename set_t::const_iterator end_of_b = b.end();

    iterator_t iter(a.begin(), a.end(), b);
    for (; iter != iterator_t(); ++iter) {
        *out++ = *iter;
    }
}

template <typename InputIt, typename set_type>
SetDifferenceIterator<InputIt, set_type>
set_difference_iterator(InputIt begin, InputIt end, const set_type &set)
{
    return SetDifferenceIterator<InputIt, set_type>(begin, end, set);
}

template <typename set_t1, typename set_t2>
SetDifferenceIterator<typename set_t1::const_iterator, set_t2>
set_difference_iterator(const set_t1 &set1, const set_t2 &set2)
{
    return SetDifferenceIterator<
        typename set_t1::const_iterator, set_t2>(
            set1.cbegin(), set1.cend(), set2);
}

}

namespace std {
template <typename value_type>
std::ostream& operator<<(std::ostream& stream,
                         const std::set<value_type> &set)
{
    stream << "set{";
    auto it = set.cbegin();
    stream << *it++;
    while (it != set.cend()) {
        stream << ", " << *it++;
    }
    stream << "}";
    return stream;
}


}

#endif
