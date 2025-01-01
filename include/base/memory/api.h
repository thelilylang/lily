/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_BASE_MEMORY_API_H
#define LILY_BASE_MEMORY_API_H

// NOTE: For the moment, we'll be using C's memory management API, until our API
// is stabilized and there are no more bugs.
#define USE_C_MEMORY_API
// #undef USE_C_MEMORY_API

#include <base/macros.h>
#include <base/types.h>
#include <builtin/alloc.h>

#ifdef USE_C_MEMORY_API
#include <stdlib.h>
#endif

/**
 *
 * @brief Align pointer with the given size alignment.
 */
void *
__align__(void *mem, Usize align);

/**
 *
 * @brief Alloc to a pointer with the given size + align the allocated pointer.
 */
void *
__alloc__(Usize size, Usize align);

/**
 *
 * @brief Resize the given pointer with the new_size + align the resized
 * pointer.
 */
void *
__resize__(void *old_mem, Usize old_size, Usize new_size, Usize align);

/**
 *
 * @brief Free the given pointer according the given size and alignment.
 * @note The given freed pointer is assigned to NULL at the end.
 */
void
__free__(void **mem, Usize size, Usize align);

typedef struct MemoryApi
{
    void *(*align)(void *, Usize);
    void *(*alloc)(Usize, Usize);
    void *(*resize)(void *, Usize, Usize, Usize);
    void (*free)(void **, Usize, Usize);
} MemoryApi;

/**
 *
 * @brief Construct MemoryApi type.
 */
inline CONSTRUCTOR(MemoryApi, MemoryApi)
{
    return (MemoryApi){ .align = &__align__,
                        .alloc = &__alloc__,
                        .resize = &__resize__,
                        .free = &__free__ };
}

#endif // LILY_BASE_MEMORY_API_H
