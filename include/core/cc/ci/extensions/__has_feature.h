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

#ifndef LILY_CORE_CC_CI_EXTENSIONS___HAS_FEATURE_H
#define LILY_CORE_CC_CI_EXTENSIONS___HAS_FEATURE_H

#include <base/string.h>

// https://clang.llvm.org/docs/LanguageExtensions.html#checks-for-standard-language-features
enum CIExtensionsHasFeature
{
    // Standard features
    CI_EXTENSIONS_HAS_FEATURE_C_ALIGNAS,
    CI_EXTENSIONS_HAS_FEATURE_C_ALIGNOF,
    CI_EXTENSIONS_HAS_FEATURE_C_ATOMIC,
    CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTIONS,
    CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTION_WITH_CONTROLLING_TYPE,
    CI_EXTENSIONS_HAS_FEATURE_C_STATIC_ASSERT,
    CI_EXTENSIONS_HAS_FEATURE_C_THREAD_LOCAL,
    CI_EXTENSIONS_HAS_FEATURE_ADDRESS_SANITIZER,
    CI_EXTENSIONS_HAS_FEATURE_MODULES
};

/**
 *
 * @brief Check if is valid feature.
 * @param feature String* (&)
 */
bool
is_valid__CIExtensionsHasFeature(const String *feature);

/**
 *
 * @brief Convert feature in String to CIExtensionsHasFeature.
 */
enum CIExtensionsHasFeature
get__CIExtensionsHasFeature(const String *feature);

/**
 *
 * @brief Convert CIExtensionsHasFeature in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIExtensionsHasFeature,
               enum CIExtensionsHasFeature self);
#endif

#endif // LILY_CORE_CC_CI_EXTENSIONS___HAS_FEATURE_H
