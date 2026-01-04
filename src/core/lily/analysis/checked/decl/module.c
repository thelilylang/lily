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

#include <base/new.h>

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/decl/module.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclModule,
               const LilyCheckedDeclModule *self)
{
    String *res = format__String(
      "LilyCheckedDeclModule{{ name = {S}, global_name = {S}, decls =",
      self->name,
      self->global_name);

    DEBUG_VEC_STRING(self->decls, res, LilyCheckedDecl);

    {
        char *s = format(", scope = {Sr}, visibility = {s}, is_checked = {b} }",
                         to_string__Debug__LilyCheckedScope(self->scope),
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_checked);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDeclModule, const LilyCheckedDeclModule *self)
{
    if (self->global_name) {
        FREE(String, self->global_name);
    } else {
        UNREACHABLE("Bug on thread!!");
    }

    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyCheckedDecl);
    FREE(Vec, self->decls);

    if (self->scope) {
        FREE(LilyCheckedScope, self->scope);
    } else {
        UNREACHABLE("Bug on thread!!");
    }
}
