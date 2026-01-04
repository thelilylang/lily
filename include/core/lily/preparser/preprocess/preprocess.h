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

#ifndef LILY_CORE_LILY_PREPARSER_PREPROCESS_H
#define LILY_CORE_LILY_PREPARSER_PREPROCESS_H

#include <core/lily/preprocess/allow.h>
#include <core/lily/preprocess/arch.h>
#include <core/lily/preprocess/deny.h>
#include <core/lily/preprocess/doc.h>
#include <core/lily/preprocess/forbid.h>
#include <core/lily/preprocess/os.h>
#include <core/lily/preprocess/repr.h>
#include <core/lily/preprocess/warn.h>

enum LilyPreprocessKind
{
    // #[allow: <value>]
    LILY_PREPROCESS_KIND_ALLOW,
    // #[alwaysinline]
    LILY_PREPROCESS_KIND_ALWAYSINLINE,
    // #[arch: <string>]
    LILY_PREPROCESS_KIND_ARCH,
    // #[builtin]
    LILY_PREPROCESS_KIND_BUILTIN,
    // #[cold]
    LILY_PREPROCESS_KIND_COLD,
    // #[deny: <value>]
    LILY_PREPROCESS_KIND_DENY,
    // #[deprecated: <value>]
    LILY_PREPROCESS_KIND_DEPRECATED,
    // #[doc: <value>]
    LILY_PREPROCESS_KIND_DOC,
    // #[enable_sys]
    LILY_PREPROCESS_KIND_ENABLE_SYS,
    // #[forbid: <value>]
    LILY_PREPROCESS_KIND_FORBID,
    // #[hot]
    LILY_PREPROCESS_KIND_HOT,
    // #[immarg]
    LILY_PREPROCESS_KIND_IMMARG,
    // #[inline]
    LILY_PREPROCESS_KIND_INLINE,
    // #[instruction_set: <value>]
    LILY_PREPROCESS_KIND_INSTRUCTION_SET,
    // #[main]
    LILY_PREPROCESS_KIND_MAIN,
    // #[must_use]
    LILY_PREPROCESS_KIND_MUST_USE,
    // #[no_inline]
    LILY_PREPROCESS_KIND_NO_INLINE,
    // #[no_overload]
    LILY_PREPROCESS_KIND_NO_OVERLOAD,
    // #[no_std]
    LILY_PREPROCESS_KIND_NO_STD,
    // #[os: <value>]
    LILY_PREPROCESS_KIND_OS,
    // #[readonly]
    LILY_PREPROCESS_KIND_READONLY,
    // #[readnone]
    LILY_PREPROCESS_KIND_READNONE,
    // #[repr: <value>]
    LILY_PREPROCESS_KIND_REPR,
    // #[skip]
    LILY_PREPROCESS_KIND_SKIP,
    // #[target_feature: <value>]
    LILY_PREPROCESS_KIND_TARGET_FEATURE,
    // #[test]
    LILY_PREPROCESS_KIND_TEST,
    // #[warn: <value>]
    LILY_PREPROCESS_KIND_WARN,
};

typedef struct LilyPreprocess
{
    enum LilyPreprocessKind kind;
    union
    {
        LilyPreprocessAllow allow;
        LilyPreprocessArch arch;
    };
} LilyPreprocess;

#endif // LILY_CORE_LILY_PREPARSER_PREPROCESS_H
