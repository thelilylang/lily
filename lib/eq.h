#ifndef C_COMPTIME_GEN_EQ_H
#define C_COMPTIME_GEN_EQ_H

#include <stdbool.h>

#include "types.h"

#define EQ(type, x, y) __eq__##type##__(x, y)
#define IMPL_EQ(type) bool __eq__##type##__(const type *x, const type *y)

inline IMPL_EQ(i8)
{
    return *x == *y;
}

inline IMPL_EQ(i16)
{
    return *x == *y;
}

inline IMPL_EQ(i32)
{
    return *x == *y;
}

inline IMPL_EQ(i64)
{
    return *x == *y;
}

inline IMPL_EQ(isize)
{
    return *x == *y;
}

inline IMPL_EQ(u8)
{
    return *x == *y;
}

inline IMPL_EQ(u16)
{
    return *x == *y;
}

inline IMPL_EQ(u32)
{
    return *x == *y;
}

inline IMPL_EQ(u64)
{
    return *x == *y;
}

inline IMPL_EQ(usize)
{
    return *x == *y;
}

inline IMPL_EQ(f32)
{
    return *x == *y;
}

inline IMPL_EQ(f64)
{
    return *x == *y;
}

inline IMPL_EQ(uptr)
{
    return *x == *y;
}

inline IMPL_EQ(str)
{
    return *x == *y;
}

inline IMPL_EQ(char)
{
    return *x == *y;
}

inline IMPL_EQ(bool)
{
    return *x == *y;
}

#endif // C_COMPTIME_GEN_EQ_H