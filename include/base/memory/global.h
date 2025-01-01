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

#ifndef LILY_BASE_MEMORY_GLOBAL_H
#define LILY_BASE_MEMORY_GLOBAL_H

#include <base/macros.h>
#include <base/memory/api.h>
#include <base/memory/block.h>
#include <base/new.h>
#include <base/types.h>

#include <builtin/alloc.h>

#include <stdbool.h>
#include <stddef.h>

#define MEMORY_GLOBAL_ALLOC(T, n) \
    alloc__MemoryGlobal(sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

#define MEMORY_GLOBAL_RESIZE(T, mem, n) resize__MemoryGlobal(mem, sizeof(T) * n)

#define MEMORY_GLOBAL_FREE(mem) free__MemoryGlobal(mem)

/**
 *
 * @brief Create a new cell for a new allocation.
 */
void *
alloc__MemoryGlobal(Usize size, Usize align);

/**
 *
 * @brief Resize a previous allocated mem.
 */
void *
resize__MemoryGlobal(void *mem, Usize new_size);

/**
 *
 * @brief Free a block of memory.
 */
void
free__MemoryGlobal(void *mem);

/**
 *
 * @brief Print the stats of the global Allocator.
 */
void
print_stat__MemoryGlobal();

#endif // LILY_BASE_MEMORY_GLOBAL_H
