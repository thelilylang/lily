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

#ifndef LILY_CORE_LILY_ANALYSIS_H
#define LILY_CORE_LILY_ANALYSIS_H

#include <base/vec.h>

#include <core/lily/analysis/checked.h>
#include <core/lily/parser/parser.h>

typedef struct LilyPackage LilyPackage;
typedef struct LilyAstDecl LilyAstDecl;

typedef struct LilyAnalysis
{
    LilyCheckedDeclModule module;
    LilyPackage *package;
    LilyPackage *root_package;
    LilyAstDecl *current;
    const LilyParser *parser;
    Usize position;
    bool use_switch;
} LilyAnalysis;

/**
 *
 * @brief Construct LilyAnalysis type.
 */
inline CONSTRUCTOR(LilyAnalysis,
                   LilyAnalysis,
                   LilyPackage *package,
                   LilyPackage *root_package,
                   const LilyParser *parser,
                   bool use_switch)
{
    return (LilyAnalysis){ .module = NEW(LilyCheckedDeclModule,
                                         from__String("global"),
                                         NULL,
                                         NEW(Vec),
                                         NULL,
                                         LILY_VISIBILITY_PUBLIC),
                           .package = package,
                           .root_package = root_package,
                           .current = NULL,
                           .parser = parser,
                           .position = 0,
                           .use_switch = use_switch };
}

/**
 *
 * @brief Init the module of the analysis.
 */
void
init_module__LilyAnalysis(LilyAnalysis *self);

/**
 *
 * @brief Run the analysis.
 */
void
run__LilyAnalysis(LilyAnalysis *self);

/**
 *
 * @brief Free LilyAnalysis type.
 */
DESTRUCTOR(LilyAnalysis, const LilyAnalysis *self);

#endif // LILY_CORE_LILY_ANALYSIS_H
