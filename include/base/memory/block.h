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

#ifndef LILY_BASE_MEMORY_BLOCK_H
#define LILY_BASE_MEMORY_BLOCK_H

#include <base/memory/api.h>
#include <base/memory/layout.h>

#include <stdbool.h>

typedef struct MemoryBlock
{
    MemoryLayout layout;
    void *mem; // void*?
    bool is_free;
    bool can_free;
} MemoryBlock;

/**
 *
 * @brief Construct MemoryBlock type.
 */
inline CONSTRUCTOR(MemoryBlock,
                   MemoryBlock,
                   MemoryLayout layout,
                   void *mem,
                   bool can_free)
{
    return (MemoryBlock){
        .layout = layout, .mem = mem, .is_free = false, .can_free = can_free
    };
}

/**
 *
 * @brief Free MemoryBlock type.
 */
DESTRUCTOR(MemoryBlock, MemoryBlock *self, MemoryApi *api);

#endif // LILY_BASE_MEMORY_BLOCK_H
