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

#include <core/lily/parser/ast/body/trait.h>

#include <stdio.h>
#include <stdlib.h>

// Free LilyAstBodyTraitItem type (LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyAstBodyTraitItem,
                          attribute,
                          LilyAstBodyTraitItem *self);

// Free LilyAstBodyTraitItem type (LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyAstBodyTraitItem,
                          prototype,
                          LilyAstBodyTraitItem *self);

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyTraitItemKind,
               enum LilyAstBodyTraitItemKind self)
{
    switch (self) {
        case LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            return "LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE";
        case LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            return "LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyAstBodyTraitItem *,
                    LilyAstBodyTraitItem,
                    attribute,
                    Location location,
                    LilyAstDeclAttribute attribute)
{
    LilyAstBodyTraitItem *self = lily_malloc(sizeof(LilyAstBodyTraitItem));

    self->kind = LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

/**
 *
 * @brief Construct LilyAstBodyTraitItem type
 * (LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE).
 */
VARIANT_CONSTRUCTOR(LilyAstBodyTraitItem *,
                    LilyAstBodyTraitItem,
                    prototype,
                    Location location,
                    LilyAstDeclPrototype prototype)
{
    LilyAstBodyTraitItem *self = lily_malloc(sizeof(LilyAstBodyTraitItem));

    self->kind = LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE;
    self->location = location;
    self->prototype = prototype;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyAstBodyTraitItem,
               const LilyAstBodyTraitItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            return format__String(
              "LilyAstBodyTraitItem{{ kind = {s}, location = {sa}, attribute = "
              "{Sr} }",
              to_string__Debug__LilyAstBodyTraitItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclAttribute(&self->attribute));
        case LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            return format__String(
              "LilyAstBodyTraitItem{{ kind = {s}, location = {sa}, prototype = "
              "{Sr} }",
              to_string__Debug__LilyAstBodyTraitItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyAstDeclPrototype(&self->prototype));
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyAstBodyTraitItem, attribute, LilyAstBodyTraitItem *self)
{
    FREE(LilyAstDeclAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyAstBodyTraitItem, prototype, LilyAstBodyTraitItem *self)
{
    FREE(LilyAstDeclPrototype, &self->prototype);
    lily_free(self);
}

DESTRUCTOR(LilyAstBodyTraitItem, LilyAstBodyTraitItem *self)
{
    switch (self->kind) {
        case LILY_AST_BODY_TRAIT_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyAstBodyTraitItem, attribute, self);
            break;
        case LILY_AST_BODY_TRAIT_ITEM_KIND_PROTOTYPE:
            FREE_VARIANT(LilyAstBodyTraitItem, prototype, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}
