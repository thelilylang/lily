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

#include <base/macros.h>
#include <base/str.h>
#include <base/types.h>

#include <stdlib.h>
#include <string.h>

char **
split__Str(const char *self, char separator)
{

    char **res = malloc(PTR_SIZE);
    Usize res_size = 0;

    for (Usize i = 0; i < strlen(self); i++) {
        char *item = malloc(1);
        item[0] = '\0';
        Usize item_size = 0;

        while (self[i]) {
            if (self[i] != separator) {
                item = realloc(item, item_size + 2);
                item[item_size] = self[i++];
                item[++item_size] = '\0';
            } else {
                break;
            }
        }

        if (res_size == 0)
            res[res_size++] = item;
        else {
            res = realloc(res, PTR_SIZE * ++res_size);
            res[res_size - 1] = item;
        }
    }

    return res;
}