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

#include <base/assert.h>
#include <base/command.h>
#include <base/file.h>
#include <base/format.h>
#include <base/new.h>
#include <base/path.h>
#include <base/platform.h>

#include <core/cc/ci/diagnostic/emit.h>
#include <core/cc/ci/include.h>

#include <stdio.h>
#include <stdlib.h>

static Vec *include_dirs = NULL; // Vec<String*>*?

void
init_include_dirs__CIInclude(const String *compiler_command,
                             const char *base_path)
{
    ASSERT(compiler_command);

#ifdef LILY_WASM_OS
    // There is no compiler to ask on WASM, and no system include directories
    // for it to report: what the transpiler can see is exactly what the caller
    // put in the virtual filesystem, plus whatever `-I` names.
    include_dirs = init__Vec(1, from__String((char *)base_path));
#else
    char *command =
      format("echo | {S} -E -Wp,-v - 2>&1 | grep \"^ \" | sed 's/^ *//'",
             compiler_command);
    int command_exit_status;
    String *include_dirs_s = save__Command(command, &command_exit_status);

    if (command_exit_status != EXIT_OK) {
        FATAL_ERROR__CI(
          NEW(CIError, CI_ERROR_KIND_FAILED_TO_FETCH_DEFAULT_INCLUDE_PATHS));
    }

    Vec *split_include_dirs_s = split__String(include_dirs_s, '\n');

    include_dirs = init__Vec(1, from__String((char *)base_path));

    lily_free(command);

    for (Usize i = 0; i < split_include_dirs_s->len; ++i) {
        push__Vec(include_dirs, get__Vec(split_include_dirs_s, i));
    }

    FREE(String, include_dirs_s);
    FREE(Vec, split_include_dirs_s);
#endif
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

bool
has_include__CIInclude(const String *include_path,
                       const char *current_filename,
                       bool is_next)
{
    ASSERT(include_dirs);

    String *current_dir = get_dir__File(current_filename);
    // The `_next` form passes over everything up to and including the
    // directory the file it is written in was found in.
    Usize begin = 0;

    if (is_next) {
        for (Usize i = 0; i < include_dirs->len; ++i) {
            const String *include_dir = get__Vec(include_dirs, i);

            if (!strcmp(include_dir->buffer, current_dir->buffer)) {
                begin = i + 1;

                break;
            }
        }
    }

    bool res = false;

    for (Usize i = begin; i < include_dirs->len && !res; ++i) {
        const String *include_dir = get__Vec(include_dirs, i);
        char *full_include_path = format("{S}/{S}", include_dir, include_path);

        res = exists__File(full_include_path);

        lily_free(full_include_path);
    }

    // The directory the file is written in is looked into last, as
    // `resolve_preprocessor_include__CIResolver` does, and never by the `_next`
    // form: that one is written to reach past it.
    if (!res && !is_next) {
        char *full_include_path = format("{S}/{S}", current_dir, include_path);

        res = exists__File(full_include_path);

        lily_free(full_include_path);
    }

    FREE(String, current_dir);

    return res;
}

void
destroy__CIInclude()
{
    if (include_dirs) {
        FREE_BUFFER_ITEMS(include_dirs->buffer, include_dirs->len, String);
        FREE(Vec, include_dirs);
    }
}
