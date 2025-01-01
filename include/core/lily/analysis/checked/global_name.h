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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_GLOBAL_NAME_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_GLOBAL_NAME_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/signature.h>

/**
 *
 * @brief Generate a global name according the params and the return data type
 * of the function.
 * @param fun Vec<LilyCheckedDataType*>*
 */
void
generate_global_fun_name_with_vec__LilyCheckedGlobalName(String *global_name,
                                                         const Vec *fun);

/**
 *
 * @brief Generate a global name according the generic params.
 * @param generic_params OrderedHashMap<LilyCheckedDataType*>*
 */
void
generate_global_type_name_with_ordered_hash_map__LilyCheckedGlobalName(
  String *global_name,
  const OrderedHashMap *generic_params);

/**
 *
 * @brief Generate a global name according the resolved data type.
 */
inline void
generate_global_variant_name__LilyCheckedGlobalName(
  String *global_name,
  LilyCheckedDataType *resolve_dt)
{
    serialize__LilyCheckedDataType(resolve_dt, global_name);
}

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_GLOBAL_NAME_H
