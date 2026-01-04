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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_DECLS_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_DECLS_H

#include <base/vec.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/scope_stmt.h>

typedef struct LilyCheckedDecl LilyCheckedDecl;
typedef struct LilyCheckedDeclModule LilyCheckedDeclModule;

// TODO: add an other kind of decls for the lambda statement
enum LilyCheckedScopeDeclsKind
{
    LILY_CHECKED_SCOPE_DECLS_KIND_DECL,
    LILY_CHECKED_SCOPE_DECLS_KIND_MODULE,
    LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE,
    LILY_CHECKED_SCOPE_DECLS_KIND_STMT
};

/**
 *
 * @brief Convert LilyCheckedScopeDeclsKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeDeclsKind,
               enum LilyCheckedScopeDeclsKind self);
#endif

typedef struct LilyCheckedScopeDecls
{
    enum LilyCheckedScopeDeclsKind kind;
    union
    {
        LilyCheckedDecl *decl;         // LilyCheckedDecl* (&)
        LilyCheckedDeclModule *module; // LilyCheckedDeclModule* (&)
        Vec *scope;                    // Vec<LilyCheckedBodyFunItem*>* (&)
        LilyCheckedScopeStmt stmt;
    };
} LilyCheckedScopeDecls;

/**
 *
 * @brief Construct LilyCheckedScopeDecls type
 * (LILY_CHECKED_SCOPE_DECLS_KIND_DECL).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                           LilyCheckedScopeDecls,
                           decl,
                           LilyCheckedDecl *decl)
{
    return (LilyCheckedScopeDecls){ .kind = LILY_CHECKED_SCOPE_DECLS_KIND_DECL,
                                    .decl = decl };
}

/**
 *
 * @brief Construct LilyCheckedScopeDecls type
 * (LILY_CHECKED_SCOPE_DECLS_KIND_MODULE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                           LilyCheckedScopeDecls,
                           module,
                           LilyCheckedDeclModule *module)
{
    return (LilyCheckedScopeDecls){ .kind =
                                      LILY_CHECKED_SCOPE_DECLS_KIND_MODULE,
                                    .module = module };
}

/**
 *
 * @brief Construct LilyCheckedScopeDecls type
 * (LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                           LilyCheckedScopeDecls,
                           scope,
                           Vec *scope)
{
    return (LilyCheckedScopeDecls){ .kind = LILY_CHECKED_SCOPE_DECLS_KIND_SCOPE,
                                    .scope = scope };
}

/**
 *
 * @brief Construct LilyCheckedScopeDecls type
 * (LILY_CHECKED_SCOPE_DECLS_KIND_STMT).
 */
inline VARIANT_CONSTRUCTOR(LilyCheckedScopeDecls,
                           LilyCheckedScopeDecls,
                           stmt,
                           LilyCheckedScopeStmt stmt)
{
    return (LilyCheckedScopeDecls){ .kind = LILY_CHECKED_SCOPE_DECLS_KIND_STMT,
                                    .stmt = stmt };
}

/**
 *
 * @brief Add unlock data type to a function, a method or a lambda function.
 */
void
add_unlock_data_type__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self,
                                            LilyCheckedDataType *unlock);

/**
 *
 * @brief Set `has_return` on function, method or lambda function.
 */
void
set_has_return__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self);

/**
 *
 * @brief Get return data type from function, method or lambda function.
 */
LilyCheckedDataType *
get_return_data_type__LilyCheckedScopeDecls(const LilyCheckedScopeDecls *self);

/**
 *
 * @brief Get used compiler generic from function, method or lambda function.
 * @return Vec<String*>*
 */
Vec *
get_used_compiler_generic__LilyCheckedScopeDecls(
  const LilyCheckedScopeDecls *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_SCOPE_DECLS_H
