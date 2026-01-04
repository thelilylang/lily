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

#include <base/alloc.h>
#include <base/assert.h>
#include <base/macros.h>
#include <base/new.h>
#include <base/str.h>
#include <base/types.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Vec *
split__Str(const char *self, char separator)
{
    Vec *res = NEW(Vec);

    for (Usize i = 0; i < strlen(self); ++i) {
        char *item = lily_malloc(1);
        item[0] = '\0';
        Usize item_size = 0;

        while (self[i]) {
            if (self[i] != separator) {
                item = lily_realloc(item, item_size + 2);
                item[item_size] = self[i++];
                item[++item_size] = '\0';
            } else {
                break;
            }
        }

        push__Vec(res, item);
    }

    return res;
}

char *
get_slice__Str(const char *self, Usize start, Usize end)
{
    ASSERT(start < end);

    Usize size = (end - start) + 1;
    char *res = lily_malloc(size);

    memset(res, 0, size);

    for (Usize i = start, j = 0; i < end; ++i) {
        res[j++] = self[i];
    }

    return res;
}

void
replace__Str(char *self, char target, char replace)
{
    Usize size = strlen(self);

    for (Usize i = 0; i < size; ++i) {
        if (self[i] == target) {
            self[i] = replace;
        }
    }
}

Usize
count_c__Str(const char *self, char c)
{
    Usize count = 0;

    for (char *current = (char *)self; *current; ++current) {
        if (*current == c) {
            ++count;
        }
    }

    return count;
}

char *
strstr_list__Str(const char *self,
                 const char **matches,
                 Usize matches_n,
                 Isize *match_at)
{
    char *res = NULL;
    Usize i = 0;

    for (; i < matches_n && !res; ++i) {
        res = strstr(self, matches[i]);
    }

    if (match_at) {
        *match_at = res ? i - 1 : -1;
    }

    return res;
}
