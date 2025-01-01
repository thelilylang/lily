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

#include <core/lily/analysis/checked/body/enum_object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyCheckedBodyEnumObjectItem type
// (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                          constant,
                          LilyCheckedBodyEnumObjectItem *self);

// (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                          method,
                          LilyCheckedBodyEnumObjectItem *self);

// Free LilyCheckedBodyEnumObjectItem type
// (LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT).
static VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                          variant,
                          LilyCheckedBodyEnumObjectItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyEnumObjectItemKind,
               enum LilyCheckedBodyEnumObjectItemKind self)
{
    switch (self) {
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            return "LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT";
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            return "LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD";
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            return "LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    constant,
                    const Location *location,
                    LilyCheckedDeclConstant constant)
{
    LilyCheckedBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyEnumObjectItem));

    self->kind = LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    method,
                    const Location *location,
                    LilyCheckedDeclMethod method)
{
    LilyCheckedBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyEnumObjectItem));

    self->kind = LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedBodyEnumObjectItem *,
                    LilyCheckedBodyEnumObjectItem,
                    variant,
                    const Location *location,
                    LilyCheckedVariant *variant)
{
    LilyCheckedBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyCheckedBodyEnumObjectItem));

    self->kind = LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT;
    self->location = location;
    self->variant = variant;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedBodyEnumObjectItem,
               const LilyCheckedBodyEnumObjectItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            return format__String(
              "LilyCheckedBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "constant = {Sr} }",
              to_string__Debug__LilyCheckedBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedDeclConstant(&self->constant));
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            return format__String(
              "LilyCheckedBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "method = {Sr} }",
              to_string__Debug__LilyCheckedBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedDeclMethod(&self->method));
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            return format__String(
              "LilyCheckedBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "variant = {Sr} }",
              to_string__Debug__LilyCheckedBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(self->location),
              to_string__Debug__LilyCheckedVariant(self->variant));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                   constant,
                   LilyCheckedBodyEnumObjectItem *self)
{
    FREE(LilyCheckedDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                   method,
                   LilyCheckedBodyEnumObjectItem *self)
{
    FREE(LilyCheckedDeclMethod, &self->method);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyCheckedBodyEnumObjectItem,
                   variant,
                   LilyCheckedBodyEnumObjectItem *self)
{
    FREE(LilyCheckedVariant, self->variant);
    lily_free(self);
}

DESTRUCTOR(LilyCheckedBodyEnumObjectItem, LilyCheckedBodyEnumObjectItem *self)
{
    switch (self->kind) {
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyCheckedBodyEnumObjectItem, constant, self);
            break;
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyCheckedBodyEnumObjectItem, method, self);
            break;
        case LILY_CHECKED_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            FREE_VARIANT(LilyCheckedBodyEnumObjectItem, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
