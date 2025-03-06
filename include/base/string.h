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

#ifndef LILY_BASE_STRING_H
#define LILY_BASE_STRING_H

#include <base/macros.h>
#include <base/types.h>

#include <stdbool.h>

#ifdef ENV_DEBUG
#define DEBUG_STRING(self, res)          \
    push_str__String(res, "\"");         \
    push_str__String(res, self->buffer); \
    push_str__String(res, "\"");
#endif

#define STRING_DEFAULT_CAPACITY 4

#define PUSH_STR_AND_FREE(self, s) \
    {                              \
        push_str__String(self, s); \
        lily_free(s);              \
    }

#define APPEND_AND_FREE(self, s) \
    {                            \
        append__String(self, s); \
        FREE(String, s);         \
    }

typedef struct Vec Vec;

typedef struct String
{
    char *buffer;
    Usize len;
    Usize capacity;
} String;

/**
 *
 */
CONSTRUCTOR(String *, String);

/**
 *
 * @brief Append String.
 */
void
append__String(String *self, const String *other);

/**
 *
 * @brief Clone String.
 */
String *
clone__String(String *self);

/**
 *
 * @brief Count the number of repetitions of this character in this String.
 */
Usize
count_c__String(const String *self, char c);

/**
 *
 * @brief Find a character into a string.
 * @return char*? (&)
 */
char *
find__String(const String *self, char c);

/**
 *
 * @brief Find a substring into a string.
 * @return char*? (&)
 */
char *
find_sub__String(const String *self, const char *match);

/**
 *
 * @brief Find a substring into a string overt a list of matches.
 * @return char*? (&)
 */
char *
find_sub_over_many__String(const String *self,
                           const char **matches,
                           Usize matches_n,
                           Isize *match_at);

/**
 *
 * @brief A format way for String.
 */
String *
format__String(char *fmt, ...);

/**
 *
 * @brief Construct String type with default buffer items.
 */
String *
from__String(char *buffer);

/**
 *
 * @brief Get item from String.
 */
char
get__String(const String *self, Usize index);

/**
 *
 * @brief Get slice from String.
 */
char *
get_slice__String(const String *self, Usize start, Usize end);

/**
 *
 * @brief Grow String buffer.
 * @param new_capacity New buffer capacity.
 */
void
grow__String(String *self, Usize new_capacity);

/**
 *
 * @brief Insert an item at the given index.
 */
void
insert__String(String *self, char item, Usize index);

/**
 *
 * @brief Insert an item at n + 1 index.
 */
void
insert_after__String(String *self, char item, Usize index);

/**
 *
 * @brief Return true if the string is empty otherwise return false.
 */
inline bool
is_empty__String(const String *self)
{
    return self->len == 0;
}

/**
 *
 * @brief Get last character.
 */
char
last__String(String *self);

/**
 *
 * @brief Remove last item from String.
 */
char
pop__String(String *self);

/**
 *
 * @brief Push char to String.
 */
void
push__String(String *self, char item);

/**
 *
 * @brief Push str to String.
 */
void
push_str__String(String *self, char *s);

/**
 *
 * @brief Push str according its len to String.
 */
void
push_str_with_len__String(String *self, const char *s, Usize len);

/**
 *
 * @brief Remove character into a string at the given index.
 */
char
remove__String(String *self, Usize index);

/**
 *
 * @brief Repeat n times a str.
 */
String *
repeat__String(char *s, Usize n);

/**
 *
 * @brief Replace character by the `replacement` character at the given index.
 */
void
replace__String(String *self, Usize index, char replacement);

/**
 *
 * @brief Replace `c` character, by `replacement` character.
 */
void
replace_c__String(String *self, char c, char replacement);

/**
 *
 * @brief Replace many characters, by the corresponding `replacements`.
 * @param cs const char* (&) - Array of character.
 * @param replacements const char* (&) - Array of replacement characters.
 * @param len Length of `cs` and `replacements` array.
 */
void
replace_many__String(String *self,
                     const char *cs,
                     const char *replacements,
                     Usize len);

/**
 *
 * @brief Replace `s` sub string, by `replacement` sub string.
 * @param s const char* (&)
 * @param replacement const char* (&)
 */
void
replace_sub__String(String *self, const char *s, const char *replacement);

/**
 *
 * @brief Replace `subs` subs string, by `replacements` subs string.
 * @param len Length of `subs` and `replacements` array.
 * @param subs const char* (&)* (&)
 * @param replacements const char* (&)* (&)
 */
void
replace_many_sub__String(String *self,
                         const char **subs,
                         const char **replacements,
                         Usize len);

/**
 *
 * @note Enable all constants escapes by replacing.
 * @example "\\a" => "\a", "\\b" => "\b", "\\e" => "\e", "\\f" => "\f", "\\n" =>
 * "\n", "\\r" => "\r", "\\t" => "\t", "\\v" => "\v", "\\\\" => "\\", "\\'" =>
 * "\'", "\\?" => "\?"
 * @note except "\"".
 */
void
enable_constant_escapes__String(String *self);

/**
 *
 * @brief Reverse String.
 */
void
reverse__String(String *self);

/**
 *
 * @brief Get item from String (no assert).
 */
inline char
safe_get__String(const String *self, Usize index)
{
    return index < self->len ? self->buffer[index] : '\0';
}

/**
 *
 * @brief Split string.
 * @return Vec<String*>*.
 * @note The result and its items can be free.
 */
Vec *
split__String(String *self, char separator);

/**
 *
 * @brief Take a slice of string at index to the end.
 */
String *
take_slice__String(String *self, Usize index);

/**
 *
 * @brief Ungrow String buffer.
 */
void
ungrow__String(String *self);

/**
 *
 * @brief Write Uint8 character on the String's buffer.
 */
void
write_u8__String(String *self, Uint8 v);

/**
 *
 * @brief Write Uint16 character on the String's buffer.
 */
void
write_u16__String(String *self, Uint16 v);

/**
 *
 * @brief Write Uint32 character on the String's buffer.
 */
void
write_u32__String(String *self, Uint32 v);

/**
 *
 * @brief Write Uint64 character on the String's buffer.
 */
void
write_u64__String(String *self, Uint64 v);

/**
 *
 * @brief Free String buffer.
 */
DESTRUCTOR(String, String *self);

typedef struct StringIter
{
    String *string;
    Usize count;
} StringIter;

/**
 *
 * @brief Construct StringIter type.
 */
inline CONSTRUCTOR(StringIter, StringIter, String *string)
{
    return (StringIter){ .string = string, .count = 0 };
}

/**
 *
 * @brief Get the next character.
 */
inline char
next__StringIter(StringIter *self)
{
    return safe_get__String(self->string, self->count++);
}

#endif // LILY_BASE_STRING_H
