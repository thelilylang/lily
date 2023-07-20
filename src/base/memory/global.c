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

#include <base/assert.h>
#include <base/memory/global.h>
#include <base/new.h>
#include <base/print.h>
#include <base/units.h>

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

static MemoryApi api = { .align = __align__,
                         .alloc = __alloc__,
                         .resize = __resize__,
                         .free = __free__ };
static Usize total_size = 0;
static Usize total_block = 0;
static Usize total_block_free = 0;
static Usize total_size_free = 0;
static MemoryBlock *blocks = NULL;
static MemoryBlock *last_block = NULL;
static Usize capacity = 0;

void *
alloc__MemoryGlobal(Usize size, Usize align)
{
    if (capacity == 0) {
        capacity = __max_capacity__$Alloc();
    }

    if (total_size + size < capacity) {
        total_size += size + sizeof(MemoryBlock);
        ++total_block;
    } else {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    if (!last_block) {
        blocks =
          NEW(MemoryBlock, &api, NEW(MemoryLayout, align, size), NULL);
        last_block = blocks;
    } else {
        last_block->next = NEW(
          MemoryBlock, &api, NEW(MemoryLayout, align, size), last_block);
        last_block->next->prev = last_block;
        last_block = last_block->next;
    }

    return (void *)(last_block + sizeof(MemoryBlock));
}

void *
resize__MemoryGlobal(void *mem, Usize new_size)
{
    ASSERT(mem);

    MemoryBlock *block = (MemoryBlock *)(mem - sizeof(MemoryBlock));

    if (new_size < block->layout.size) {
        return mem;
    } else if (capacity == 0) {
        capacity = __max_capacity__$Alloc();
    }

    if (total_size + (new_size - block->layout.size) < capacity) {
        total_size += (new_size - block->layout.size);
    } else {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    mem = api.resize(block,
                       block->layout.size + sizeof(MemoryBlock),
                       new_size + sizeof(MemoryBlock),
                       block->layout.align);
    block->layout.size = new_size;

    return (void *)(block + sizeof(MemoryBlock));
}

void
free__MemoryGlobal(void *mem)
{
    MemoryBlock *block = (MemoryBlock *)(mem - sizeof(MemoryBlock));

    if (block->prev) {
        block->prev->next = block->next;
    }

    api.free((void **)&block, block->layout.size + sizeof(MemoryBlock), block->layout.align);

    ++total_block_free;
    total_size_free += block->layout.size + sizeof(MemoryBlock);
}

void
print_stat__MemoryGlobal()
{
    Float32 mib_total_size = total_size / MiB;
    Float32 mib_total_size_free = total_size_free / MiB;
    Float32 mib_capacity = capacity / MiB;

    PRINTLN("==================================");
    PRINTLN("=========Global allocator=========");
    PRINTLN("total size: {d} b => {f} MiB", total_size, mib_total_size);
    PRINTLN("total block: {d}", total_block);
    PRINTLN("total block free: {d}", total_block_free);
    PRINTLN("total size free: {d} b => {f} MiB",
            total_size_free,
            mib_total_size_free);
    PRINTLN("capacity: {d} b => {f} MiB", capacity, mib_capacity);
    PRINTLN("==================================");
}
