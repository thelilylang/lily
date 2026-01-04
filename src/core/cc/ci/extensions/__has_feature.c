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

#include <base/assert.h>
#include <base/sized_str.h>

#include <core/cc/ci/extensions/__has_feature.h>
#include <core/shared/search.h>

#include <stdio.h>
#include <stdlib.h>

static const SizedStr features[] = {
    SIZED_STR_FROM_RAW("c_alignas"),
    SIZED_STR_FROM_RAW("c_alignof"),
    SIZED_STR_FROM_RAW("c_atomic"),
    SIZED_STR_FROM_RAW("c_generic_selections"),
    SIZED_STR_FROM_RAW("c_generic_selection_with_controlling_type"),
    SIZED_STR_FROM_RAW("c_static_assert"),
    SIZED_STR_FROM_RAW("c_thread_local"),
    SIZED_STR_FROM_RAW("address_sanitizer"),
    SIZED_STR_FROM_RAW("modules"),
};
static const Usize features_len = sizeof(features) / sizeof(*features);

static const enum CIExtensionsHasFeature feature_ids[] = {
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
static const Usize feature_ids_len = sizeof(feature_ids) / sizeof(*feature_ids);

static_assert(features_len == feature_ids_len,
              "expected features_len==feature_ids_len");

bool
is_valid__CIExtensionsHasFeature(const String *feature)
{
    return get_id__Search(
             feature, features, (const Int32 *)feature_ids, features_len) != -1;
}

enum CIExtensionsHasFeature
get__CIExtensionsHasFeature(const String *feature)
{
    enum CIExtensionsHasFeature res = get_id__Search(
      feature, features, (const Int32 *)feature_ids, features_len);

    ASSERT(res != -1);

    return res;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               CIExtensionsHasFeature,
               enum CIExtensionsHasFeature self)
{
    switch (self) {
        case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNAS:
        case CI_EXTENSIONS_HAS_FEATURE_C_ALIGNOF:
        case CI_EXTENSIONS_HAS_FEATURE_C_ATOMIC:
        case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTIONS:
        case CI_EXTENSIONS_HAS_FEATURE_C_GENERIC_SELECTION_WITH_CONTROLLING_TYPE:
        case CI_EXTENSIONS_HAS_FEATURE_C_STATIC_ASSERT:
        case CI_EXTENSIONS_HAS_FEATURE_C_THREAD_LOCAL:
        case CI_EXTENSIONS_HAS_FEATURE_ADDRESS_SANITIZER:
        case CI_EXTENSIONS_HAS_FEATURE_MODULES:
            return (char *)features[self].buffer;
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif
