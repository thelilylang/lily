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

#ifndef LILY_CORE_LILY_CHECKED_SIGNATURE_H
#define LILY_CORE_LILY_CHECKED_SIGNATURE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

typedef struct LilyCheckedSignatureFun
{
    String *global_name;
    // [params, return_data_type]
    Vec *types; // Vec<LilyCheckedDataType* (&)>*
} LilyCheckedSignatureFun;

/**
 *
 * @brief Construct LilyCheckedSignatureFun type.
 * @param global_name String* (&)
 */
CONSTRUCTOR(LilyCheckedSignatureFun *,
            LilyCheckedSignatureFun,
            String *global_name,
            Vec *types);

/**
 *
 * @brief Reload global name.
 */
void
reload_global_name__LilyCheckedSignatureFun(LilyCheckedSignatureFun *self);

/**
 *
 * @brief Check if the signature contains a compiler defined data type.
 */
bool
contains_compiler_defined_dt__LilyCheckedSignatureFun(
  const LilyCheckedSignatureFun *self);

/**
 *
 * @brief Convert LilyCheckedSignatureFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedSignatureFun,
               const LilyCheckedSignatureFun *self);
#endif

/**
 *
 * @brief Free LilyCheckedSignatureFun type.
 */
DESTRUCTOR(LilyCheckedSignatureFun, LilyCheckedSignatureFun *self);

#endif // LILY_CORE_LILY_CHECKED_SIGNATURE_H
