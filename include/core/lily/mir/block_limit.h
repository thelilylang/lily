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

#ifndef LILY_CORE_LILY_MIR_BLOCK_LIMIT_H
#define LILY_CORE_LILY_MIR_BLOCK_LIMIT_H

#include <base/macros.h>
#include <base/types.h>

typedef struct LilyMirBlockLimit
{
    Usize ref_count;
    // This is the id of the last block of the parent.
    // This limit will be use in the MIR and the LLVM IR.
    Usize id;
    // Check if the id is set.
    bool is_set;
} LilyMirBlockLimit;

/**
 *
 * @brief Construct LilyMirBlockLimit type.
 */
CONSTRUCTOR(LilyMirBlockLimit *, LilyMirBlockLimit);

/**
 *
 * @brief Pass to ref a block limit and increment the `ref_count`.
 * @return LilyMirBlockLimit* (&)
 */
inline LilyMirBlockLimit *
ref__LilyMirBlockLimit(LilyMirBlockLimit *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Convert LilyMirBlockLimit in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyMirBlockLimit, const LilyMirBlockLimit *self);
#endif

/**
 *
 * @brief Free LilyMirBlockLimit type.
 */
DESTRUCTOR(LilyMirBlockLimit, LilyMirBlockLimit *self);

#endif // LILY_CORE_LILY_BLOCK_LIMIT_H
