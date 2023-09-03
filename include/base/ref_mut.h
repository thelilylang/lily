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

#ifndef LILY_BASE_REF_MUT_NON_NULL_H
#define LILY_BASE_REF_MUT_NON_NULL_H

#include <base/assert.h>
#include <base/ptr_mut.h>

#include <stdio.h>
#include <stdlib.h>

#define Ref(T)                                                  \
    /* RefMut<T> */                                             \
    typedef struct RefMut__##T                                  \
    {                                                           \
        PtrMut(T) v; /* T* (&) */                               \
    } RefMut__##T;                                              \
                                                                \
    /**                                                         \
     *                                                          \
     * @brief Construct RefMut type.                            \
     */                                                         \
    inline CONSTRUCTOR(RefMut__##T, RefMut__##T, PtrMut(T) v)   \
    {                                                           \
        ASSERT(v);                                              \
        return (RefMut__##T){ .v = v };                         \
    }                                                           \
                                                                \
    /**                                                         \
     *                                                          \
     * @brief Unwrap the pointer of the mutable reference.      \
     */                                                         \
    inline PtrMut(T) unwrap__RefMut__##T(Ref(RefMut__##T) self) \
    {                                                           \
        ASSERT(self);                                           \
        return self->v;                                         \
    }

#define REF_MUT(T, v) NEW(RefMut__##T, v)

#endif // LILY_BASE_REF_MUT_NON_NULL_H
