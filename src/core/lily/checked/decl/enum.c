
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

#include <core/lily/checked/decl/enum.h>
#include <core/lily/checked/generic_param.h>
#include <core/lily/checked/signature.h>

#ifdef ENV_DEBUG
#include <base/format.h>

String *
IMPL_FOR_DEBUG(to_string, LilyCheckedDeclEnum, const LilyCheckedDeclEnum *self)
{
    String *res = format__String(
      "LilyCheckedDeclEnum{{ name = {S}, global_name = {S}, generic_params =",
      self->name,
      self->global_name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyCheckedGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", variants =");
    DEBUG_VEC_STRING(self->variants, res, LilyCheckedVariant);

    push_str__String(res, ", signatures =");
    DEBUG_VEC_STRING(self->signatures, res, LilyCheckedSignatureType);

    {
        char *s = format(", scope = {Sr}, visibility = {s}, is_checked = {b}, "
                         "is_recursive = {b} }",
                         to_string__Debug__LilyCheckedScope(self->scope),
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_checked,
                         self->is_recursive);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedDeclEnum, const LilyCheckedDeclEnum *self)
{
    FREE(String, self->global_name);

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyCheckedGenericParam);
        FREE(Vec, self->generic_params);
    }

    FREE_BUFFER_ITEMS(
      self->variants->buffer, self->variants->len, LilyCheckedVariant);
    FREE(Vec, self->variants);

    FREE_BUFFER_ITEMS(self->signatures->buffer,
                      self->signatures->len,
                      LilyCheckedSignatureType);
    FREE(Vec, self->signatures);

    FREE(LilyCheckedScope, self->scope);
}
