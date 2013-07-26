/**********************************************************************
File name: variadic_initializer.hpp
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
#ifndef _PYE_MATH_VARIADIC_INIT_H
#define _PYE_MATH_VARIADIC_INIT_H

namespace {

template <typename data_t, typename... other_data_ts>
struct init_array_with_data;

template <typename data_t, typename other_data_t,
    typename... other_data_ts>
struct init_array_with_data<data_t, other_data_t,
    other_data_ts...>
{
    static inline void init(
        data_t *arr,
        other_data_t value,
        other_data_ts... values)
    {
        arr[0] = value;
        init_array_with_data<data_t, other_data_ts...>::init(
            &arr[1], values...);
    }
};

template <typename data_t>
struct init_array_with_data<data_t>
{
    static inline void init(data_t*)
    {

    }
};

}

#endif
