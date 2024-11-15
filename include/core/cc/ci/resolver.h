/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_CC_CI_RESOLVER_H
#define LILY_CORE_CC_CI_RESOLVER_H

#include <base/vec.h>

#include <core/cc/ci/scanner.h>

typedef struct CIResolvedTokens
{
    Vec *content; // Vec<CIToken*>*
    Usize ref_count;
} CIResolvedTokens;

/**
 *
 * @brief Construct CIResolvedTokens type.
 */
CONSTRUCTOR(CIResolvedTokens *, CIResolvedTokens);

/**
 *
 * @brief Add `token` to `content` field.
 */
inline void
add__CIResolvedTokens(const CIResolvedTokens *self, CIToken *token)
{
    push__Vec(self->content, token);
}

/**
 *
 * @brief Get token from `content` field.
 * @return CIToken* (&)
 */
inline CIToken *
get__CIResolvedTokens(const CIResolvedTokens *self, Usize index)
{
    return get__Vec(self->content, index);
}

/**
 *
 * @brief Get the last token from `content` field.
 * @return CIToken* (&)
 */
inline CIToken *
last__CIResolvedTokens(const CIResolvedTokens *self)
{
    return last__Vec(self->content);
}

/**
 *
 * @brief Replace item of the `content` field at index position.
 */
inline void
replace__CIResolvedTokens(CIResolvedTokens *self, Usize index, CIToken *token)
{
    return replace__Vec(self->content, index, token);
}

/**
 *
 * @brief Remove item from `content` field.
 */
inline CIToken *
remove__CIResolvedTokens(CIResolvedTokens *self, Usize index)
{
    return remove__Vec(self->content, index);
}

/**
 *
 * @brief Get the number of resolved tokens.
 */
inline Usize
count__CIResolvedTokens(const CIResolvedTokens *self)
{
    return self->content->len;
}

/**
 *
 * @brief Merge other in self.
 * @param self const CIResolvedTokens* (&)
 * @param other const CIResolvedTokens* (&)
 */
void
merge__CIResolvedTokens(const CIResolvedTokens *self,
                        const CIResolvedTokens *other);

/**
 *
 * @brief Pop the content of the vector if the type of the last token is EOF.
 */
void
pop_if_eof__CIResolvedTokens(const CIResolvedTokens *self);

/**
 *
 * @brief Increment `ref_count`.
 * @return CIResolvedTokens*
 */
inline CIResolvedTokens *
ref__CIResolvedTokens(CIResolvedTokens *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free CIResolvedTokens type.
 */
DESTRUCTOR(CIResolvedTokens, CIResolvedTokens *self);

typedef struct CIResolverMacroCallParam
{
    CIResolvedTokens *resolved_content;    // CIResolvedTokens*
    struct CIResolverMacroCallParam *next; // CIResolverMacroCallParam*?
} CIResolverMacroCallParam;

/**
 *
 *
 * @brief Construct CIResolverMacroCallParam type.
 */
CONSTRUCTOR(CIResolverMacroCallParam *,
            CIResolverMacroCallParam,
            CIResolvedTokens *resolved_content);

/**
 *
 * @brief Free CIResolverMacroCallParam type.
 */
DESTRUCTOR(CIResolverMacroCallParam, CIResolverMacroCallParam *self);

typedef struct CIResolverMacroCallParams
{
    CIResolverMacroCallParam *first;
    CIResolverMacroCallParam *last; // CIParserMacroCallParam* (&)
    Usize len;
} CIResolverMacroCallParams;

/**
 *
 * @brief Construct CIResolverMacroCallParams type.
 */
inline CONSTRUCTOR(CIResolverMacroCallParams, CIResolverMacroCallParams)
{
    return (CIResolverMacroCallParams){ .first = NULL, .last = NULL, .len = 0 };
}

/**
 *
 * @brief Add a param to params.
 */
void
add__CIResolverMacroCallParams(CIResolverMacroCallParams *self,
                               CIResolverMacroCallParam *param);

/**
 *
 * @brief Get macro call param according the index.
 */
CIResolverMacroCallParam *
get__CIResolverMacroCallParam(const CIResolverMacroCallParam *self,
                              Usize index);

/**
 *
 * @brief Get variadic macro call param.
 */
inline CIResolverMacroCallParam *
get_macro_param_variadic__CIResolverMacroCallParams(
  const CIResolverMacroCallParams *self)
{
    return self->last;
}

/**
 *
 * @brief Free CIResolverMacroCallParams type.
 */
DESTRUCTOR(CIResolverMacroCallParams, const CIResolverMacroCallParams *self);

typedef struct CIResolverMacroCall
{
    // Params of macro call
    CIResolverMacroCallParams params;
    bool is_empty;
    Usize ref_count;
} CIResolverMacroCall;

/**
 *
 * @brief Construct CIResolverMacroCall type (is_empty=false).
 */
CONSTRUCTOR(CIResolverMacroCall *, CIResolverMacroCall);

/**
 *
 * @brief Construct CIResolverMacroCall type (is_empty=true).
 */
VARIANT_CONSTRUCTOR(CIResolverMacroCall *, CIResolverMacroCall, is_empty);

/**
 *
 * @brief Increment `ref_count`.
 * @return CIResolverMacroCall*
 */
inline CIResolverMacroCall *
ref__CIResolverMacroCall(CIResolverMacroCall *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free CIResolverMacroCall type.
 */
DESTRUCTOR(CIResolverMacroCall, CIResolverMacroCall *self);

typedef struct CIResolver
{
    CIResultFile *file;                // CIResultFile* (&)
    const CITokens *tokens;            // const CITokens* (&)
    CIToken *current_token;            // CIToken*? (&)
    CIResolvedTokens *resolved_tokens; // CIResolvedTokens*?
    Usize *count_error;                // Usize* (&)
    Usize *count_warning;              // Usize* (&)
    CIResolverMacroCall *macro_call;   // CIResolverMacroCall*?
    Usize count_merged_id;
} CIResolver;

/**
 *
 * @brief Construct CIResolver type.
 * @param tokens CITokens* (&)
 */
inline CONSTRUCTOR(CIResolver,
                   CIResolver,
                   CIResultFile *file,
                   const CITokens *tokens,
                   Usize *count_error,
                   Usize *count_warning)
{
    return (CIResolver){ .file = file,
                         .tokens = tokens,
                         .current_token = NULL,
                         .resolved_tokens = NULL,
                         .count_error = count_error,
                         .count_warning = count_warning,
                         .macro_call = NULL,
                         .count_merged_id = 0 };
}

/**
 *
 * @param resolved_tokens CIResolvedTokens*? - If the `resolved_tokens` passed
 * is NULL, the default constructor of `ResolvedTokens` will be used.
 * @return Vec<CIToken* (&)>*
 */
void
run__CIResolver(CIResolver *self, CIResolvedTokens *resolved_tokens);

/**
 *
 * @brief Free CIResolver type.
 */
DESTRUCTOR(CIResolver, const CIResolver *self);

#endif // LILY_CORE_CC_CI_RESOLVER_H
