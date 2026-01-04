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
#include <base/format.h>

#include <core/lily/parser/ast/field.h>

CONSTRUCTOR(LilyAstField *,
            LilyAstField,
            String *name,
            LilyAstDataType *data_type,
            LilyAstExpr *optional_expr,
            bool is_mut,
            Location location)
{
    LilyAstField *self = lily_malloc(sizeof(LilyAstField));

    self->name = name;
    self->data_type = data_type;
    self->optional_expr = optional_expr;
    self->is_mut = is_mut;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstField, const LilyAstField *self)
{
    String *res = format__String(
      "LilyAstField{{ name = {S}, data_type = {Sr}, optional_expr =",
      self->name,
      to_string__Debug__LilyAstDataType(self->data_type));

    if (self->optional_expr) {
        String *s = to_string__Debug__LilyAstExpr(self->optional_expr);

        APPEND_AND_FREE(res, s);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", is_mut = {b}, location = {sa} }",
                         self->is_mut,
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyAstField, LilyAstField *self)
{
    FREE(String, self->name);
    FREE(LilyAstDataType, self->data_type);

    if (self->optional_expr) {
        FREE(LilyAstExpr, self->optional_expr);
    }

    lily_free(self);
}
