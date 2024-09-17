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

#include <base/assert.h>
#include <base/command.h>
#include <base/format.h>
#include <base/new.h>
#include <base/path.h>

#include <core/cc/ci/include.h>

#include <stdio.h>
#include <stdlib.h>

static Vec *include_dirs = NULL; // Vec<String*>*?

void
init_include_dirs__CIInclude(const String *compiler_path, const char *base_path)
{
    ASSERT(compiler_path);

    char *command =
      format("echo | {S} -E -Wp,-v - 2>&1 | grep \"^ \" | sed 's/^ *//'",
             compiler_path);
    String *include_dirs_s = save__Command(command);
    Vec *split_include_dirs_s = split__String(include_dirs_s, '\n');

    include_dirs = init__Vec(1, from__String((char *)base_path));

    lily_free(command);

    for (Usize i = 0; i < split_include_dirs_s->len; ++i) {
        push__Vec(include_dirs, get__Vec(split_include_dirs_s, i));
    }

    FREE(String, include_dirs_s);
    FREE(Vec, split_include_dirs_s);
}

void
add_include_dir__CIInclude(String *include_dir)
{
    ASSERT(include_dirs);

    push__Vec(include_dirs, include_dir);
}

void
insert_include_dir__CIInclude(String *include_dir, Usize index)
{
    ASSERT(include_dirs);

    insert__Vec(include_dirs, include_dir, index);
}

const Vec *
get_include_dirs__CIInclude()
{
    // NOTE: You need to call `init_include_dirs__CIInclude` function, before to
    // use this function.
    ASSERT(include_dirs);

    return include_dirs;
}

void
destroy__CIInclude()
{
    FREE_BUFFER_ITEMS(include_dirs->buffer, include_dirs->len, String);
    FREE(Vec, include_dirs);
}
