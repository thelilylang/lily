/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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
