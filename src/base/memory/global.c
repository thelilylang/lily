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

#include <base/new.h>
#include <base/memory/global.h>

#include <stdlib.h>
#include <stdio.h>

MemoryLayout *
alloc__MemoryGlobal(MemoryGlobal *self, Usize size)
{
	if (self->total_size + size < self->capacity) {
		self->total_size += size;
		++self->total_cell;
	} else {
        perror("Lily(Fail): too much memory allocation allocated");
		exit(1);
	}

	void *mem = __alloc__$Alloc(size, DEFAULT_ALIGNMENT);

	if (!self->last_cells) {
		self->cells = NEW(MemoryGlobalCell, mem, size);
		self->last_cells = self->cells;
	} else {
		self->last_cells->next = NEW(MemoryGlobalCell, mem, size);
		self->last_cells = self->last_cells->next;
	}

	return &self->last_cells->layout;
}

MemoryLayout *
resize__MemoryGlobal(MemoryGlobal *self, MemoryLayout *layout, Usize new_size)
{
	if (new_size < layout->size) {
		return layout;
	}

	if (self->total_size + (new_size - layout->size) < self->capacity) {
		self->total_size += (new_size - layout->size);
	} else {
        perror("Lily(Fail): too much memory allocation allocated");
		exit(1);
	}

	layout->mem = __resize__$Alloc(layout->mem, layout->size, new_size, DEFAULT_ALIGNMENT);
	layout->size = new_size;
	layout->is_free = false;

	return layout;
}

void
free__MemoryGlobal(MemoryGlobal *self, MemoryLayout *layout)
{
	if (layout->is_free) {
        perror("Lily(Fail): this layout is already free");
		exit(1);
	}

	Usize layout_size = layout->size;

	FREE(MemoryLayout, layout);

	++self->total_cell_free;
	self->total_size_free += layout_size;
}

DESTRUCTOR(MemoryGlobal, MemoryGlobal *self)
{
	MemoryGlobalCell *current = self->cells;

	while (current) {
		Usize cell_size = current->layout.size;

		FREE(MemoryGlobalCell, current);

		++self->total_cell_free;
		self->total_size_free += cell_size;

		current = current->next;
	}
}
