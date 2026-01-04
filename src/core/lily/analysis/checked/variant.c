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

#include <core/lily/analysis/checked/signature.h>
#include <core/lily/analysis/checked/variant.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyCheckedVariant *,
            LilyCheckedVariant,
            String *name,
            String *global_name,
            LilyCheckedDataType *data_type,
            const Location *location,
            const LilyCheckedDecl *enum_,
            Usize id)
{
    LilyCheckedVariant *self = lily_malloc(sizeof(LilyCheckedVariant));

    self->name = name;
    self->global_name = global_name;
    self->data_type = data_type;
    self->location = location;
    self->signatures = NEW(Vec);
    self->enum_ = enum_;
    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedVariant, const LilyCheckedVariant *self)
{
    // TODO: improve this debug
    if (self->data_type) {
        String *res = format__String(
          "LilyCheckedVariant{{ name = {S}, global_name = {S}, data_type = "
          "{Sr}, location = {sa}, signatures =",
          self->name,
          self->global_name,
          to_string__Debug__LilyCheckedDataType(self->data_type),
          to_string__Debug__Location(self->location));

        DEBUG_VEC_STRING(self->signatures, res, LilyCheckedSignatureVariant);

        {
            char *s = format(", enum_ = {{...}, id = {zu} }", self->id);

            PUSH_STR_AND_FREE(res, s);
        }

        return res;
    }

    String *res =
      format__String("LilyCheckedVariant{{ name = {S}, global_name = {S}, "
                     "data_type = NULL, location = {sa}, signatures =",
                     self->name,
                     self->global_name,
                     to_string__Debug__Location(self->location));

    DEBUG_VEC_STRING(self->signatures, res, LilyCheckedSignatureVariant);

    {
        char *s = format(", enum_ = {{...}, id = {zu} }", self->id);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyCheckedVariant, LilyCheckedVariant *self)
{
    FREE(String, self->global_name);

    if (self->data_type) {
        FREE(LilyCheckedDataType, self->data_type);
    }

    FREE_BUFFER_ITEMS(self->signatures->buffer,
                      self->signatures->len,
                      LilyCheckedSignatureVariant);
    FREE(Vec, self->signatures);

    lily_free(self);
}
