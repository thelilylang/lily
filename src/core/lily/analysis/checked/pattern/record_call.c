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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, PATTERNESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <base/alloc.h>
#include <base/new.h>

#include <core/lily/analysis/checked/pattern.h>
#include <core/lily/analysis/checked/pattern/record_call.h>

CONSTRUCTOR(LilyCheckedPatternRecordField *,
            LilyCheckedPatternRecordField,
            String *name,
            LilyCheckedPattern *pattern)
{
    LilyCheckedPatternRecordField *self =
      lily_malloc(sizeof(LilyCheckedPatternRecordField));

    self->name = name;
    self->pattern = pattern;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternRecordField,
               const LilyCheckedPatternRecordField *self)
{
    if (self->name) {
        return format__String(
          "LilyCheckedPatternRecordField{{ name = {S}, pattern = {Sr} }",
          self->name,
          to_string__Debug__LilyCheckedPattern(self->pattern));
    }

    return format__String(
      "LilyCheckedPatternRecordField{{ name = NULL, pattern = {Sr} }",
      to_string__Debug__LilyCheckedPattern(self->pattern));
}
#endif

DESTRUCTOR(LilyCheckedPatternRecordField, LilyCheckedPatternRecordField *self)
{
    FREE(LilyCheckedPattern, self->pattern);
    lily_free(self);
}

bool
eq__LilyCheckedPatternRecordCall(const LilyCheckedPatternRecordCall *self,
                                 const LilyCheckedPatternRecordCall *other)
{
    // TODO: cmp id
    // TODO: improve

    if (self->fields->len != other->fields->len) {
        return false;
    }

    for (Usize i = 0; i < self->fields->len; ++i) {
        LilyCheckedPatternRecordField *self_field = get__Vec(self->fields, i);
        LilyCheckedPatternRecordField *other_field = get__Vec(other->fields, i);

        if (self_field->name && other_field->name) {
            if (!(!strcmp(self_field->name->buffer,
                          other_field->name->buffer) &&
                  eq__LilyCheckedPattern(self_field->pattern,
                                         other_field->pattern))) {
                return false;
            }
        } else {
            return false;
        }
    }

    return true;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedPatternRecordCall,
               const LilyCheckedPatternRecordCall *self)
{
    String *res = NULL;

    if (self->id) {
        res =
          format__String("LilyCheckedPatternRecordCall{{ id = {Sr}, fields =",
                         to_string__Debug__LilyCheckedExpr(self->id));
    } else {
        res = from__String("LilyCheckedPatternRecordCall{ id = NULL, fields =");
    }

    DEBUG_VEC_STRING(self->fields, res, LilyCheckedPatternRecordField);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedPatternRecordCall,
           const LilyCheckedPatternRecordCall *self)
{
    if (self->id) {
        FREE(LilyCheckedExpr, self->id);
    }

    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyCheckedPatternRecordField);
    FREE(Vec, self->fields);
}
