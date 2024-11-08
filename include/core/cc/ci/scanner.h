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

#ifndef LILY_CORE_CC_CI_SCANNER_H
#define LILY_CORE_CC_CI_SCANNER_H

#include <base/types.h>
#include <base/vec.h>

#include <core/cc/ci/features.h>
#include <core/cc/ci/project_config.h>
#include <core/cc/ci/token.h>
#include <core/shared/scanner.h>

typedef struct CIScanner
{
    CITokens tokens;
    Scanner base;
    const CIProjectConfig *config; // const CIProjectConfig* (&)
    /// A field indicating whether or not the file being scanned is predefined.
    /// This makes it possible to define standard macros such as `__STDC__`,
    /// `__STDC_HOSTED__`, `__STDC_VERSION__`, without error.
    bool is_predefined;
} CIScanner;

/**
 *
 * @brief Construct CIScanner type.
 */
inline CONSTRUCTOR(CIScanner,
                   CIScanner,
                   Source source,
                   Usize *count_error,
                   const CIProjectConfig *config)
{
    return (CIScanner){ .tokens = NEW(CITokens),
                        .base = NEW(Scanner, source, count_error),
                        .config = config,
                        .is_predefined = false };
}

/**
 *
 * @brief Set the `is_predefined` field to true.
 */
inline void
set_predefined__CIScanner(CIScanner *self)
{
    self->is_predefined = true;
}

/**
 *
 * @brief Check if identifier is standard defined macro.
 */
bool
is_standard_predefined_macro__CIScanner(const String *name);

/**
 *
 * @brief Run the scanner.
 */
void
run__CIScanner(CIScanner *self, bool dump_scanner);

/**
 *
 * @brief Get `tokens_feature` reference.
 * @return const CIFeature* (&)
 */
const CIFeature *
get_tokens_feature__CIScanner();

#define CI_CHECK_STANDARD_SINCE(standard, since, block) \
    if (standard < since) {                             \
        block;                                          \
    }

#define CI_CHECK_STANDARD_UNTIL(standard, until, block) \
    if (standard >= until) {                            \
        block;                                          \
    }

/**
 *
 * @brief Free CIScanner type.
 */
inline DESTRUCTOR(CIScanner, const CIScanner *self)
{
    FREE(CITokens, &self->tokens);
}

enum CIScannerContextLocation
{
    // #define
    CI_SCANNER_CONTEXT_LOCATION_MACRO,
    // #if, #elif condition
    CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_COND,
    // #if, #ifdef, #ifndef, #elif, #elifdef or #elifndef
    CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF,
    // #else
    CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_ELSE,
    // All other location contexts
    CI_SCANNER_CONTEXT_LOCATION_NONE
};

typedef struct CIScannerContext
{
    enum CIScannerContextLocation ctx_location;
    CITokens *tokens; // CITokens*? (&)
    union
    {
        // Macro parameters
        Vec *macro; // Vec<CITokenPreprocessorDefineParam*>*? (&)
    };
} CIScannerContext;

/**
 *
 * @brief Construct CIScannerContext type
 * (CI_SANNER_CONTEXT_LOCATION_MACRO).
 */
inline VARIANT_CONSTRUCTOR(CIScannerContext,
                           CIScannerContext,
                           macro,
                           CITokens *tokens,
                           Vec *macro)
{
    return (CIScannerContext){ .ctx_location =
                                 CI_SCANNER_CONTEXT_LOCATION_MACRO,
                               .tokens = tokens,
                               .macro = macro };
}

/**
 *
 * @brief Construct CIScannerContext type
 * (CI_SANNER_CONTEXT_LOCATION_PREPROCESSOR_COND).
 */
inline VARIANT_CONSTRUCTOR(CIScannerContext,
                           CIScannerContext,
                           preprocessor_cond,
                           CITokens *tokens)
{
    return (CIScannerContext){ .ctx_location =
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_COND,
                               .tokens = tokens };
}

/**
 *
 * @brief Construct CIScannerContext type
 * (CI_SANNER_CONTEXT_LOCATION_PREPROCESSOR_IF).
 */
inline VARIANT_CONSTRUCTOR(CIScannerContext,
                           CIScannerContext,
                           preprocessor_if,
                           CITokens *tokens)
{
    return (CIScannerContext){ .ctx_location =
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF,
                               .tokens = tokens };
}

/**
 *
 * @brief Construct CIScannerContext type
 * (CI_SANNER_CONTEXT_LOCATION_PREPROCESSOR_ELSE).
 */
inline VARIANT_CONSTRUCTOR(CIScannerContext,
                           CIScannerContext,
                           preprocessor_else,
                           CITokens *tokens)
{
    return (CIScannerContext){ .ctx_location =
                                 CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_ELSE,
                               .tokens = tokens };
}

/**
 *
 * @brief Construct CIScannerContext type.
 */
inline CONSTRUCTOR(CIScannerContext,
                   CIScannerContext,
                   enum CIScannerContextLocation ctx_location,
                   CITokens *tokens)
{
    return (CIScannerContext){ .ctx_location = ctx_location, .tokens = tokens };
}

/**
 *
 * @brief Determine whether we are in a macro.
 */
inline bool
is_in_macro__CIScannerContext(const CIScannerContext *self)
{
    return self->ctx_location == CI_SCANNER_CONTEXT_LOCATION_MACRO;
}

/**
 *
 * @brief Determine whether we are in #if, #elif condition.
 */
inline bool
is_in_prepro_cond__CIScannerContext(const CIScannerContext *self)
{
    return self->ctx_location == CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_COND;
}

/**
 *
 * @brief Determine whether we are in #if, #ifdef, #ifndef, #elif, #elifdef or
 * #elifndef preprocessor.
 */
inline bool
is_in_prepro_if__CIScannerContext(const CIScannerContext *self)
{
    return self->ctx_location == CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_IF;
}

/**
 *
 * @brief Determine whether we are in #else preprocessor.
 */
inline bool
is_in_prepro_else__CIScannerContext(const CIScannerContext *self)
{
    return self->ctx_location == CI_SCANNER_CONTEXT_LOCATION_PREPROCESSOR_ELSE;
}

#endif // LILY_CORE_CC_CI_SCANNER_H
