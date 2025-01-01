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

#ifndef LILY_CORE_LILY_PARSER_AST_DECL_TRAIT_H
#define LILY_CORE_LILY_PARSER_AST_DECL_TRAIT_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/parser/ast/body/trait.h>
#include <core/lily/parser/ast/generic_param.h>
#include <core/lily/parser/ast/inherit_param.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyAstDeclTrait
{
    String *name;
    Vec *generic_params; // Vec<LilyAstGenericParam*>*?
    Vec *inherit_params; // Vec<LilyAstInheritParam*>*?
    Vec *body;           // Vec<LilyAstBodyTraitItem*>*
    enum LilyVisibility visibility;
    bool is_close;
} LilyAstDeclTrait;

/**
 *
 * @brief Construct LilyAstDeclTrait type.
 */
inline CONSTRUCTOR(LilyAstDeclTrait,
                   LilyAstDeclTrait,
                   String *name,
                   Vec *generic_params,
                   Vec *inherit_params,
                   Vec *body,
                   enum LilyVisibility visibility,
                   bool is_close)
{
    return (LilyAstDeclTrait){ .name = name,
                               .generic_params = generic_params,
                               .inherit_params = inherit_params,
                               .body = body,
                               .visibility = visibility,
                               .is_close = is_close };
}

/**
 *
 * @brief Convert LilyAstDeclTrait in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyAstDeclTrait, const LilyAstDeclTrait *self);
#endif

/**
 *
 * @brief Free LilyAstDeclTrait type.
 */
DESTRUCTOR(LilyAstDeclTrait, const LilyAstDeclTrait *self);

#endif // LILY_CORE_LILY_PARSER_AST_DECL_TRAIT_H
