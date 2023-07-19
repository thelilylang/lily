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
#include <base/macros.h>
#include <base/memory/arena.h>
#include <base/print.h>
#include <base/units.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(MemoryArena, MemoryArena)
{

    Usize capacity = __max_capacity__$Alloc();
    MemoryApi api = NEW(MemoryApi);

    return (MemoryArena){
        .api = api,
        .cells = NULL,
        .last_cell = NULL,
        .arena = api.alloc(capacity, DEFAULT_ALIGNMENT),
        .total_size = 0,
        .pos = 0,
        .capacity = capacity,
        .is_destroy = false,
    };
}

MemoryArena
from_capacity__MemoryArena(Usize capacity)
{
    MemoryApi api = NEW(MemoryApi);

    return (MemoryArena){
        .api = api,
        .cells = NULL,
        .last_cell = NULL,
        .arena = api.alloc(capacity, DEFAULT_ALIGNMENT),
        .total_size = 0,
        .pos = 0,
        .capacity = capacity,
        .is_destroy = false,
    };
}

MemoryBlock *
alloc__MemoryArena(MemoryArena *self, Usize size, Usize align)
{
    ASSERT(!self->is_destroy);

    if (self->total_size + size + align < self->capacity) {
        self->total_size += size;
        self->pos += size + align;
        ++self->total_cell;
    } else {
        perror("Lily(Fail): too mutch allocated memory or out of memory");
        exit(1);
    }

    void *mem = (void *)ALIGN(self->arena + self->pos, align);

    if (!self->last_cell) {
        self->cells =
          NEW(MemoryCell,
              NEW(MemoryBlock, NEW(MemoryLayout, align, size), mem, false));
        self->last_cell = self->cells;
    } else {
        self->last_cell->next =
          NEW(MemoryCell,
              NEW(MemoryBlock, NEW(MemoryLayout, align, size), mem, false));
        self->last_cell = self->last_cell->next;
    }

    // Memory using by MemoryCell.
    self->total_size += sizeof(MemoryCell);

    return &self->last_cell->block;
}

MemoryBlock *
resize__MemoryArena(MemoryArena *self, MemoryBlock *block, Usize new_size)
{
    ASSERT(!self->is_destroy);

    if (!block) {
        return alloc__MemoryArena(self, new_size, block->layout.align);
    } else if (new_size == 0) {
        block->layout.size = 0;
        block->mem = NULL;

        return block;
    }

    MemoryBlock *new_block =
      alloc__MemoryArena(self, new_size, block->layout.align);

    memcpy(new_block->mem, block->mem, new_size);

    return new_block;
}

void
destroy__MemoryArena(MemoryArena *self)
{
    if (self->total_size > 0) {
        MemoryCell *current = self->cells;

        while (current) {
            Usize cell_size = current->block.layout.size;

            FREE(MemoryCell, current, &self->api);

            ++self->total_cell_free;
            self->total_size_free += cell_size;

            MemoryCell *tmp = current;

            current = current->next;

            self->api.free(
              (void **)&tmp, sizeof(MemoryCell), alignof(MemoryCell));

            // Memory using by MemoryCell.
            self->total_size_free += sizeof(MemoryCell);
        }

        self->api.free(&self->arena, self->total_size, DEFAULT_ALIGNMENT);
        self->is_destroy = true;
    }
}

void
reset__MemoryArena(MemoryArena *self)
{
    destroy__MemoryArena(self);

    self->total_size = 0;
    self->total_cell = 0;
    self->total_cell_free = 0;
    self->total_size_free = 0;
    self->pos = 0;
    self->arena = self->api.alloc(self->capacity, DEFAULT_ALIGNMENT);
    self->is_destroy = false;
}

void
print_stat__MemoryArena(const MemoryArena *self)
{
    Float32 mib_total_size = self->total_size / MiB;
    Float32 mib_total_size_free = self->total_size_free / MiB;
    Float32 mib_capacity = self->capacity / MiB;

    PRINTLN("===================================");
    PRINTLN("==========Arena allocator==========");
    PRINTLN("total size: {d} b => {f} MiB", self->total_size, mib_total_size);
    PRINTLN("total cell: {d}", self->total_cell);
    PRINTLN("total cell free: {d}", self->total_cell_free);
    PRINTLN("total size free: {d} b => {f} MiB",
            self->total_size_free,
            mib_total_size_free);
    PRINTLN("capacity: {d} b => {f} MiB", self->capacity, mib_capacity);
    PRINTLN("===================================");
}
