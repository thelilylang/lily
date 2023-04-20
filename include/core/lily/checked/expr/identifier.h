/*
 * MIT License
 *
 * Copyright (c) 2022-2023 ArthurPV
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

#ifndef LILY_CORE_LILY_CHECKED_EXPR_IDENTIFIER_H
#define LILY_CORE_LILY_CHECKED_EXPR_IDENTIFIER_H

#include <base/new.h>
#include <base/string.h>

#include <core/lily/checked/scope.h>

typedef struct LilyCheckedExprIdentifier
{
    String *name;
    LilyCheckedScope scope;
} LilyCheckedExprIdentifier;

/**
 *
 * @brief Construct LilyCheckedExprIdentifier type.
 */
inline CONSTRUCTOR(LilyCheckedExprIdentifier,
                   LilyCheckedExprIdentifier,
                   String *name,
                   LilyCheckedScope scope)
{
    return (LilyCheckedExprIdentifier){ .name = name, .scope = scope };
}

/**
 *
 * @brief Convert LilyCheckedExprIdentifier in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
inline String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprIdentifier,
               const LilyCheckedExprIdentifier *self)
{
    return format__String(
      "LilyCheckedExpridentifier{{ name = {S}, scope = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedScope(&self->scope));
}
#endif

/**
 *
 * @brief Free LilyCheckedExprIdentifier type.
 */
inline DESTRUCTOR(LilyCheckedExprIdentifier,
                  const LilyCheckedExprIdentifier *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
}

typedef struct LilyCheckedExprIdentifierDollar
{
    String *name;
    LilyCheckedScope scope;
} LilyCheckedExprIdentifierDollar;

/**
 *
 * @brief Construct LilyCheckedExprIdentifierDollar type.
 */
inline CONSTRUCTOR(LilyCheckedExprIdentifierDollar,
                   LilyCheckedExprIdentifierDollar,
                   String *name,
                   LilyCheckedScope scope)
{
    return (LilyCheckedExprIdentifierDollar){ .name = name, .scope = scope };
}

/**
 *
 * @brief Convert LilyCheckedExprIdentifierDollar in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
inline String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedExprIdentifierDollar,
               const LilyCheckedExprIdentifierDollar *self,
               LilyCheckedScope scope)
{
    return format__String(
      "LilyCheckedExpridentifierDollar{{ name = {S}, scope = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedScope(&self->scope));
}
#endif

/**
 *
 * @brief Free LilyCheckedExprIdentifierDollar type.
 */
inline DESTRUCTOR(LilyCheckedExprIdentifierDollar,
                  const LilyCheckedExprIdentifierDollar *self)
{
    FREE_MOVE(self->name, FREE(String, self->name));
}

#endif // LILY_CORE_LILY_CHECKED_EXPR_IDENTIFIER_H
