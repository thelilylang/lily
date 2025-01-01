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

#include <core/lily/parser/ast/body/class.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstBodyClassItem type (LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyAstBodyClassItem,
                          attribute,
                          LilyAstBodyClassItem *self);

// Free LilyAstBodyClassItem type (LILY_AST_BODY_CLASS_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyAstBodyClassItem,
                          method,
                          LilyAstBodyClassItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyClassItemKind,
               enum LilyAstBodyClassItemKind self)
{
    switch (self) {
        case LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            return "LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE";
        case LILY_AST_BODY_CLASS_ITEM_KIND_METHOD:
            return "LILY_AST_BODY_CLASS_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstBodyClassItem *,
                    LilyAstBodyClassItem,
                    attribute,
                    Location location,
                    LilyAstDeclAttribute attribute)
{
    LilyAstBodyClassItem *self = lily_malloc(sizeof(LilyAstBodyClassItem));

    self->kind = LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

VARIANT_CONSTRUCTOR(LilyAstBodyClassItem *,
                    LilyAstBodyClassItem,
                    method,
                    Location location,
                    LilyAstDeclMethod method)
{
    LilyAstBodyClassItem *self = lily_malloc(sizeof(LilyAstBodyClassItem));

    self->kind = LILY_AST_BODY_CLASS_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyClassItem,
               const LilyAstBodyClassItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            return format__String(
              "LilyAstBodyClassItem{{ kind = {s}, location = {sa}, attribute = "
              "{Sr} }",
              to_string__Debug__LilyAstBodyClassItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclAttribute(&self->attribute));
        case LILY_AST_BODY_CLASS_ITEM_KIND_METHOD:
            return format__String(
              "LilyAstBodyClassItem{{ kind = {s}, location = {sa}, method = "
              "{Sr} }",
              to_string__Debug__LilyAstBodyClassItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclMethod(&self->method));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstBodyClassItem, attribute, LilyAstBodyClassItem *self)
{
    FREE(LilyAstDeclAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyClassItem, method, LilyAstBodyClassItem *self)
{
    FREE(LilyAstDeclMethod, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyAstBodyClassItem, LilyAstBodyClassItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_CLASS_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyAstBodyClassItem, attribute, self);
            break;
        case LILY_AST_BODY_CLASS_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyAstBodyClassItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
