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

#ifndef LILY_CORE_CC_CI_VISITOR_H
#define LILY_CORE_CC_CI_VISITOR_H

#include <core/cc/ci/result.h>

typedef struct CIVisitor
{
    const CIResult *result;       // const CIResult* (&)
    const CIResultFile *file;     // const CIResultFile*? (&)
    const CIScope *current_scope; // const CIScope*? (&)
} CIVisitor;

/**
 *
 * @brief Construct CIVisitor type.
 */
inline CONSTRUCTOR(CIVisitor, CIVisitor, const CIResult *result)
{
    return (CIVisitor){ .result = result, .file = NULL, .current_scope = NULL };
}

/**
 *
 * @brief Run the visitor.
 */
void
run__CIVisitor(CIVisitor *self);

#endif // LILY_CORE_CC_CI_VISITOR_H
