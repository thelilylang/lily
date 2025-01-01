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
#include <base/new.h>
#include <base/platform.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/package/default_path.h>

#include <stdio.h>
#include <stdlib.h>

char *
generate_default_path(char *filename_base)
{
    char *default_path = NULL;

    String *filename_string = from__String(filename_base);

#ifdef LILY_WINDOWS_OS
    Vec *filename_string_split = split__String(filename_string, '\\');
#else
    Vec *filename_string_split = split__String(filename_string, '/');
#endif

    FREE(String, pop__Vec(filename_string_split));

#ifdef LILY_WINDOWS_OS
    String *filename_string_joined = join__Vec(filename_string_split, '\\');
#else
    String *filename_string_joined = join__Vec(filename_string_split, '/');
#endif

    default_path = filename_string_joined->buffer;

    FREE(String, filename_string);
    FREE_BUFFER_ITEMS(
      filename_string_split->buffer, filename_string_split->len, String);
    FREE(Vec, filename_string_split);
    lily_free(filename_string_joined);

    return default_path;
}
