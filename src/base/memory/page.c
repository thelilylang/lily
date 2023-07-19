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
#include <base/memory/page.h>

#include <stdio.h>
#include <stdlib.h>

MemoryBlock *
alloc__MemoryPage(MemoryPage *self, Usize size, Usize align)
{
    self->block = NEW(MemoryBlock,
                      NEW(MemoryLayout, align, size),
                      self->api.alloc(size, align),
                      true);
    self->is_undef = false;

    return &self->block;
}

MemoryBlock *
resize__MemoryPage(MemoryPage *self, Usize new_size)
{
    ASSERT(!self->is_undef);

    self->block.mem = self->api.resize(self->block.mem,
                                       self->block.layout.size,
                                       new_size,
                                       self->block.layout.align);
    return &self->block;
}

void
free__MemoryPage(MemoryPage *self)
{
    ASSERT(!self->is_undef);

    FREE(MemoryBlock, &self->block, &self->api);
    self->is_undef = true;
}
