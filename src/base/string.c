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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/format.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/str.h>
#include <base/string.h>
#include <base/vec.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

CONSTRUCTOR(String *, String)
{
    String *self = lily_malloc(sizeof(String));

    self->buffer = lily_malloc(1);
    self->buffer[0] = '\0';
    self->len = 0;
    self->capacity = 0;

    return self;
}

void
append__String(String *self, const String *other)
{
    push_str__String(self, other->buffer);
}

String *
clone__String(String *self)
{
    char *buffer = lily_malloc(self->capacity + 1);
    memcpy(buffer, self->buffer, self->len + 1);

    String *clone = lily_malloc(sizeof(String));

    clone->buffer = buffer;
    clone->capacity = self->capacity;
    clone->len = self->len;

    return clone;
}

Usize
count_c__String(const String *self, char c)
{
    Usize count = 0;

    for (Usize i = 0; i < self->len; ++i) {
        if (self->buffer[i] == c) {
            ++count;
        }
    }

    return count;
}

char *
find__String(const String *self, char c)
{
    if (self->buffer) {
        return strchr(self->buffer, c);
    }

    return NULL;
}

char *
find_sub__String(const String *self, const char *match)
{
    if (self->buffer) {
        return strstr(self->buffer, match);
    }

    return NULL;
}

char *
find_sub_over_many__String(const String *self,
                           const char **matches,
                           Usize matches_n,
                           Isize *match_at)
{
    if (self->buffer) {
        return strstr_list__Str(self->buffer, matches, matches_n, match_at);
    }

    return NULL;
}

String *
format__String(char *fmt, ...)
{
    va_list arg;

    va_start(arg, fmt);

    char *buffer = vformat(fmt, arg);

    va_end(arg);

    Usize len = strlen(buffer);
    String *self = lily_malloc(sizeof(String));

    self->buffer = buffer;
    self->len = len;
    self->capacity = len;

    return self;
}

String *
from__String(char *buffer)
{
    Usize len = strlen(buffer);

    if (len > 0) {
        String *self = lily_malloc(sizeof(String));

        self->capacity = len * 2;
        self->buffer = lily_malloc(self->capacity);
        self->len = len;

        for (Usize i = 0; i < self->len; ++i) {
            self->buffer[i] = buffer[i];
        }

        self->buffer[self->len] = '\0';

        return self;
    }

    return NEW(String);
}

char
get__String(const String *self, Usize index)
{
    ASSERT(index < self->len);

    return self->buffer[index];
}

char *
get_slice__String(const String *self, Usize start, Usize end)
{
    ASSERT(start < end);

    Usize size = (end - start) + 1;
    char *s = lily_malloc(size);

    memset(s, 0, size);

    for (Usize i = start, j = 0; i < end; ++i) {
        s[j++] = get__String(self, i);
    }

    return s;
}

void
grow__String(String *self, Usize new_capacity)
{
    ASSERT(new_capacity >= self->capacity);

    self->buffer = lily_realloc(self->buffer, new_capacity);
    self->capacity = new_capacity;
}

void
insert__String(String *self, char item, Usize index)
{
    ASSERT(index < self->len);

    char move = self->buffer[index];

    self->buffer[index] = item;

    for (Usize i = index + 1; i < self->len; ++i) {
        char tmp_move = move;
        move = self->buffer[i];

        self->buffer[i] = tmp_move;
    }

    push__String(self, move);
}

void
insert_after__String(String *self, char item, Usize index)
{
    ASSERT(index + 1 <= self->len);

    if (index + 1 == self->len) {
        push__String(self, item);

        return;
    }

    char move = self->buffer[index + 1];

    self->buffer[index + 1] = item;

    for (Usize i = index + 2; i < self->len; ++i) {
        char tmp_move = move;
        move = self->buffer[i];

        self->buffer[i] = tmp_move;
    }

    push__String(self, move);
}

char
last__String(String *self)
{
    ASSERT(self->len > 0);

    return self->buffer[self->len - 1];
}

char
pop__String(String *self)
{
    ASSERT(self->len > 0);

    char c = self->buffer[self->len - 1];

    self->buffer[self->len - 1] = '\0';
    self->len--;

    ungrow__String(self);

    return c;
}

void
push__String(String *self, char item)
{
    if (!self->capacity)
        grow__String(self, STRING_DEFAULT_CAPACITY);
    else if (self->len >= self->capacity - 1)
        grow__String(self, self->capacity * 2);

    self->buffer[self->len] = item;
    self->buffer[++self->len] = '\0';
}

void
push_str__String(String *self, char *s)
{
    for (Usize i = 0; s[i++];) {
        push__String(self, s[i - 1]);
    }
}

void
push_str_with_len__String(String *self, const char *s, Usize len)
{
    for (Usize i = 0; i < len; ++i) {
        push__String(self, s[i]);
    }
}

char
remove__String(String *self, Usize index)
{
    ASSERT(index < self->len);

    char item = self->buffer[index];
    self->len -= 1;

    // Align the rest of the buffer
    for (Usize i = index; i < self->len; ++i) {
        self->buffer[i] = self->buffer[i + 1];
    }

    self->buffer[self->len] = '\0';

    ungrow__String(self);

    return item;
}

String *
repeat__String(char *s, Usize n)
{
    String *res = NEW(String);

    for (Usize i = 0; i < n; ++i) {
        push_str__String(res, s);
    }

    return res;
}

void
replace__String(String *self, Usize index, char replacement)
{
    ASSERT(index < self->len);

    self->buffer[index] = replacement;
}

void
replace_c__String(String *self, char c, char replacement)
{
    if (self->buffer) {
        char *current = self->buffer;

        while ((current = strchr(current, c))) {
            *current = replacement;
        }
    }
}

void
replace_many__String(String *self,
                     const char *cs,
                     const char *replacements,
                     Usize len)
{
    if (self->buffer) {
        for (Usize i = 0; i < len; ++i) {
            char c = cs[i];
            char replacement = replacements[i];

            replace__String(self, c, replacement);
        }
    }
}

void
replace_sub__String(String *self, const char *s, const char *replacement)
{
    if (self->buffer) {
        char *begin = self->buffer;   // char* (&)
        char *current = self->buffer; // char* (&)
        Usize current_index = 0;

        while ((current = strstr(current, s))) {
            current_index = current - begin;

            char *s_current = (char *)s;                     // char* (&)
            char *replacement_current = (char *)replacement; // char* (&)
            Usize nb_deletion = 0;

            for (Usize i = 0; *s_current || *replacement_current; ++i) {
                if (*s_current) {
                    if (*replacement_current) {
                        replace__String(
                          self, current_index + i, *replacement_current);
                        ++replacement_current;
                    } else {
                        remove__String(self, current_index + i - nb_deletion);
                        ++nb_deletion;
                    }

                    ++s_current;

                    continue;
                }

                insert_after__String(
                  self, *replacement_current, current_index + i - 1);
                ++replacement_current;
            }

            Usize replacement_len = replacement_current - replacement - 1;

            begin = self->buffer;
            current =
              self->buffer + current_index + replacement_len - nb_deletion;
        }
    }
}

void
replace_many_sub__String(String *self,
                         const char **subs,
                         const char **replacements,
                         Usize len)
{
    for (Usize i = 0; i < len; ++i) {
        replace_sub__String(self, subs[i], replacements[i]);
    }
}

void
enable_constant_escapes__String(String *self)
{
    replace_sub__String(self, "\\\\", "\\");

    const char *constant_escapes[] = { "\\a", "\\b", "\\e", "\\f", "\\n",
                                       "\\r", "\\t", "\\v", "\\'", "\\?" };
    const char *replacements[] = { "\a", "\b", "\e", "\f", "\n",
                                   "\r", "\t", "\v", "\'", "\?" };

    const Usize constant_escapes_len = LEN(constant_escapes, *constant_escapes);
    const Usize replacements_len = LEN(replacements, *replacements);

    static_assert(constant_escapes_len == replacements_len);

    replace_many_sub__String(
      self, constant_escapes, replacements, constant_escapes_len);
}

void
disable_constant_escapes__String(String *self)
{
    replace_sub__String(self, "\\", "\\\\");

    const char *constant_escapes[] = { "\a", "\b", "\e", "\f", "\n",
                                       "\r", "\t", "\v", "\'", "\?" };
    const char *replacements[] = { "\\a", "\\b", "\\e", "\\f", "\\n",
                                   "\\r", "\\t", "\\v", "\\'", "\\?" };

    const Usize constant_escapes_len = LEN(constant_escapes, *constant_escapes);
    const Usize replacements_len = LEN(replacements, *replacements);

    static_assert(constant_escapes_len == replacements_len);

    replace_many_sub__String(
      self, constant_escapes, replacements, constant_escapes_len);
}

void
reverse__String(String *self)
{
    Usize i = 0;
    Usize j = self->len - 1;

    while (i < j) {
        char tmp = self->buffer[i];
        self->buffer[i] = self->buffer[j];
        self->buffer[j] = tmp;

        i++;
        j--;
    }
}

Vec *
split__String(String *self, char separator)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < self->len; ++i) {
        String *item = NEW(String);

        while (self->buffer[i]) {
            if (self->buffer[i] != separator) {
                push__String(item, self->buffer[i++]);
            } else {
                break;
            }
        }

        push__Vec(res, item);
    }

    return res;
}

String *
take_slice__String(String *self, Usize index)
{
    ASSERT(index < self->len);

    String *res = NEW(String);

    for (Usize i = index; i < self->len; ++i) {
        push__String(res, self->buffer[i]);
    }

    return res;
}

void
ungrow__String(String *self)
{
    if (self->len + 1 <= self->capacity / 2) {
        self->capacity /= 2;
        self->buffer = lily_realloc(self->buffer, self->capacity);
    }
}

void
write_u8__String(String *self, Uint8 v)
{
    push__String(self, (v >> 0) & 0xFF);
}

void
write_u16__String(String *self, Uint16 v)
{
    push__String(self, (v >> 0) & 0xFF);
    push__String(self, (v >> 8) & 0xFF);
}

void
write_u32__String(String *self, Uint32 v)
{
    push__String(self, (v >> 0) & 0xFF);
    push__String(self, (v >> 8) & 0xFF);
    push__String(self, (v >> 16) & 0xFF);
    push__String(self, (v >> 24) & 0xFF);
}

void
write_u64__String(String *self, Uint64 v)
{
    push__String(self, (v >> 0) & 0xFF);
    push__String(self, (v >> 8) & 0xFF);
    push__String(self, (v >> 16) & 0xFF);
    push__String(self, (v >> 24) & 0xFF);
    push__String(self, (v >> 32) & 0xFF);
    push__String(self, (v >> 40) & 0xFF);
    push__String(self, (v >> 48) & 0xFF);
    push__String(self, (v >> 56) & 0xFF);
}

DESTRUCTOR(String, String *self)
{
    lily_free(self->buffer);
    lily_free(self);
}
