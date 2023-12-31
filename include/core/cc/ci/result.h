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

#ifndef LILY_CORE_CC_CI_RESULT_H
#define LILY_CORE_CC_CI_RESULT_H

#include <base/hash_map.h>
#include <base/macros.h>

#include <core/cc/ci/ast.h>
#include <core/cc/ci/token.h>

typedef struct CIResultFile CIResultFile;

typedef struct CIResultDefine
{
    const CITokenPreprocessorDefine
      *define;                      // const CITokenPreprocessorDefine* (&)
    const Location *undef_location; // const Location*? (&)
} CIResultDefine;

/**
 *
 * @brief Construct CIResultDefine type.
 */
CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define,
            const Location *undef_location);

/**
 *
 * @brief Free CIResultDefine type.
 */
inline DESTRUCTOR(CIResultDefine, CIResultDefine *self)
{
    lily_free(self);
}

enum CIResultIncludeKind
{
    CI_RESULT_INCLUDE_KIND_QUOTE,
    CI_RESULT_INCLUDE_KIND_ANGLE,
};

typedef struct CIResultInclude
{
    enum CIResultIncludeKind kind;
    const CIResultFile *path_ref; // const CIResultFile* (&)
    Usize repeat_count; // NOTE: This is used to count the number of times the
                        // same include is used in the same file.
} CIResultInclude;

/**
 *
 * @brief Construct CIResultInclude type.
 */
CONSTRUCTOR(CIResultInclude *,
            CIResultInclude,
            enum CIResultIncludeKind kind,
            const CIResultFile *path_ref);

/**
 *
 * @brief Increment the repeat count of the include.
 */
inline void
increment_repeat_count__CIResultInclude(CIResultInclude *self)
{
    ++self->repeat_count;
}

/**
 *
 * @brief Free CIResultInclude type.
 */
inline DESTRUCTOR(CIResultInclude, CIResultInclude *self)
{
    lily_free(self);
}

// This structure represents the organization of an *.hci or *.ci file.
typedef struct CIResultFile
{
    String *filename;
    HashMap *defines;  // HashMap<Vec<CIResultDefine*>*>*
    HashMap *includes; // HashMap<CIResultInclude*>*
    HashMap *decls;    // HashMap<CIDecl*>*
} CIResultFile;

/**
 *
 * @brief Construct CIResultFile type.
 */
CONSTRUCTOR(CIResultFile *, CIResultFile, String *filename);

/**
 *
 * @brief Free CIResultFile type.
 */
DESTRUCTOR(CIResultFile, CIResultFile *self);

typedef struct CIResult
{
    HashMap *headers; // HashMap<CIResultFile*>*
    HashMap *sources; // HashMap<CIResultFile*>*
} CIResult;

/**
 *
 * @brief Construct CIResult type.
 */
inline CONSTRUCTOR(CIResult, CIResult)
{
    return (CIResult){
        .headers = NEW(HashMap),
        .sources = NEW(HashMap),
    };
}

/**
 *
 * @brief Free CIResult type.
 */
DESTRUCTOR(CIResult, const CIResult *self);

#endif // LILY_CORE_CC_CI_RESULT_H