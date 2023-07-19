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

#ifndef LILY_BASE_MEMORY_PAGE_H
#define LILY_BASE_MEMORY_PAGE_H

#include <base/memory/api.h>
#include <base/memory/block.h>
#include <base/new.h>

#define MEMORY_PAGE_ALLOC(T, self, n) \
    alloc__MemoryPage(self, sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

#define MEMORY_PAGE_RESIZE(T, self, n) resize__MemoryPage(self, sizeof(T) * n)

#define MEMORY_PAGE_FREE(self) free__MemoryPage(self)

typedef struct MemoryPage
{
    MemoryApi api;
    MemoryBlock block; // undef
    bool is_undef;
} MemoryPage;

/**
 *
 * @brief Construct MemoryPage type.
 */
inline CONSTRUCTOR(MemoryPage, MemoryPage)
{
    return (MemoryPage){ .api = NEW(MemoryApi), .is_undef = true };
}

/**
 *
 * @brief Create a new allocation.
 */
MemoryBlock *
alloc__MemoryPage(MemoryPage *self, Usize size, Usize align);

/**
 *
 * @brief Resize a block of memory.
 */
MemoryBlock *
resize__MemoryPage(MemoryPage *self, Usize new_size);

/**
 *
 * @brief Free a block of memory.
 */
void
free__MemoryPage(MemoryPage *self);

/**
 *
 * @brief Free MemoryPage type.
 */
inline DESTRUCTOR(MemoryPage, MemoryPage *self)
{
    FREE(MemoryBlock, &self->block, &self->api);
}

#endif // LILY_BASE_MEMORY_PAGE_H
