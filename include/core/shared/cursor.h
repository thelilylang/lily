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

#ifndef LILY_CORE_SHARED_CURSOR_H
#define LILY_CORE_SHARED_CURSOR_H

#include <base/macros.h>
#include <base/types.h>

typedef struct Cursor
{
    Usize position;
    char current;
    Usize line;
    Usize column;
} Cursor;

/**
 *
 * @brief Construct Cursor type.
 */
inline CONSTRUCTOR(Cursor, Cursor, char *content)
{
    return (
      Cursor){ .position = 0, .current = content[0], .line = 1, .column = 1 };
}

/**
 *
 * @brief Advance to the next line.
 */
void
next_line__Cursor(Cursor *cursor, char current);

/**
 *
 * @brief Advance to the next (line maybe, ) column, position and update the
 * current.
 */
void
next__Cursor(Cursor *cursor, char current);

/**
 *
 * @brief Back to the previous column, position and update the current.
 */
void
previous__Cursor(Cursor *cursor, char current);

#endif // LILY_CORE_SHARED_CURSOR_H
