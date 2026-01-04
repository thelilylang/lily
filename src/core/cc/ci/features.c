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

#include <core/cc/ci/features.h>

// NOTE: This table must be sorted in ascending order.
static const SizedStr standards[CI_N_STANDARD] = {
    [CI_STANDARD_NONE] = SIZED_STR_FROM_RAW(""),
    [CI_STANDARD_11] = SIZED_STR_FROM_RAW("c11"),
    [CI_STANDARD_17] = SIZED_STR_FROM_RAW("c17"),
    [CI_STANDARD_23] = SIZED_STR_FROM_RAW("c23"),
    [CI_STANDARD_89] = SIZED_STR_FROM_RAW("c89"),
    [CI_STANDARD_95] = SIZED_STR_FROM_RAW("c95"),
    [CI_STANDARD_99] = SIZED_STR_FROM_RAW("c99"),
    [CI_STANDARD_KR] = SIZED_STR_FROM_RAW("kr"),
};

static const Int32 standard_ids[CI_N_STANDARD] = {
    CI_STANDARD_NONE, CI_STANDARD_KR, CI_STANDARD_89, CI_STANDARD_95,
    CI_STANDARD_99,   CI_STANDARD_11, CI_STANDARD_17, CI_STANDARD_23
};

const SizedStr *
get_standards__CIStandard()
{
    return standards;
}

const Int32 *
get_standard_ids__CIStandard()
{
    return standard_ids;
}
