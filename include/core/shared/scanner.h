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

#ifndef LILY_CORE_SHARED_SCANNER_H
#define LILY_CORE_SHARED_SCANNER_H

#include <base/sized_str.h>

#include <core/shared/cursor.h>
#include <core/shared/location.h>
#include <core/shared/search.h>
#include <core/shared/source.h>

#include <ctype.h>

#define SCANNER_HAS_REACH_END(self) \
    (self.source.cursor.position >= self.source.file->len - 1)

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
 * @brief Check if the current is space (new line, tab, ...).
 */
inline bool
is_space__Scanner(Scanner *self)
{
    return isspace(self->source.cursor.current);
}

/**
 *
 * @brief Check if the peeked character is space (new line, tab, ...).
 * @param c const char*?
 */
inline bool
is_space_with_peeked_char__Scanner([[maybe_unused]] const Scanner *,
                                   const char *c)
{
    return c ? isspace(*c) : false;
}

/**
 *
 * @brief Skip space (new line, tab, ...).
 */
void
skip_space__Scanner(Scanner *self);

/**
 *
 * @brief Skip space, but except new line (tab, ...).
 */
void
skip_space_except_new_line__Scanner(Scanner *self);

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
start_token__Scanner(Scanner *self,
                     const Usize line,
                     const Usize column,
                     const Usize position);

/**
 *
 * @brief Assign to line and column to end_line and end_column Location's
 * field.
 */
void
end_token__Scanner(Scanner *self,
                   const Usize line,
                   const Usize column,
                   const Usize position);

/**
 *
 * @brief Get character at position + n.
 * @return char*? (&)
 */
char *
peek_char__Scanner(const Scanner *self, const Usize n);

/**
 *
 * @brief Generic function for obtaining a keyword from any scanner.
 * @param keywords Expect an array sorted in ascending order.
 * @return If the return value is -1, this means that the function has not found
 * a keyword, or that this is an identifier.
 */
inline Int32
get_keyword__Scanner(const String *id,
                     const SizedStr keywords[],
                     const Int32 keyword_ids[],
                     const Usize keywords_len)
{
    return get_id__Search(id, keywords, keyword_ids, keywords_len);
}

#endif // LILY_CORE_SHARED_SCANNER_H
