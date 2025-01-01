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

#include <core/lily/analysis/checked/decl/record.h>
#include <core/lily/analysis/checked/generic_param.h>

bool
field_exists__LilyCheckedDeclRecord(const LilyCheckedDeclRecord *self,
                                    String *name)
{
    for (Usize i = 0; i < self->fields->len; ++i) {
        if (!strcmp(
              CAST(LilyCheckedField *, get__Vec(self->fields, i))->name->buffer,
              name->buffer)) {
            return true;
        }
    }

    return false;
}

LilyCheckedDataType *
get_data_type_from_field_name__LilyCheckedDeclRecord(
  const LilyCheckedDeclRecord *self,
  String *name)
{
    for (Usize i = 0; i < self->fields->len; ++i) {
        LilyCheckedField *field = get__Vec(self->fields, i);

        if (!strcmp(field->name->buffer, name->buffer)) {
            return field->data_type;
        }
    }

    return NULL;
}

Usize
get_id_from_field_name__LilyCheckedDeclRecord(const LilyCheckedDeclRecord *self,
                                              String *name)
{
    for (Usize i = 0; i < self->fields->len; ++i) {
        LilyCheckedField *field = get__Vec(self->fields, i);

        if (!strcmp(field->name->buffer, name->buffer)) {
            return i;
        }
    }

    return self->fields->len;
}

#ifdef ENV_DEBUG
#include <base/format.h>

String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclRecord,
               const LilyCheckedDeclRecord *self)
{
    String *res = format__String(
      "LilyCheckedDeclRecord{{ name = {S}, global_name = {S}, generic_params =",
      self->name,
      self->global_name);

    if (self->generic_params) {
        DEBUG_VEC_STRING(self->generic_params, res, LilyCheckedGenericParam);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", fields =");
    DEBUG_VEC_STRING(self->fields, res, LilyCheckedField);

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

DESTRUCTOR(LilyCheckedDeclRecord, const LilyCheckedDeclRecord *self)
{
    FREE(String, self->global_name);

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(self->generic_params->buffer,
                          self->generic_params->len,
                          LilyCheckedGenericParam);
        FREE(Vec, self->generic_params);
    }

    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyCheckedField);
    FREE(Vec, self->fields);

    FREE_BUFFER_ITEMS(self->signatures->buffer,
                      self->signatures->len,
                      LilyCheckedSignatureType);
    FREE(Vec, self->signatures);

    FREE(LilyCheckedScope, self->scope);
}
