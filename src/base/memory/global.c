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

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(MemoryGlobalCell *, MemoryGlobalCell, MemoryBlock block)
{
    MemoryGlobalCell *self =
      __alloc__$Alloc(sizeof(MemoryGlobalCell), alignof(MemoryGlobalCell));

    self->block = block;
    self->next = NULL;

    return self;
}

MemoryBlock *
alloc__MemoryGlobal(MemoryGlobal *self, Usize size, Usize align)
{
    ASSERT(!self->is_destroy);

    if (self->total_size + size < self->capacity) {
        self->total_size += size;
        ++self->total_cell;
    } else {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    void *mem = self->api.alloc(size, align);

    if (!self->last_cells) {
        self->cells =
          NEW(MemoryGlobalCell,
              NEW(MemoryBlock, NEW(MemoryLayout, align, size), mem));
        self->last_cells = self->cells;
    } else {
        self->last_cells->next =
          NEW(MemoryGlobalCell,
              NEW(MemoryBlock, NEW(MemoryLayout, align, size), mem));
        self->last_cells = self->last_cells->next;
    }

    // Memory using by MemoryGlobalCell.
    self->total_size += sizeof(MemoryGlobalCell);

    return &self->last_cells->block;
}

MemoryBlock *
resize__MemoryGlobal(MemoryGlobal *self, MemoryBlock *block, Usize new_size)
{
    ASSERT(!self->is_destroy && block);

    if (new_size < block->layout.size) {
        return block;
    }

    if (self->total_size + (new_size - block->layout.size) < self->capacity) {
        self->total_size += (new_size - block->layout.size);
    } else {
        perror("Lily(Fail): too much memory allocation allocated");
        exit(1);
    }

    block->mem = self->api.resize(
      block->mem, block->layout.size, new_size, DEFAULT_ALIGNMENT);
    block->layout.size = new_size;
    block->is_free = false;

    return block;
}

void
free__MemoryGlobal(MemoryGlobal *self, MemoryBlock *block)
{
    ASSERT(!self->is_destroy);

    Usize layout_size = block->layout.size;

    FREE(MemoryBlock, block, &self->api);

    ++self->total_cell_free;
    self->total_size_free += layout_size;
}

void
print_stat__MemoryGlobal(const MemoryGlobal *self)
{
    Float32 mib_total_size = self->total_size / MiB;
    Float32 mib_total_size_free = self->total_size_free / MiB;
    Float32 mib_capacity = self->capacity / MiB;

    PRINTLN("==================================");
    PRINTLN("=========Global allocator=========");
    PRINTLN("total size: {d} b => {f} MiB", self->total_size, mib_total_size);
    PRINTLN("total cell: {d}", self->total_cell);
    PRINTLN("total cell free: {d}", self->total_cell_free);
    PRINTLN("total size free: {d} b => {f} MiB",
            self->total_size_free,
            mib_total_size_free);
    PRINTLN("capacity: {d} b => {f} MiB", self->capacity, mib_capacity);
    PRINTLN("==================================");
}

void
destroy__MemoryGlobal(MemoryGlobal *self)
{
    MemoryGlobalCell *current = self->cells;

    while (current) {
        if (current->block.is_free) {
            MemoryGlobalCell *tmp = current;

            current = current->next;

            self->api.free((void **)&tmp,
                           sizeof(MemoryGlobalCell),
                           alignof(MemoryGlobalCell));

            self->total_size_free += sizeof(MemoryGlobalCell);
            continue;
        }

        Usize cell_size = current->block.layout.size;

        FREE(MemoryGlobalCell, current, &self->api);

        ++self->total_cell_free;
        self->total_size_free += cell_size;

        MemoryGlobalCell *tmp = current;

        current = current->next;

        self->api.free(
          (void **)&tmp, sizeof(MemoryGlobalCell), alignof(MemoryGlobalCell));

        // Memory using by MemoryGlobalCell.
        self->total_size_free += sizeof(MemoryGlobalCell);
    }

    self->is_destroy = true;
}
