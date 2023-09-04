/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_BASE_BOX_H
#define LILY_BASE_BOX_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/ptr_mut.h>
#include <base/ref.h>
#include <base/types.h>

#define Box(T) Box__##T

#define DEF_BOX(T)                                               \
    /* Box<T> */                                                 \
    typedef struct Box(T)                                        \
    {                                                            \
        PtrMut(T) ptr;                                           \
    }                                                            \
    Box(T);                                                      \
                                                                 \
    /**                                                          \
     *                                                           \
     * @brief Construct Box type.                                \
     */                                                          \
    CONSTRUCTOR(Box__##T, Box__##T, T v);                        \
                                                                 \
    /**                                                          \
     *                                                           \
     * @brief Implement Eq trait.                                \
     */                                                          \
    inline IMPL_FOR_EQ(                                          \
      Box__##T, Box__##T, return EQ(T, *self.ptr, *other.ptr);); \
                                                                 \
    /**                                                          \
     *                                                           \
     * @brief Implement Ref wrapper for Box(T).                  \
     */                                                          \
    DEF_REF(Box__##T);                                           \
                                                                 \
    /**                                                          \
     *                                                           \
     * @brief Free Box type.                                     \
     */                                                          \
    DESTRUCTOR(Box__##T, Ref(Box__##T) self);

#define IMPL_BOX(T)                             \
    CONSTRUCTOR(Box__##T, Box__##T, T v)        \
    {                                           \
        PtrMut(T) ptr = lily_malloc(sizeof(T)); \
                                                \
        ASSERT(ptr);                            \
        *ptr = v;                               \
                                                \
        return (Box(T)){ .ptr = ptr };          \
    }                                           \
                                                \
    DESTRUCTOR(Box__##T, Ref(Box__##T) self)    \
    {                                           \
        FREE(T, *self->ptr);                    \
        lily_free(self->ptr);                   \
    }

DEF_BOX(Int8);
DEF_BOX(Int16);
DEF_BOX(Int32);
DEF_BOX(Int64);

DEF_BOX(Uint8);
DEF_BOX(Uint16);
DEF_BOX(Uint32);
DEF_BOX(Uint64);

DEF_BOX(Isize);
DEF_BOX(Usize);

DEF_BOX(Float32);
DEF_BOX(Float64);

DEF_BOX(Uptr);

DEF_BOX(Bool);
DEF_BOX(Char);
DEF_BOX(Short);
DEF_BOX(Int);
DEF_BOX(Long);
DEF_BOX(Longlong);

DEF_BOX(Uchar);
DEF_BOX(Ushort);
DEF_BOX(Uint);
DEF_BOX(Ulong);
DEF_BOX(Ulonglong);

#endif // LILY_BASE_BOX_H
