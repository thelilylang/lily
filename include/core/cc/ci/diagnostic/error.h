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

#ifndef LILY_CORE_CC_CI_DIAGNOSTIC_ERROR_H
#define LILY_CORE_CC_CI_DIAGNOSTIC_ERROR_H

#include <base/macros.h>

enum CIErrorKind {
	CI_ERROR_KIND_UNCLOSED_COMMENT_BLOCK,
	CI_ERROR_KIND_UNCLOSED_COMMENT_DOC,
	CI_ERROR_KIND_MISMATCHED_CLOSING_DELIMITER,
	CI_ERROR_KIND_UNCLOSED_CHAR_LITERAL,
	CI_ERROR_KIND_INVALID_ESCAPE
};


typedef struct CIError 
{
    enum CIErrorKind kind;
} CIError;

/**
 *
 * @brief Construct CIError.
 */
inline CONSTRUCTOR(CIError, CIError, enum CIErrorKind kind)
{
    return (CIError){ .kind = kind };
}

/**
 *
 * @brief Convert CIError in msg.
 */
char *
to_msg__CIError(const CIError *self);

/**
 *
 * @brief Convert CIError in code.
 */
char *
to_code__CIError(const CIError *self);

/**
 *
 * @brief Convert CIError in str.
 */
char *
to_string__CIError(const CIError *self);

#endif // LILY_CORE_CC_CI_DIAGNOSTIC_ERROR_H
