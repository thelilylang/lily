/*
 * MIT License
 *
 * Copyright (c) 2022-2026 ArthurPV
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

#include <core/shared/location.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <base/print.h>
#endif

void
start__Location(Location *self,
                const Usize line,
                const Usize column,
                const Usize position)
{
    self->start_line = line;
    self->start_column = column;
    self->start_position = position;
}

void
end__Location(Location *self,
              const Usize line,
              const Usize column,
              const Usize position)
{
    self->end_line = line;
    self->end_column = column;
    self->end_position = position;
}

void
set_all__Location(Location *self, const Location *other)
{
    self->start_line = other->start_line;
    self->start_column = other->start_column;
    self->end_line = other->end_line;
    self->end_column = other->end_column;
    self->start_position = other->start_position;
    self->end_position = other->end_position;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, Location, const Location *self)
{
    return format("Location{{ filename = {s}, start_line = {d}, end_line = "
                  "{d}, start_column = {d}, end_column = {d}, start_position = "
                  "{d}, end_position = {d} }",
                  self->filename,
                  self->start_line,
                  self->end_line,
                  self->start_column,
                  self->end_column,
                  self->start_position,
                  self->end_position);
}

void
IMPL_FOR_DEBUG(debug, Location, const Location *self)
{
    PRINTLN("{sa}", CALL_DEBUG_IMPL(to_string, Location, self));
}
#endif
