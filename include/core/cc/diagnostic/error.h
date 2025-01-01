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

#ifndef LILY_CORE_CC_DIAGNOSTIC_ERROR_H
#define LILY_CORE_CC_DIAGNOSTIC_ERROR_H

#include <base/macros.h>

enum CcErrorKind
{
    CC_ERROR_KIND_UNEXPECTED_TOKEN
};

typedef struct CcError
{
    enum CcErrorKind kind;
    union
    {
        char *unexpected_token;
    };
} CcError;

/**
 *
 * @brief Construct CcError.
 */
inline CONSTRUCTOR(CcError, CcError, enum CcErrorKind kind)
{
    return (CcError){ .kind = kind };
}

/**
 *
 * @brief Construct CcError (CC_ERROR_KIND_UNEXPECTED_TOKEN).
 */
inline VARIANT_CONSTRUCTOR(CcError,
                           CcError,
                           unexpected_token,
                           char *unexpected_token)
{
    return (CcError){ .kind = CC_ERROR_KIND_UNEXPECTED_TOKEN,
                      .unexpected_token = unexpected_token };
}

/**
 *
 * @brief Convert CcError in msg.
 */
char *
to_msg__CcError(const CcError *self);

/**
 *
 * @brief Convert CcError in code.
 */
char *
to_code__CcError(const CcError *self);

/**
 *
 * @brief Convert CcError in str.
 */
char *
to_string__CcError(const CcError *self);

#endif // LILY_CORE_CC_DIAGNOSTIC_ERROR_H
