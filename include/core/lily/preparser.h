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

#ifndef LILY_CORE_LILY_PREPARSER_H
#define LILY_CORE_LILY_PREPARSER_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/scanner.h>
#include <core/lily/token.h>
#include <core/lily/visibility.h>

typedef struct LilyPreparserImport
{
    String *value;
    String *as; // String*?
} LilyPreparserImport;

/**
 *
 * @brief Construct LilyPreparserImport type.
 */
CONSTRUCTOR(LilyPreparserImport *,
            LilyPreparserImport,
            String *value,
            String *as);

/**
 *
 * @brief Free LilyPreparserImport type.
 */
DESTRUCTOR(LilyPreparserImport, LilyPreparserImport *self);

typedef struct LilyPreparserMacro
{
    String *name;
    Vec *tokens; // Vec<LilyToken*>*
} LilyPreparserMacro;

/**
 *
 * @brief Construct LilyPreparserMacro type.
 */
CONSTRUCTOR(LilyPreparserMacro *,
            LilyPreparserMacro,
            String *name,
            Vec *tokens);

/**
 *
 * @brief Free LilyPreparserMacro type.
 */
DESTRUCTOR(LilyPreparserMacro, LilyPreparserMacro *self);

typedef struct LilyPreparserSubPackage
{
    String *name;
    enum LilyVisibility visibility;
} LilyPreparserSubPackage;

/**
 *
 * @brief Construct LilyPreparserSubPackage type.
 */
CONSTRUCTOR(LilyPreparserSubPackage *,
            LilyPreparserSubPackage,
            enum LilyVisibility visibility,
            String *name);

/**
 *
 * @brief Free LilyPreparserSubPackage type.
 */
DESTRUCTOR(LilyPreparserSubPackage, LilyPreparserSubPackage *self);

typedef struct LilyPreparserPackage
{
    String *name;      // String*?
    Vec *sub_packages; // Vec<LilyPreparserSubPackage*>*
} LilyPreparserPackage;

/**
 *
 * @brief Construct LilyPreparserPackage type.
 */
CONSTRUCTOR(LilyPreparserPackage *, LilyPreparserPackage, String *name);

/**
 *
 * @brief Free LilyPreparserPackage type.
 */
DESTRUCTOR(LilyPreparserPackage, LilyPreparserPackage *self);

typedef struct LilyPreparser
{
    LilyScanner *scanner;
    Vec *imports;       // Vec<LilyPreparserImport*>*
    Vec *public_macros; // Vec<LilyPreparserMacro*>*
    LilyPreparserPackage *package;
    LilyToken *current;
    Usize position;
    Usize count_error;
} LilyPreparser;

/**
 *
 * @brief Construct LilyPreparser type.
 */
inline CONSTRUCTOR(LilyPreparser,
                   LilyPreparser,
                   LilyScanner *scanner,
                   String *package_name)
{
    return (LilyPreparser){ .scanner = scanner,
                            .imports = NEW(Vec),
                            .public_macros = NEW(Vec),
                            .package = NEW(LilyPreparserPackage, package_name),
                            .current = get__Vec(scanner->tokens, 0),
                            .position = 0,
                            .count_error = 0 };
}

/**
 *
 * @brief Run the preparser.
 */
void
run__LilyPreparser(LilyPreparser *self);

#endif // LILY_CORE_LILY_PREPARSER_H