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

#ifndef LILY_CORE_SHARED_SCANNER_H
#define LILY_CORE_SHARED_SCANNER_H

#include <core/shared/cursor.h>
#include <core/shared/location.h>
#include <core/shared/source.h>

#include <ctype.h>

typedef struct Scanner
{
    Location location;
    Source source;
    Usize *count_error; // Usize* (&)
} Scanner;

/**
 *
 * @brief Construct Scanner type.
 */
inline CONSTRUCTOR(Scanner, Scanner, Source source, Usize *count_error)
{
    return (Scanner){ .location = default__Location(source.file->name),
                      .source = source,
                      .count_error = count_error };
}

/**
 *
 * @brief Move next one character.
 */
inline void
next_char__Scanner(Scanner *self)
{
    return next_char__Source(&self->source);
}

/**
 *
 * @brief Move back one character.
 */
inline void
previous_char__Scanner(Scanner *self)
{
    return previous_char__Source(&self->source);
}

/**
 *
 * @brief Skip space (new line, tab, ...).
 */
void
skip_space__Scanner(Scanner *self);

/**
 *
 * @brief Next char n times.
 */
void
jump__Scanner(Scanner *self, Usize n);

/**
 *
 * @brief Assign to line and column to start_line and start_column Location's
 * field.
 */
void
start_token__Scanner(Scanner *self, Usize line, Usize column, Usize position);

/**
 *
 * @brief Assign to line and column to end_line and end_column Location's
 * field.
 */
void
end_token__Scanner(Scanner *self, Usize line, Usize column, Usize position);

/**
 *
 * @brief Get character at position + n.
 * @return char*? (&)
 */
char *
peek_char__Scanner(const Scanner *self, Usize n);

#endif // LILY_CORE_SHARED_SCANNER_H
