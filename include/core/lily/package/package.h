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

#ifndef LILY_CORE_LILY_PACKAGE_PACKAGE_H
#define LILY_CORE_LILY_PACKAGE_PACKAGE_H

#include <base/string.h>
#include <base/vec.h>

#include <core/lily/package/library.h>
#include <core/lily/visibility.h>

typedef struct LilyPackage
{
    String *name;
    Vec *public_macros;  // Vec<LilyMacro*>*?
    Vec *private_macros; // Vec<LilyMacro*>*
    // Import import
    Vec *sub_packages;         // Vec<LilyPackage*>*
    Vec *pacakge_dependencies; // Vec<LilyPackage*>*
    Vec *lib_dependencies;     // Vec<LilyLibrary*>*
    // LilyLibrary library;
    enum LilyVisibility visibility;
} LilyPackage;

/**
 *
 * @brief Construct LilyPackage type.
 */
CONSTRUCTOR(LilyPackage *,
            LilyPackage,
            String *name,
            enum LilyVisibility visibility,
            Vec *public_macros);

/**
 *
 * @brief Build all packages.
 */
LilyPackage *
build__LilyPackage(const CompileConfig *config,
                   String *name,
                   enum LilyVisibility visibility,
                   Vec *public_macros);

/**
 *
 * @brief Compile package.
 */
LilyPackage *
compile__LilyPackage(const CompileConfig *config,
                     String *name,
                     enum LilyVisibility visibility);

/**
 *
 * @brief Free LilyPackage type.
 */
DESTRUCTOR(LilyPackage, LilyPackage *self);

#endif // LILY_CORE_LILY_PACKAGE_PACKAGE_H