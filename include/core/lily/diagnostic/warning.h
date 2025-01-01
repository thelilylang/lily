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

#ifndef LILY_CORE_LILY_DIAGNOSTIC_WARNING_H
#define LILY_CORE_LILY_DIAGNOSTIC_WARNING_H

#include <base/macros.h>

enum LilyWarningKind
{
    LILY_WARNING_KIND_UNUSED_PAREN,
    LILY_WARNING_KIND_UNUSED_SEMICOLON,
    LILY_WARNING_KIND_UNUSED_MACRO_PARAM,
    LILY_WARNING_KIND_UNREACHABLE_CODE,
    LILY_WARNING_KIND_UNUSED_MULTIPLE_CAPTURE,
    LILY_WARNING_KIND_UNUSED_CAPTURE
};

typedef struct LilyWarning
{
    enum LilyWarningKind kind;
    // union
    // {};
} LilyWarning;

/**
 *
 * @brief Construct LilyWarning.
 */
inline CONSTRUCTOR(LilyWarning, LilyWarning, enum LilyWarningKind kind)
{
    return (LilyWarning){ .kind = kind };
}

/**
 *
 * @brief Convert LilyWarning in msg.
 */
char *
to_msg__LilyWarning(const LilyWarning *self);

/**
 *
 * @brief Convert LilyWarning in code.
 */
char *
to_code__LilyWarning(const LilyWarning *self);

/**
 *
 * @brief Convert LilyWarning in str.
 */
char *
to_string__LilyWarning(const LilyWarning *self);

#endif // LILY_CORE_LILY_DIAGNOSTIC_WARNING_H
