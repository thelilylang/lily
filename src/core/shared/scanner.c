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

#include <core/shared/scanner.h>

#include <stdio.h>
#include <stdlib.h>

void
skip_space__Scanner(Scanner *self)
{
    while (isspace(self->source.cursor.current) &&
           self->source.cursor.position < self->source.file->len - 1) {
        next_char__Scanner(self);
    }
}

void
skip_space_except_new_line__Scanner(Scanner *self)
{
    while (self->source.cursor.current != '\n' &&
           isspace(self->source.cursor.current) &&
           self->source.cursor.position < self->source.file->len - 1) {
        next_char__Scanner(self);
    }
}

void
jump__Scanner(Scanner *self, Usize n)
{
    while (n--) {
        next_char__Scanner(self);
    }
}

void
start_token__Scanner(Scanner *self,
                     const Usize line,
                     const Usize column,
                     const Usize position)
{
    self->location.start_line = line;
    self->location.start_column = column;
    self->location.start_position = position;
}

void
end_token__Scanner(Scanner *self,
                   const Usize line,
                   const Usize column,
                   const Usize position)
{
    ASSERT(self->location.start_line <= line);
    ASSERT(self->location.start_column <= column ||
           self->location.start_line != line);

    self->location.end_line = line;
    self->location.end_column = column;
    self->location.end_position = position;
}

char *
peek_char__Scanner(const Scanner *self, const Usize n)
{
    if (self->source.cursor.position + n < self->source.file->len - 1) {
        return (char *)(Uptr)
          self->source.file->content[self->source.cursor.position + n];
    }

    return NULL;
}
