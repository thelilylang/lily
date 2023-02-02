#include <base/atoi.h>

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define __atoi__(type, s)                  \
    type res = 0;                          \
    int i = 0;                             \
    bool is_neg = false;                   \
    if (s[i] == '-') {                     \
        i++;                               \
        is_neg = true;                     \
    }                                      \
    while (s[i]) {                         \
        if (is_neg)                        \
            res = res * 10 - (s[i] - '0'); \
        else                               \
            res = res * 10 + (s[i] - '0'); \
        i++;                               \
    }                                      \
    return res

Int8
atoi__Int8(const char *s)
{
    __atoi__(Int8, s);
}

Int16
atoi__Int16(const char *s)
{
    __atoi__(Int16, s);
}

Int32
atoi__Int32(const char *s)
{
    __atoi__(Int32, s);
}

Int64
atoi__Int64(const char *s)
{
    __atoi__(Int64, s);
}

Uint8
atoi__Uint8(const char *s)
{
    __atoi__(Uint8, s);
}

Uint16
atoi__Uint16(const char *s)
{
    __atoi__(Uint16, s);
}

Uint32
atoi__Uint32(const char *s)
{
    __atoi__(Uint32, s);
}

Uint64
atoi__Uint64(const char *s)
{
    __atoi__(Uint64, s);
}

Isize
atoi__Isize(const char *s)
{
    __atoi__(Isize, s);
}

Usize
atoi__Usize(const char *s)
{
    __atoi__(Usize, s);
}
