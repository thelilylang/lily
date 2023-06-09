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

#ifndef LILY_BUILTIN_ALLOC_H
#define LILY_BUILTIN_ALLOC_H

#include <base/types.h>

#include <api.h>

#if defined(__cplusplus)
extern "C"
{
#endif

    typedef struct Block
    {
        Usize size;
        int is_free;
        int is_used;
        void *ptr;
    } Block;

    typedef struct HeapChild
    {
        Block block;
        volatile struct HeapChild *next;
        volatile struct HeapChild *prev;
    } HeapChild;

    typedef struct Heap
    {
        HeapChild head;
        HeapChild *tail;
        Usize capacity;
        Usize total_size;
        Usize total_block;
        Usize total_free;
    } Heap;

    LILY_API void __init__$Alloc();

    LILY_API Block *__align__$Alloc(void *ptr, Usize align);

    LILY_API volatile Block *__alloc__$Alloc(Usize size, Usize align);

    LILY_API volatile Block *__resize__$Alloc(volatile Block *block,
                                              Usize new_size,
                                              Usize align);

    LILY_API void __free__$Alloc(volatile Block *block);

    LILY_API void *__get_ptr__$Alloc(volatile Block *block);

#if defined(__cplusplus)
}
#endif

#endif // LILY_BUILTIN_ALLOC_H
