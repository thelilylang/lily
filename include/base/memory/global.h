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

#ifndef LILY_BASE_MEMORY_GLOBAL_H
#define LILY_BASE_MEMORY_GLOBAL_H

#include <base/macros.h>
#include <base/memory/api.h>
#include <base/memory/block.h>
#include <base/new.h>
#include <base/types.h>

#include <builtin/alloc.h>

#include <stddef.h>

#define MEMORY_GLOBAL_ALLOC(T, self, n) \
    alloc__MemoryGlobal(self, sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

#define MEMORY_GLOBAL_RESIZE(T, self, block, n) \
    resize__MemoryGlobal(self, block, sizeof(T) * n)

#define MEMORY_GLOBAL_FREE(self, block) free__MemoryGlobal(self, block)

typedef struct MemoryGlobalCell
{
    MemoryBlock block;
    struct MemoryGlobalCell *next; // struct MemoryGlobalCell*?
} MemoryGlobalCell;

/**
 *
 * @brief Construct MemoryGlobalCell type.
 */
CONSTRUCTOR(MemoryGlobalCell *, MemoryGlobalCell, MemoryBlock block);

/**
 *
 * @brief Free MemoryGlobalCell type.
 */
inline DESTRUCTOR(MemoryGlobalCell, MemoryGlobalCell *self, MemoryApi *api)
{
    FREE(MemoryBlock, &self->block, api);
}

typedef struct MemoryGlobal
{
    MemoryApi api;
    MemoryGlobalCell *cells;      // MemoryGlobalCell*?
    MemoryGlobalCell *last_cells; // MemoryGlobalCell*?
    Usize total_size;
    Usize total_cell;
    Usize total_cell_free;
    Usize total_size_free;
    Usize capacity;
    bool is_destroy;
} MemoryGlobal;

/**
 *
 * @brief Construct MemoryGlobal type.
 */
inline CONSTRUCTOR(MemoryGlobal, MemoryGlobal)
{
    return (MemoryGlobal){
        .api = NEW(MemoryApi),
        .cells = NULL,
        .last_cells = NULL,
        .total_size = 0,
        .total_cell = 0,
        .total_cell_free = 0,
        .total_size_free = 0,
        .capacity = __max_capacity__$Alloc(),
    };
}

/**
 *
 * @brief Create a new cell for a new allocation.
 */
MemoryBlock *
alloc__MemoryGlobal(MemoryGlobal *self, Usize size, Usize align);

/**
 *
 * @brief Resize a previous allocated mem.
 */
MemoryBlock *
resize__MemoryGlobal(MemoryGlobal *self, MemoryBlock *block, Usize new_size);

/**
 *
 * @brief Free a layout.
 */
void
free__MemoryGlobal(MemoryGlobal *self, MemoryBlock *block);

/**
 *
 * @brief Print the stats of the global Allocator.
 */
void
print_stat__MemoryGlobal(const MemoryGlobal *self);

/**
 *
 * @brief Destroy global Allocator.
 */
void
destroy__MemoryGlobal(MemoryGlobal *self);

#endif // LILY_BASE_MEMORY_GLOBAL_H
