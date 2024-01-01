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

#ifndef LILY_BASE_MEMORY_ARENA_H
#define LILY_BASE_MEMORY_ARENA_H

#include <base/eq.h>
#include <base/memory/api.h>
#include <base/memory/block.h>
#include <base/memory/global.h>
#include <base/new.h>
#include <base/ref.h>
#include <base/ref_mut.h>
#include <base/types.h>

#define MEMORY_ARENA_ALLOC(T, self, n) \
    alloc__MemoryArena(self, sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

#define MEMORY_ARENA_RESIZE(T, self, block, n) \
    resize__MemoryArena(                       \
      self, block, sizeof(T) * n, alignof(T) * ALIGNMENT_COEFF)

typedef struct MemoryArena
{
    Anyptr arena;
    Usize total_size;
    Usize capacity;
    Bool is_destroy;
} MemoryArena;

/**
 *
 * @brief Construct MemoryArena type.
 */
inline CONSTRUCTOR(MemoryArena, MemoryArena, Usize capacity)
{
    return (MemoryArena){
        .arena = alloc__MemoryGlobal(capacity, DEFAULT_ALIGNMENT),
        .total_size = 0,
        .capacity = capacity,
        .is_destroy = false,
    };
}

/**
 *
 * @brief Impl Eq trait.
 */
inline DEF_FOR_EQ(MemoryArena, MemoryArena);
inline IMPL_FOR_EQ(MemoryArena,
                   MemoryArena,
                   return self.arena == other.arena &&
                          self.total_size == other.total_size &&
                          self.capacity == other.capacity &&
                          self.is_destroy == other.is_destroy);

/**
 *
 * @brief Impl Ref and RefMut wrapper for MemoryArena type.
 */
DEF_REF(MemoryArena);
DEF_REF_MUT(MemoryArena);

/**
 *
 * @brief Reserve region of the Arena.
 */
Anyptr
alloc__MemoryArena(RefMut(MemoryArena) self, Usize size, Usize align);

/**
 *
 * @brief Resize region of the Arena.
 */
Anyptr
resize__MemoryArena(RefMut(MemoryArena) self,
                    Anyptr mem,
                    Usize new_size,
                    Usize align);

/**
 *
 * @brief Free all allocated memory by the arena Allocator.
 */
void destroy__MemoryArena(RefMut(MemoryArena) self);

/**
 *
 * @brief Free all allocated memory by the arena Allocator and reset the
 * allocator.
 */
void reset__MemoryArena(RefMut(MemoryArena) self);

/**
 *
 * @brief Print the stats of the arena Allocator.
 */
void print_stat__MemoryArena(Ref(MemoryArena) self);

#endif // LILY_BASE_MEMORY_ARENA_H
