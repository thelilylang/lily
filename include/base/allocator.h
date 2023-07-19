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

#ifndef LILY_BASE_ALLOCATOR_H
#define LILY_BASE_ALLOCATOR_H

#include <base/memory/api.h>
#include <base/memory/arena.h>
#include <base/memory/global.h>
#include <base/memory/page.h>
#include <base/memory/pool.h>

#define API_ALLOCATOR() NEW_VARIANT(Allocator, api)

#define ARENA_ALLOCATOR() NEW_VARIANT(Allocator, arena)

#define GLOBAL_ALLOCATOR() NEW_VARIANT(Allocator, global)

enum AllocatorKind
{
    ALLOCATOR_KIND_ARENA,
    ALLOCATOR_KIND_GLOBAL,
    ALLOCATOR_KIND_PAGE,
    ALLOCATOR_KIND_POOL,
};

typedef struct Allocator
{
    enum AllocatorKind kind;
    union
    {
        MemoryArena arena;
        MemoryGlobal global;
    };
} Allocator;

/**
 *
 * @brief Construct Allocator type (ALLOCATOR_KIND_ARENA).
 */
inline VARIANT_CONSTRUCTOR(Allocator, Allocator, arena)
{
    return (Allocator){ .kind = ALLOCATOR_KIND_ARENA,
                        .arena = NEW(MemoryArena) };
}

/**
 *
 * @brief Construct Allocator type (ALLOCATOR_KIND_GLOBAL).
 */
inline VARIANT_CONSTRUCTOR(Allocator, Allocator, global)
{
    return (Allocator){ .kind = ALLOCATOR_KIND_GLOBAL,
                        .global = NEW(MemoryGlobal) };
}

#define ALLOC(T, a, n)                                           \
    ({                                                           \
        MemoryBlock *_block = NULL;                              \
                                                                 \
        switch ((a).kind) {                                      \
            case ALLOCATOR_KIND_ARENA:                           \
                _block = MEMORY_ARENA_ALLOC(T, &(a).arena, n);   \
                break;                                           \
            case ALLOCATOR_KIND_GLOBAL:                          \
                _block = MEMORY_GLOBAL_ALLOC(T, &(a).global, n); \
                break;                                           \
            case ALLOCATOR_KIND_PAGE:                            \
                break;                                           \
            case ALLOCATOR_KIND_POOL:                            \
                break;                                           \
            default:                                             \
                UNREACHABLE("unknown variant");                  \
        }                                                        \
                                                                 \
        _block;                                                  \
    })

/**
 *
 * @brief Destroy Allocator type.
 */
void
destroy__Allocator(Allocator *self);

#endif // LILY_BASE_ALLOCATOR_H
