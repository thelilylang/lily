/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_BASE_VEC_H
#define LILY_BASE_VEC_H

#include <base/macros.h>
#include <base/types.h>

#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef ENV_DEBUG
// Vec<T>
#define DEBUG_VEC_STRING(self, res, name)                        \
    push_str__String(res, " { ");                                \
    for (Usize x = 0; x < self->len; ++x) {                      \
        String *s = to_string__Debug__##name(get__Vec(self, x)); \
        APPEND_AND_FREE(res, s);                                 \
        if (x != self->len - 1) {                                \
            push_str__String(res, ", ");                         \
        }                                                        \
    }                                                            \
    push_str__String(res, " }");

// Vec<T>
#define DEBUG_VEC_STR(self, res, name)                         \
    push_str__String(res, " { ");                              \
    for (Usize x = 0; x < self->len; ++x) {                    \
        char *s = to_string__Debug__##name(get__Vec(self, x)); \
        PUSH_STR_AND_FREE(res, s);                             \
        if (x != self->len - 1) {                              \
            push_str__String(res, ", ");                       \
        }                                                      \
    }                                                          \
    push_str__String(res, " }");

// Vec<Vec<T>>
#define DEBUG_VEC_STRING_2(self, res, name)                           \
    push_str__String(res, " { ");                                     \
    for (Usize x = 0; x < self->len; ++x) {                           \
        Vec *item = get__Vec(self, x);                                \
        String *res_item = from__String("{ ");                        \
        for (Usize x2 = 0; x2 < item->len; x2++) {                    \
            String *s = to_string__Debug__##name(get__Vec(item, x2)); \
            APPEND_AND_FREE(res_item, s);                             \
            if (x2 != self->len - 1) {                                \
                push_str__String(res_item, ", ");                     \
            }                                                         \
        }                                                             \
        push_str__String(res_item, " }");                             \
        APPEND_AND_FREE(res, res_item);                               \
        if (x != self->len - 1) {                                     \
            push_str__String(res, ", ");                              \
        }                                                             \
    }                                                                 \
    push_str__String(res, " }");

// Vec<Vec<T>>
#define DEBUG_VEC_STR_2(self, res, name)                            \
    push_str__String(res, " { ");                                   \
    for (Usize x = 0; x < self->len; ++x) {                         \
        Vec *item = get__Vec(self, x);                              \
        String *res_item = from__String("{ ");                      \
        for (Usize x2 = 0; x2 < item->len; x2++) {                  \
            char *s = to_string__Debug__##name(get__Vec(item, x2)); \
            PUSH_STR_AND_FREE(res_item, s);                         \
            if (x2 != self->len - 1) {                              \
                push_str__String(res_item, ", ");                   \
            }                                                       \
        }                                                           \
        push_str__String(res_item, " }");                           \
        APPEND_AND_FREE(res, res_item);                             \
        if (x != self->len - 1) {                                   \
            push_str__String(res, ", ");                            \
        }                                                           \
    }                                                               \
    push_str__String(res, " }");
#endif

typedef struct String String;

typedef struct Vec
{
    void **buffer; // void**?
    Usize len;
    Usize capacity;
    Usize default_capacity;
} Vec;

/**
 *
 * @brief Construct Vec type.
 */
CONSTRUCTOR(Vec *, Vec);

/**
 *
 * @brief Append to the buffer an other vector.
 */
void
append__Vec(Vec *self, const Vec *other);

/**
 *
 * @brief Return true if the Vec contains an element with the given String.
 * @param self Vec<String*>*
 */
bool
contains__Vec(const Vec *self, const String *s);

/**
 *
 * @brief Construct Vec type with default buffer items.
 */
Vec *
from__Vec(void **buffer, Usize len);

/**
 *
 * @brief Get item from Vec.
 */
void *
get__Vec(const Vec *self, Usize index);

/**
 *
 * @brief Grow Vec buffer.
 * @param new_capacity New buffer capacity.
 */
void
grow__Vec(Vec *self, Usize new_capacity);

/**
 *
 * @brief Initialize Vec with `...` parameter.
 */
Vec *
init__Vec(Usize len, ...);

/**
 *
 * @brief Alternative version of the `init__Vec` function, to pass the argument
 * directly.
 */
Vec *
vinit__Vec(Usize len, va_list arg);

/**
 *
 * @brief Insert an item at n index.
 */
void
insert__Vec(Vec *self, void *item, Usize index);

/**
 *
 * @brief Insert an item at n + 1 index.
 */
void
insert_after__Vec(Vec *self, void *item, Usize index);

/**
 *
 * @brief Join each item with c param.
 * @param self Vec<String*>*
 */
String *
join__Vec(const Vec *self, char c);

/**
 *
 * @brief Get the last item of the Vec.
 */
void *
last__Vec(const Vec *self);

/**
 *
 * @brief Get the last item of the Vec.
 * @return void*? (&)
 */
inline void *
safe_last__Vec(const Vec *self)
{
    return self->len > 0 ? self->buffer[self->len - 1] : NULL;
}

/**
 *
 * @brief Remove the last item from Vec.
 */
void *
pop__Vec(Vec *self);

/**
 *
 * @brief Push item to Vec buffer.
 */
void
push__Vec(Vec *self, void *item);

/**
 *
 * @brief Remove item from Vec buffer.
 */
void *
remove__Vec(Vec *self, Usize index);

/**
 *
 * @brief Replace item of the Vector at index position.
 */
void
replace__Vec(Vec *self, Usize index, void *item);

/**
 *
 * @brief Reverse Vec buffer.
 */
void
reverse__Vec(Vec *self);

/**
 *
 * @brief Get item from Vec (no assert).
 * @return void*?
 */
inline void *
safe_get__Vec(const Vec *self, Usize index)
{
    return index < self->len ? self->buffer[index] : NULL;
}

/**
 *
 * @brief Take a slice of the vector.
 */
Vec *
slice__Vec(Vec *self, Usize start, Usize end);

/**
 *
 * @brief Ungrow Vec buffer.
 * @param new_capacity New buffer capacity.
 */
void
ungrow__Vec(Vec *self);

/**
 *
 * @brief Free Vec type.
 */
DESTRUCTOR(Vec, Vec *self);

typedef struct VecIter
{
    const Vec *vec;
    Usize count;
} VecIter;

inline CONSTRUCTOR(VecIter, VecIter, const Vec *vec)
{
    return (VecIter){ .vec = vec, .count = 0 };
}

/**
 *
 * @brief Get the next value.
 */
void *
next__VecIter(VecIter *self);

/**
 *
 * @brief Get the current value.
 */
void *
current__VecIter(VecIter *self);

#endif // LILY_BASE_VEC_H
