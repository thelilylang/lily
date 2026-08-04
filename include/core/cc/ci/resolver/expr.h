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

#ifndef LILY_CORE_CC_CI_RESOLVER_EXPR_H
#define LILY_CORE_CC_CI_RESOLVER_EXPR_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/parser.h>

// A name bound to the value it holds before the program runs, as the counter
// of an unrolled loop is, or as a param written `constexpr` is.
//
// The value is held as the expression it was read as, which is a literal of
// whichever kind it was written with: a number as much as a character, a
// floating value or a string. Reading it is what says what it holds, so
// nothing here is written on one kind of value alone.
typedef struct CIComptimeBinding
{
    const Rc *name; // Rc<String*>* (&)
    CIExpr *value;  // CIExpr* A literal.
} CIComptimeBinding;

/**
 *
 * @brief Construct CIComptimeBinding type.
 * @param value CIExpr* The binding takes it over.
 */
CONSTRUCTOR(CIComptimeBinding *,
            CIComptimeBinding,
            const Rc *name,
            CIExpr *value);

/**
 *
 * @brief Free CIComptimeBinding type.
 */
DESTRUCTOR(CIComptimeBinding, CIComptimeBinding *self);

typedef struct CIResolverExpr
{
    const CIParser *parser; // const CIParser*? (&)
    const CIScope *scope;   // const CIScope*? (&)
    // File and error count are held directly, rather than reached through
    // `parser`, because `parser` is NULL when resolving at preprocessor-time.
    const CIResultFile *file; // const CIResultFile* (&)
    Usize *count_error;       // Usize* (&)
    bool is_at_preprocessor_time;
    // The names that are known before the program runs. A name is looked for
    // here before it is looked for as a declaration, so the innermost one
    // written on it is the one it holds. NULL where nothing is known, which
    // is everywhere but an unrolled loop.
    const Vec *comptime_env; // const Vec<CIComptimeBinding*>*? (&)
} CIResolverExpr;

/**
 *
 * @brief Say what the names known before the program runs are.
 * @param comptime_env const Vec<CIComptimeBinding*>*? (&)
 */
inline void
set_comptime_env__CIResolverExpr(CIResolverExpr *self, const Vec *comptime_env)
{
    self->comptime_env = comptime_env;
}

/**
 *
 * @brief Look for the value a name holds before the program runs.
 * @param self const Vec<CIComptimeBinding*>*? (&)
 * @return CIExpr*? (&) The literal it holds, or NULL where the name is
 * one nothing is known of.
 */
CIExpr *
search_comptime_binding__CIResolverExpr(const Vec *self, const String *name);

/**
 *
 * @brief Construct CIResolverExpr type.
 */
inline CONSTRUCTOR(CIResolverExpr,
                   CIResolverExpr,
                   const CIParser *parser,
                   const CIScope *scope,
                   const CIResultFile *file,
                   Usize *count_error,
                   bool is_at_preprocessor_time)
{
    return (CIResolverExpr){ .parser = parser,
                             .scope = scope,
                             .file = file,
                             .count_error = count_error,
                             .is_at_preprocessor_time =
                               is_at_preprocessor_time };
}

/**
 *
 * @brief Check if the expression is true.
 */
bool
is_true__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr);

/**
 *
 * @brief Check if the expression is NULL.
 */
bool
is_null__CIResolverExpr(CIExpr *expr);

/**
 *
 * @brief Convert resolved expression to literal integer value.
 */
Isize
to_literal_integer_value__CIResolverExpr(const CIResolverExpr *self,
                                         CIExpr *expr);

/**
 *
 * @brief Resolve preprocessor condition.
 */
CIExpr *
run__CIResolverExpr(const CIResolverExpr *self, CIExpr *expr);

#endif // LILY_CORE_CC_CI_RESOLVER_EXPR_H
