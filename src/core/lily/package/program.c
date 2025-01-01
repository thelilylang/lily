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

#include <base/assert.h>

#include <core/lily/package/library.h>
#include <core/lily/package/program.h>

#include <stdio.h>
#include <stdlib.h>

CONSTRUCTOR(LilyProgram, LilyProgram, enum LilyProgramKind kind)
{
    return (LilyProgram){ .kind = kind,
                          .resources = NEW(LilyProgramResources) };
}

DESTRUCTOR(LilyProgramResources, const LilyProgramResources *self)
{
    for (Usize i = 0; i < BUILTINS_COUNT; ++i) {
        FREE(LilyBuiltinFun, &self->builtins[i]);
    }

    lily_free(self->builtins);

    for (Usize i = 0; i < SYSS_COUNT; ++i) {
        FREE(LilySysFun, &self->syss[i]);
    }

    lily_free(self->syss);

    // FIXME: sometimes that's crash, caused by the add of the the
    // multi-threading in the precompiler

    for (Usize i = 0; i < DEFAULT_OPERATORS_COUNT; ++i) {
        ASSERT(self->default_operators[i]->ref_count == 0);

        FREE(LilyCheckedOperator, self->default_operators[i]);
    }

    lily_free(self->default_operators);

    FREE_BUFFER_ITEMS(self->libs->buffer, self->libs->len, LilyLibrary);
    FREE(Vec, self->libs);
}
