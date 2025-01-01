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

#include <base/alloc.h>

#include <core/lily/parser/ast/variant.h>

CONSTRUCTOR(LilyAstVariant *,
            LilyAstVariant,
            String *name,
            LilyAstDataType *data_type,
            Location location)
{
    LilyAstVariant *self = lily_malloc(sizeof(LilyAstVariant));

    self->name = name;
    self->data_type = data_type;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstVariant, const LilyAstVariant *self)
{
    if (self->data_type) {
        return format__String(
          "LilyAstVariant{{ name = {S}, data_type = {Sr}, location = {sa} }",
          self->name,
          to_string__Debug__LilyAstDataType(self->data_type),
          to_string__Debug__Location(&self->location));
    }

    return format__String(
      "LilyAstVariant{{ name = {S}, data_type = NULL, location = {sa} }",
      self->name,
      to_string__Debug__Location(&self->location));
}
#endif

DESTRUCTOR(LilyAstVariant, LilyAstVariant *self)
{
    FREE(String, self->name);

    if (self->data_type) {
        FREE(LilyAstDataType, self->data_type);
    }

    lily_free(self);
}
