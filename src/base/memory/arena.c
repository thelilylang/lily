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
#include <base/macros.h>
#include <base/memory/arena.h>
#include <base/print.h>
#include <base/units.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *
alloc__MemoryArena(MemoryArena *self, Usize size, Usize align)
{
    ASSERT(!self->is_destroy);

    if (self->total_size + size < self->capacity) {
        void *mem = (void *)ALIGN(self->arena + self->total_size, align);

        self->total_size += size;

        return mem;
    } else {
        perror("Lily(Fail): too mutch allocated memory or out of memory");
        exit(1);
    }
}

void *
resize__MemoryArena(MemoryArena *self, void *mem, Usize new_size, Usize align)
{
    ASSERT(!self->is_destroy);

    if (!mem) {
        return alloc__MemoryArena(self, new_size, align);
    } else if (new_size == 0) {
        return NULL;
    }

    void *new_mem = alloc__MemoryArena(self, new_size, align);

    memcpy(new_mem, mem, new_size);

    return new_mem;
}

void
destroy__MemoryArena(MemoryArena *self)
{
    free__MemoryGlobal(self->arena);
    self->is_destroy = true;
}

void
reset__MemoryArena(MemoryArena *self)
{
    destroy__MemoryArena(self);

    self->total_size = 0;
    self->arena = alloc__MemoryGlobal(self->capacity, DEFAULT_ALIGNMENT);
    self->is_destroy = false;
}

void
print_stat__MemoryArena(const MemoryArena *self)
{
    Float32 mib_total_size = self->total_size / MiB;
    Float32 mib_capacity = self->capacity / MiB;

    PRINTLN("===================================");
    PRINTLN("==========Arena allocator==========");
    PRINTLN("total size: {d} b => {f} MiB", self->total_size, mib_total_size);
    PRINTLN("capacity: {d} b => {f} MiB", self->capacity, mib_capacity);
    PRINTLN("===================================");
}
