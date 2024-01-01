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

#ifndef LILY_BASE_MEMORY_PAGE_H
#define LILY_BASE_MEMORY_PAGE_H

#include <base/eq.h>
#include <base/memory/api.h>
#include <base/memory/block.h>
#include <base/new.h>
#include <base/ref_mut.h>
#include <base/types.h>

#define MEMORY_PAGE_ALLOC(T, self, n) \
    alloc__MemoryPage(self, sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

#define MEMORY_PAGE_RESIZE(T, self, n) resize__MemoryPage(self, sizeof(T) * n)

#define MEMORY_PAGE_FREE(self) free__MemoryPage(self)

typedef struct MemoryPage
{
    Anyptr mem; // undef
    Bool is_undef;
} MemoryPage;

/**
 *
 * @brief Impl Eq trait for MemoryPage type.
 */
inline DEF_FOR_EQ(MemoryPage, MemoryPage);
inline IMPL_FOR_EQ(MemoryPage,
                   MemoryPage,
                   return self.is_undef && other.is_undef
                            ? true
                            : self.mem == other.mem;);

/**
 *
 * @brief Impl RefMut wrapper for MemoryPage type.
 */
DEF_REF_MUT(MemoryPage);

/**
 *
 * @brief Construct MemoryPage type.
 */
inline CONSTRUCTOR(MemoryPage, MemoryPage)
{
    return (MemoryPage){ .is_undef = true };
}

/**
 *
 * @brief Create a new allocation.
 */
Anyptr
alloc__MemoryPage(RefMut(MemoryPage) self, Usize size, Usize align);

/**
 *
 * @brief Resize a block of memory.
 */
Anyptr
resize__MemoryPage(RefMut(MemoryPage) self, Usize new_size);

/**
 *
 * @brief Destroy Page allocator.
 */
void free__MemoryPage(RefMut(MemoryPage) self);

#endif // LILY_BASE_MEMORY_PAGE_H
