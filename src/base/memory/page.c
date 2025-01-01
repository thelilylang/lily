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
#include <base/memory/page.h>
#include <base/print.h>
#include <base/units.h>

#include <stdio.h>
#include <stdlib.h>

void *
alloc__MemoryPage(MemoryPage *self, Usize size, Usize align)
{
    void *mem = alloc__MemoryGlobal(size, align);

    ASSERT(mem);

    self->mem = mem;
    self->is_undef = false;

    return self->mem;
}

void *
resize__MemoryPage(MemoryPage *self, Usize new_size)
{
    ASSERT(!self->is_undef && self->mem);

    self->mem = resize__MemoryGlobal(self->mem, new_size);

    return self->mem;
}

void
free__MemoryPage(MemoryPage *self)
{
    ASSERT(!self->is_undef && self->mem);

    self->is_undef = true;
    free__MemoryGlobal(self->mem);
}
