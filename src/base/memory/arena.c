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

#include <base/memory/arena.h>
#include <base/macros.h>
#include <base/units.h>
#include <base/print.h>
#include <base/assert.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(MemoryArena, MemoryArena) {

	Usize capacity = __max_capacity__$Alloc();
	MemoryApi api = NEW(MemoryApi);

	return (MemoryArena){
		.api = api,
		.arena = api.alloc(capacity, DEFAULT_ALIGNMENT),
		.total_size = 0,
		.pos = 0,
		.capacity = capacity,
	};
}

MemoryArena
from_capacity__MemoryArena(Usize capacity)
{
	MemoryApi api = NEW(MemoryApi);

	return (MemoryArena){
		.api = api,
		.arena = api.alloc(capacity, DEFAULT_ALIGNMENT),
		.total_size = 0,
		.pos = 0,
		.capacity = capacity,
	};
}

void *
alloc__MemoryArena(MemoryArena *self, Usize size)
{
	if (self->total_size + size > self->capacity) {
		FAILED("alloc: too mutch allocated memory");
	} else if (self->pos + size > self->capacity) {
		FAILED("alloc: out of memory");
	}

	self->total_size += size;
	self->pos += size;

	return self->arena + self->pos;
}

void *
resize__MemoryArena(MemoryArena *self, void *mem, Usize new_size)
{
	ASSERT(mem);
	
	if (new_size == 0) {
		return NULL;
	}

	void *new_mem = alloc__MemoryArena(self, new_size);

	memcpy(new_mem, mem, new_size);

	return new_mem;
}

void
reset__MemoryArena(MemoryArena *self)
{
	destroy__MemoryArena(self);

	self->total_size = 0;
	self->pos = 0;
	self->arena = self->api.alloc(self->capacity, DEFAULT_ALIGNMENT);
}

void
print_stat__MemoryArena(const MemoryArena *self)
{
	Float32 total_size = self->total_size / MiB;
	Float32 capacity = self->capacity / MiB;

	PRINTLN("===================================");
	PRINTLN("==========Arena allocator==========");
	PRINTLN("total size: {d} b => {f} MiB", self->total_size, total_size);
	PRINTLN("capacity: {d} b => {f} MiB", self->capacity, capacity);
	PRINTLN("===================================");
}
