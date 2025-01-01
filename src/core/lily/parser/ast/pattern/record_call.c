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

#include <core/lily/parser/ast/pattern.h>
#include <core/lily/parser/ast/pattern/record_call.h>

CONSTRUCTOR(LilyAstPatternRecordField *,
            LilyAstPatternRecordField,
            String *name,
            LilyAstPattern *pattern)
{
    LilyAstPatternRecordField *self =
      lily_malloc(sizeof(LilyAstPatternRecordField));

    self->name = name;
    self->pattern = pattern;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstPatternRecordField,
               const LilyAstPatternRecordField *self)
{
    if (self->name) {
        return format__String(
          "LilyAstPatternRecordField{{ name = {S}, pattern = {Sr} }",
          self->name,
          to_string__Debug__LilyAstPattern(self->pattern));
    }

    return format__String(
      "LilyAstPatternRecordField{{ name = NULL, pattern = {Sr} }",
      to_string__Debug__LilyAstPattern(self->pattern));
}
#endif

DESTRUCTOR(LilyAstPatternRecordField, LilyAstPatternRecordField *self)
{
    if (self->name) {
        FREE(String, self->name);
    }

    FREE(LilyAstPattern, self->pattern);
    lily_free(self);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstPatternRecordCall,
               const LilyAstPatternRecordCall *self)
{
    String *res = NULL;

    if (self->id) {
        res = format__String("LilyAstPatternRecordCall{{ id = {Sr}, fields =",
                             to_string__Debug__LilyAstExpr(self->id));
    } else {
        res = from__String("LilyAstPatternRecordCall{ id = NULL, fields =");
    }

    DEBUG_VEC_STRING(self->fields, res, LilyAstPatternRecordField);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyAstPatternRecordCall, const LilyAstPatternRecordCall *self)
{
    if (self->id) {
        FREE(LilyAstExpr, self->id);
    }

    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyAstPatternRecordField);
    FREE(Vec, self->fields);
}
