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

#ifndef LILY_CORE_CC_CI_FEATURES_H
#define LILY_CORE_CC_CI_FEATURES_H

#include <base/sized_str.h>
#include <base/types.h>

#define CI_N_STANDARD 8

enum CIStandard
{
    CI_STANDARD_NONE = 0,
    CI_STANDARD_KR = 1,
    CI_STANDARD_89 = 2,
    CI_STANDARD_95 = 3,
    CI_STANDARD_99 = 4,
    CI_STANDARD_11 = 5,
    CI_STANDARD_17 = 6,
    CI_STANDARD_23 = 7,
};

/**
 *
 * @brief Get list of standard in str.
 * @return const SizedStr* (&)
 */
const SizedStr *
get_standards__CIStandard();

/**
 *
 * @brief Get list of standard ids.
 * @return Int32 * (&)
 */
const Int32 *
get_standard_ids__CIStandard();

// TODO: When we move ci scanner dependencies in src/core/cc or include/core/cc
// directories, rename CIEnable to CCCIEnable
enum CIEnable
{
    CI_ENABLE_NO = 0,
    CI_ENABLE_YES = 1,
};

typedef struct CIFeature
{
    enum CIStandard since;
    enum CIStandard until;
} CIFeature;

#endif // LILY_CORE_CC_CI_FEATURES_H
