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

#include <core/lily/parser/ast/body/enum_object.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstBodyEnumObjectItem type
// (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                          constant,
                          LilyAstBodyEnumObjectItem *self);

// (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                          method,
                          LilyAstBodyEnumObjectItem *self);

// Free LilyAstBodyEnumObjectItem type
// (LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT).
static VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                          variant,
                          LilyAstBodyEnumObjectItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyEnumObjectItemKind,
               enum LilyAstBodyEnumObjectItemKind self)
{
    switch (self) {
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            return "LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT";
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            return "LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD";
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            return "LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    constant,
                    Location location,
                    LilyAstDeclConstant constant)
{
    LilyAstBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyEnumObjectItem));

    self->kind = LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    method,
                    Location location,
                    LilyAstDeclMethod method)
{
    LilyAstBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyEnumObjectItem));

    self->kind = LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyEnumObjectItem *,
                    LilyAstBodyEnumObjectItem,
                    variant,
                    Location location,
                    LilyAstVariant *variant)
{
    LilyAstBodyEnumObjectItem *self =
      lily_malloc(sizeof(LilyAstBodyEnumObjectItem));

    self->kind = LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT;
    self->location = location;
    self->variant = variant;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyEnumObjectItem,
               const LilyAstBodyEnumObjectItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            return format__String(
              "LilyAstBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "constant = {Sr} }",
              to_string__Debug__LilyAstBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclConstant(&self->constant));
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            return format__String(
              "LilyAstBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "method = {Sr} }",
              to_string__Debug__LilyAstBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclMethod(&self->method));
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            return format__String(
              "LilyAstBodyEnumObjectItem{{ kind = {s}, location = {sa}, "
              "variant = {Sr} }",
              to_string__Debug__LilyAstBodyEnumObjectItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstVariant(self->variant));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                   constant,
                   LilyAstBodyEnumObjectItem *self)
{
    FREE(LilyAstDeclConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                   method,
                   LilyAstBodyEnumObjectItem *self)
{
    FREE(LilyAstDeclMethod, &self->method);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyEnumObjectItem,
                   variant,
                   LilyAstBodyEnumObjectItem *self)
{
    FREE(LilyAstVariant, self->variant);
    lily_free(self);
}

DESTRUCTOR(LilyAstBodyEnumObjectItem, LilyAstBodyEnumObjectItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyAstBodyEnumObjectItem, constant, self);
            break;
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyAstBodyEnumObjectItem, method, self);
            break;
        case LILY_AST_BODY_ENUM_OBJECT_ITEM_KIND_VARIANT:
            FREE_VARIANT(LilyAstBodyEnumObjectItem, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
