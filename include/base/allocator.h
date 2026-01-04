/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <base/memory/arena.h>
#include <base/memory/global.h>
#include <base/memory/page.h>

#define ARENA_ALLOCATOR(capacity) NEW_VARIANT(Allocator, arena, capacity)

#define GLOBAL_ALLOCATOR() NEW_VARIANT(Allocator, global)

#define PAGE_ALLOCATOR() NEW_VARIANT(Allocator, page)

enum AllocatorKind
{
    ALLOCATOR_KIND_ARENA,
    ALLOCATOR_KIND_GLOBAL,
    ALLOCATOR_KIND_PAGE,
};

typedef struct Allocator
{
    enum AllocatorKind kind;
    union
    {
        MemoryArena arena;
        MemoryPage page;
    };
} Allocator;

/**
 *
 * @brief Construct Allocator type (ALLOCATOR_KIND_ARENA).
 */
inline VARIANT_CONSTRUCTOR(Allocator, Allocator, arena, Usize capacity)
{
    return (Allocator){ .kind = ALLOCATOR_KIND_ARENA,
                        .arena = NEW(MemoryArena, capacity) };
}

/**
 *
 * @brief Construct Allocator type (ALLOCATOR_KIND_GLOBAL).
 */
inline VARIANT_CONSTRUCTOR(Allocator, Allocator, global)
{
    return (Allocator){ .kind = ALLOCATOR_KIND_GLOBAL };
}

/**
 *
 * @brief Construct Allocator type (ALLOCATOR_KIND_PAGE).
 */
inline VARIANT_CONSTRUCTOR(Allocator, Allocator, page)
{
    return (Allocator){ .kind = ALLOCATOR_KIND_PAGE, .page = NEW(MemoryPage) };
}

#define A_ALLOC(T, a, n)                                     \
    ({                                                       \
        void *_mem = NULL;                                   \
                                                             \
        switch ((a).kind) {                                  \
            case ALLOCATOR_KIND_ARENA:                       \
                _mem = MEMORY_ARENA_ALLOC(T, &(a).arena, n); \
                break;                                       \
            case ALLOCATOR_KIND_GLOBAL:                      \
                _mem = MEMORY_GLOBAL_ALLOC(T, n);            \
                break;                                       \
            case ALLOCATOR_KIND_PAGE:                        \
                _mem = MEMORY_PAGE_ALLOC(T, &(a).page, n);   \
                break;                                       \
            default:                                         \
                UNREACHABLE("unknown variant");              \
        }                                                    \
                                                             \
        _mem;                                                \
    })

#define A_RESIZE(T, a, m, n)                          \
    switch ((a).kind) {                               \
        case ALLOCATOR_KIND_ARENA:                    \
            MEMORY_ARENA_RESIZE(T, &(a).arena, m, n); \
            break;                                    \
        case ALLOCATOR_KIND_GLOBAL:                   \
            m = MEMORY_GLOBAL_RESIZE(T, m, n);        \
            break;                                    \
        case ALLOCATOR_KIND_PAGE:                     \
            MEMORY_PAGE_RESIZE(T, &(a).page, n);      \
            break;                                    \
        default:                                      \
            UNREACHABLE("unknown variant");           \
    }

#define A_FREE(a, mem)                      \
    switch ((a).kind) {                     \
        case ALLOCATOR_KIND_ARENA:          \
            break;                          \
        case ALLOCATOR_KIND_GLOBAL:         \
            MEMORY_GLOBAL_FREE(mem);        \
            break;                          \
        case ALLOCATOR_KIND_PAGE:           \
            MEMORY_PAGE_FREE(&(a).page);    \
            break;                          \
        default:                            \
            UNREACHABLE("unknown variant"); \
    }

#define A_PRINT_STAT(a)                            \
    switch ((a).kind) {                            \
        case ALLOCATOR_KIND_ARENA:                 \
            print_stat__MemoryArena(&(a).arena);   \
            break;                                 \
        case ALLOCATOR_KIND_GLOBAL:                \
            print_stat__MemoryGlobal(&(a).global); \
            break;                                 \
        case ALLOCATOR_KIND_PAGE:                  \
            break;                                 \
        default:                                   \
            UNREACHABLE("unknown variant");        \
    }

/**
 *
 * @brief Destroy Allocator type.
 */
void
destroy__Allocator(Allocator *self);

/**
 *
 * @brief Reset Allocator.
 */
void
reset__Allocator(Allocator *self);

#endif // LILY_BASE_ALLOCATOR_H
