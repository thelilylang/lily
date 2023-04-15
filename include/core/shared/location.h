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

#ifndef LILY_CORE_SHARED_LOCATION_H
#define LILY_CORE_SHARED_LOCATION_H

#include <base/macros.h>
#include <base/types.h>

typedef struct Location
{
    const char *filename;
    Usize start_line;
    Usize end_line;
    Usize start_column;
    Usize end_column;
    Usize position; // Position in the file content
} Location;

/**
 *
 * @brief Construct Location type.
 */
inline CONSTRUCTOR(Location,
                   Location,
                   const char *filename,
                   Usize start_line,
                   Usize end_line,
                   Usize start_column,
                   Usize end_column,
                   Usize position)
{
    return (Location){ .filename = filename,
                       .start_line = start_line,
                       .end_line = end_line,
                       .start_column = start_column,
                       .end_column = end_column,
                       .position = position };
}

/**
 *
 * @brief Construct Location type with default value on start_line, end_line,
 * start_column, end_column.
 */
inline Location
default__Location(const char *filename)
{
    return (Location){ .filename = filename,
                       .start_line = 1,
                       .end_line = 1,
                       .start_column = 1,
                       .end_column = 1,
                       .position = 0 };
}

/**
 *
 * @brief Set start_line and start_column.
 */
void
start__Location(Location *self, Usize line, Usize column, Usize position);

/**
 *
 * @brief Set end_line and end_column.
 */
void
end__Location(Location *self, Usize line, Usize column);

/**
 *
 * @brief Set start_line, start_column, end_line and end_column.
 */
void
set_all__Location(Location *self, const Location *other);

/**
 *
 * @brief Clone Location.
 * @return New Location instance.
 */
inline Location
clone__Location(const Location *self)
{
    return (Location){ .filename = self->filename,
                       .start_line = self->start_line,
                       .end_line = self->end_line,
                       .start_column = self->start_column,
                       .end_column = self->end_column,
                       .position = self->position };
}

/**
 *
 * @brief Convert Location in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, Location, const Location *self);
#endif

/**
 *
 * @brief Print debug Location struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, Location, const Location *self);
#endif

#endif // LILY_CORE_SHARED_LOCATION_H
