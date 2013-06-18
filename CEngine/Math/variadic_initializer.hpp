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
    static inline void init(
        data_t *arr)
    {

    }
};

}

#endif
