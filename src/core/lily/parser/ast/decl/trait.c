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

#include <core/lily/parser/ast/decl/trait.h>

#ifdef ENV_DEBUG
#include <base/alloc.h>
#include <base/format.h>
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclTrait, const LilyAstDeclTrait *self)
{
    String *res = format__String(
      "LilyAstDeclTrait{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyAstGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", inherit_params =");

    if (self->inherit_params) {
        DEBUG_VEC_STRING(self->inherit_params, res, LilyAstInheritParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyAstBodyTraitItem);

    {
        char *s = format(", visibility = {s}, is_close = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_close);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyAstDeclTrait, const LilyAstDeclTrait *self)
{
    FREE(String, self->name);

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyAstGenericParam);
        FREE(Vec, self->generic_params);
    }

    if (self->inherit_params) {
        FREE_BUFFER_ITEMS(self->inherit_params->buffer,
                          self->inherit_params->len,
                          LilyAstInheritParam);
        FREE(Vec, self->inherit_params);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyAstBodyTraitItem);
    FREE(Vec, self->body);
}
