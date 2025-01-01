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

#ifndef LILY_CORE_LILY_PREPARSER_PREPROCESS_LINK_INFO_H
#define LILY_CORE_LILY_PREPARSER_PREPROCESS_LINK_INFO_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

// #[link: <name>]
// #[link: (<name>, <path>)]
typedef struct LilyLink
{
    String *name;
    String *path; // String*?
} LilyLink;

/**
 *
 * @brief Construct LilyLink type.
 */
CONSTRUCTOR(LilyLink *, LilyLink, String *name, String *path);

/**
 *
 * @brief Free LilyLink type.
 */
DESTRUCTOR(LilyLink, LilyLink *self);

typedef struct LilyPreprocessLinkInfo
{
    Vec *links; // Vec<LilyLink*>*
} LilyPreprocessLinkInfo;

/**
 *
 * @brief Construct LilyPreprocessLinkInfo type.
 */
inline CONSTRUCTOR(LilyPreprocessLinkInfo, LilyPreprocessLinkInfo)
{
    return (LilyPreprocessLinkInfo){ .links = NEW(Vec) };
}

/**
 *
 * @brief Free LilyPreprocessLinkInfo type.
 */
DESTRUCTOR(LilyPreprocessLinkInfo, const LilyPreprocessLinkInfo *self);

#endif // LILY_CORE_LILY_PREPARSER_PREPROCESS_LINK_INFO_H
