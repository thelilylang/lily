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

#include <base/assert.h>
#include <base/memory/global.h>
#include <base/new.h>
#include <base/print.h>
#include <base/units.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef USE_C_MEMORY_API
#if defined(LILY_APPLE_OS)
#include <malloc/malloc.h>
#else
#include <malloc.h>
#endif
#endif

[[maybe_unused]] static MemoryApi api = { .align = __align__,
                                          .alloc = __alloc__,
                                          .resize = __resize__,
                                          .free = __free__ };
static Usize total_size = 0;
static Usize total_block = 0;
static Usize total_block_free = 0;
static Usize total_size_free = 0;
[[maybe_unused]] static MemoryBlock *top = NULL;
[[maybe_unused]] static MemoryBlock *last = NULL;
[[maybe_unused]] static Usize capacity = 0;
[[maybe_unused]] static pthread_mutex_t global_mutex =
  PTHREAD_MUTEX_INITIALIZER;

void *
alloc__MemoryGlobal(Usize size, Usize align)
{
#ifdef USE_C_MEMORY_API
    void *mem = malloc(size);

    if (mem) {
        ++total_block;
        total_size += size;
    }

    return mem;
#else
    pthread_mutex_lock(&global_mutex);

#ifdef ENV_SAFE
    if (capacity == 0) {
        capacity = __max_capacity__$Alloc();
    }
#endif

    Usize alloc_size = sizeof(MemoryBlock) + size;

#ifdef ENV_SAFE
    if (total_size + alloc_size > capacity) {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }
#endif

    void *mem = api.alloc(alloc_size, align);

    total_size += size;
    ++total_block;

    if (!top) {
        top = (MemoryBlock *)mem;
        last = top;
        top->prev = NULL;
    } else {
        // Split the block if it's large enough
        Usize remaining_size = last->size - alloc_size;

        if (remaining_size >= sizeof(MemoryBlock)) {
            MemoryBlock *new_block = (MemoryBlock *)((char *)last + alloc_size);

            new_block->size = remaining_size - sizeof(MemoryBlock);
            new_block->align = last->align;
            new_block->next = last->next;
            new_block->prev = last;

            last->size = size;
            last->align = align;
            last->next = new_block;
        } else {
            last->next = (MemoryBlock *)mem;
            last->next->prev = last;
            last = last->next;
        }
    }

    last->size = size;
    last->align = align;
    last->next = NULL;

    pthread_mutex_unlock(&global_mutex);

    return (void *)((char *)mem + sizeof(MemoryBlock));
#endif
}

void *
resize__MemoryGlobal(void *mem, Usize new_size)
{
#ifdef USE_C_MEMORY_API
#if defined(LILY_APPLE_OS)
    Usize old_size = malloc_size(mem);
#else
    Usize old_size = malloc_usable_size(mem);
#endif

    void *new_mem = realloc(mem, new_size);

    if (new_mem) {
        total_size += new_size;
        total_size_free += old_size;

        ++total_block;
        ++total_block_free;
    }

    return new_mem;
#else
    if (!mem) {
        return NULL;
    }

#ifdef ENV_SAFE
    if (capacity == 0) {
        capacity = __max_capacity__$Alloc();
    }
#endif

    pthread_mutex_lock(&global_mutex);

    MemoryBlock *block = (MemoryBlock *)((char *)mem - sizeof(MemoryBlock));
    Usize old_size = block->size;
    Usize align = block->align;
    void *resized_mem = mem;

#ifdef ENV_SAFE
    {
        Usize alloc_size =
          new_size < old_size ? old_size - new_size : new_size - old_size;

        if (total_size + alloc_size > capacity) {
            perror("Lily(Fail): too much memory allocation allocated");
            exit(1);
        }
    }
#endif

    if (new_size == 0) {
        // If new_size is 0, treat resize as a deallocation.
        // Remove the block from the linked list.
        if (block == top) {
            top = block->next;
        } else {
            MemoryBlock *prev = block->prev;
            prev->next = block->next;
        }

        if (block->next) {
            block->next->prev = block->prev;
        }

        // Free memory
        api.free(&mem, old_size, align);

        resized_mem = NULL;
    } else if (new_size != old_size) {
        MemoryBlock *resized_block = api.resize(block,
                                                old_size + sizeof(MemoryBlock),
                                                new_size + sizeof(MemoryBlock),
                                                align);

        if (resized_block) {
            // FIXME: leak here. You should probably use api.alloc instead of
            // api.resize. Plus you'd probably have to add a new block to the
            // linked list and remove the old one and free it.
            resized_block->size = new_size;
            resized_mem = (void *)((char *)resized_block + sizeof(MemoryBlock));
            ++total_block;
            total_size +=
              new_size < old_size ? old_size - new_size : new_size - old_size;
        }
    }

    pthread_mutex_unlock(&global_mutex);

    return resized_mem;
#endif
}

void
free__MemoryGlobal(void *mem)
{
#ifdef USE_C_MEMORY_API
#if defined(LILY_APPLE_OS)
    Usize mem_size = malloc_size(mem);
#else
    Usize mem_size = malloc_usable_size(mem);
#endif

    free(mem);

    ++total_block_free;
    total_size_free += mem_size;
#else
    if (!mem) {
        return;
    }

    pthread_mutex_lock(&global_mutex);

    MemoryBlock *block = (MemoryBlock *)((void *)mem - sizeof(MemoryBlock));

    if (block->size == 0 || block->next != NULL) {
        ++total_block_free;

        pthread_mutex_unlock(&global_mutex);

        return;
    }

    if (block == top) {
        top = block->next;
    } else {
        MemoryBlock *prev = block->prev;

        prev->next = block->next;
    }

    if (block->next) {
        block->next->prev = block->prev;
    }

    // Free the memory
    Usize size = block->size;
    Usize align = block->align;

    api.free((void **)&block, sizeof(MemoryBlock) + size, align);

    total_size_free += size;
    ++total_block_free;

    pthread_mutex_unlock(&global_mutex);
#endif
}

void
print_stat__MemoryGlobal()
{
    Float32 mib_total_size = total_size / MiB;
    Float32 mib_total_size_free = total_size_free / MiB;
#ifdef ENV_SAFE
    Float32 mib_capacity = capacity / MiB;
#endif

    PRINTLN("==================================");
    PRINTLN("=========Global allocator=========");
    PRINTLN("total size: {d} b => {f} MiB", total_size, mib_total_size);
    PRINTLN("total block: {d}", total_block);
    PRINTLN("total block free: {d}", total_block_free);
    PRINTLN("total size free: {d} b => {f} MiB",
            total_size_free,
            mib_total_size_free);
#ifdef ENV_SAFE
    PRINTLN("capacity: {d} b => {f} MiB", capacity, mib_capacity);
#endif
    PRINTLN("==================================");
}
