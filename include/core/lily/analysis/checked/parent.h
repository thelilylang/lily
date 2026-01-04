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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PARENT_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PARENT_H

#include <base/alloc.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/scope_stmt.h>

typedef struct LilyCheckedDecl LilyCheckedDecl;
typedef struct LilyCheckedDeclModule LilyCheckedDeclModule;
typedef struct LilyCheckedScope LilyCheckedScope;

enum LilyCheckedParentKind
{
    LILY_CHECKED_PARENT_KIND_DECL,
    LILY_CHECKED_PARENT_KIND_MODULE,
    LILY_CHECKED_PARENT_KIND_SCOPE,
    LILY_CHECKED_PARENT_KIND_STMT
};

/**
 *
 * @brief Convert LilyCheckedParentKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedParentKind,
               enum LilyCheckedParentKind self);
#endif

typedef struct LilyCheckedParent
{
    enum LilyCheckedParentKind kind;
    LilyCheckedScope *scope; // LilyCheckedScope* (&)
    union
    {
        LilyCheckedDecl *decl;         // LilyCheckedDecl* (&)
        LilyCheckedDeclModule *module; // LilyCheckedDeclModule* (&)
        Vec *scope_body;               // Vec<LilyCheckedBodyFunItem*>* (&)
        LilyCheckedScopeStmt stmt;
    };
} LilyCheckedParent;

/**
 *
 * @brief Construct LilyCheckedParent type (LILY_CHECKED_PARENT_KIND_DECL).
 */
VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    decl,
                    LilyCheckedScope *scope,
                    LilyCheckedDecl *decl);

/**
 *
 * @brief Construct LilyCheckedParent type (LILY_CHECKED_PARENT_KIND_MODULE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    module,
                    LilyCheckedScope *scope,
                    LilyCheckedDeclModule *module);

/**
 *
 * @brief Construct LilyCheckedParent type (LILY_CHECKED_PARENT_KIND_SCOPE).
 */
VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    scope,
                    LilyCheckedScope *scope,
                    Vec *scope_body);

/**
 *
 * @brief Construct LilyCheckedParent type (LILY_CHECKED_PARENT_KIND_STMT).
 */
VARIANT_CONSTRUCTOR(LilyCheckedParent *,
                    LilyCheckedParent,
                    stmt,
                    LilyCheckedScope *scope,
                    LilyCheckedScopeStmt stmt);

/**
 *
 * @brief Convert LilyCheckedParent in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedParent, const LilyCheckedParent *self);
#endif

/**
 *
 * @brief Free LilyCheckedParent type.
 */
inline DESTRUCTOR(LilyCheckedParent, LilyCheckedParent *self)
{
    lily_free(self);
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PARENT_H
