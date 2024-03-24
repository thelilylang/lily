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
#include <base/new.h>

#include <core/cc/ci/include.h>

#include <stdio.h>
#include <stdlib.h>

#if defined(LILY_LINUX_OS) || defined(LILY_APPLE_OS) || defined(LILY_BSD_OS)
static const char *default_include_dirs[] = { ".", "/usr/include" };
static Usize default_include_dirs_len = 2;
#elifdef LILY_WINDOWS_OS
static const char *default_include_dirs[] = { "." };
static Usize default_include_dirs_len = 1;
#else
#error "this OS is not yet supported"
#endif

static Vec *include_dirs = NULL; // Vec<char* (&)>*?

void
init_include_dirs__CIInclude()
{
    include_dirs = NEW(Vec);

    for (Usize i = 0; i < default_include_dirs_len; ++i) {
        push__Vec(include_dirs, (char *)default_include_dirs[i]);
    }
}

void
add_include_dir__CIInclude(char *include_dir)
{
    ASSERT(include_dirs);

    push__Vec(include_dirs, include_dir);
}

const Vec *
get_include_dirs__CIInclude()
{
    return include_dirs;
}

void
destroy__CIInclude()
{
    FREE(Vec, include_dirs);
}
