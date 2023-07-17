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

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(MemoryArena, MemoryArena) {

	Usize capacity = __max_capacity__$Alloc();
	MemoryApi api = NEW(MemoryApi);

	return (MemoryArena){
		.api = api,
		.arena = api.alloc(capacity, 8),
		.total_size = 0,
		.capacity = capacity,
	};
}

void *
alloc__MemoryArena(MemoryArena *self, Usize size)
{
	if (self->total_size + size > self->capacity) {
		FAILED("alloc: too mutch allocated memory");
	}

	void *res = self->arena + self->total_size;
	self->total_size += size;

	return res;
}

void *
resize__MemoryArena(MemoryArena *self, void *mem, Usize new_size)
{
	void *resized_mem = self->api.resize(mem, new_size, 8);

	if (resized_mem > self->arena) {
		FAILED("resize: too mutch allocated memory");
	} else if (resized_mem < self->arena + self->total_size) {
		self->total_size = resized_mem - self->arena;
	}

	return resized_mem;
}

void
reset__MemoryArena(MemoryArena *self)
{
	destroy__MemoryArena(self);

	self->total_size = 0;
	self->arena = self->api.alloc(self->capacity, 8);
}
