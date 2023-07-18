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
#include <base/memory/layout.h>
#include <base/new.h>
#include <base/types.h>

#include <builtin/alloc.h>

#include <stddef.h>

typedef struct MemoryGlobalCell
{
    MemoryLayout layout;
    struct MemoryGlobalCell *next; // struct MemoryGlobalCell*?
} MemoryGlobalCell;

/**
 *
 * @brief Construct MemoryGlobalCell type.
 */
inline CONSTRUCTOR(MemoryGlobalCell *, MemoryGlobalCell, void *mem, Usize size)
{
    return &(MemoryGlobalCell){ .layout = NEW(MemoryLayout, mem, size),
                                .next = NULL };
}

/**
 *
 * @brief Free MemoryGlobalCell type.
 */
inline DESTRUCTOR(MemoryGlobalCell, MemoryGlobalCell *self)
{
    FREE(MemoryLayout, &self->layout);
}

typedef struct MemoryGlobal
{
    MemoryGlobalCell *cells;      // MemoryGlobalCell*?
    MemoryGlobalCell *last_cells; // MemoryGlobalCell*?
    Usize total_size;
    Usize total_cell;
    Usize total_cell_free;
    Usize total_size_free;
    Usize capacity;
} MemoryGlobal;

/**
 *
 * @brief Construct MemoryGlobal type.
 */
inline CONSTRUCTOR(MemoryGlobal, MemoryGlobal)
{
    return (MemoryGlobal){
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
MemoryLayout *
alloc__MemoryGlobal(MemoryGlobal *self, Usize size);

/**
 *
 * @brief Resize a previous allocated mem.
 */
MemoryLayout *
resize__MemoryGlobal(MemoryGlobal *self, MemoryLayout *layout, Usize new_size);

/**
 *
 * @brief Free a layout.
 */
void
free__MemoryGlobal(MemoryGlobal *self, MemoryLayout *layout);

/**
 *
 * @brief Free MemoryGlobal type.
 */
DESTRUCTOR(MemoryGlobal, MemoryGlobal *self);

#endif // LILY_BASE_MEMORY_GLOBAL_H
