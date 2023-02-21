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

#ifndef LILY_CORE_LILY_PRECOMPILE_H
#define LILY_CORE_LILY_PRECOMPILE_H

#include <core/lily/preparser.h>

typedef struct LilyPackage LilyPackage;

enum LilyImportValueKind
{
    LILY_IMPORT_VALUE_KIND_ACCCESS,
    LILY_IMPORT_VALUE_KIND_BUILTIN,
    LILY_IMPORT_VALUE_KIND_CORE,
    LILY_IMPORT_VALUE_KIND_FILE,
    LILY_IMPORT_VALUE_KIND_LIBRARY,
    LILY_IMPORT_VALUE_KIND_PACKAGE,
    LILY_IMPORT_VALUE_KIND_SELECT_ALL,
    LILY_IMPORT_VALUE_KIND_SELECT,
    LILY_IMPORT_VALUE_KIND_STD,
    LILY_IMPORT_VALUE_KIND_URL,
};

typedef struct LilyImportValue
{
    enum LilyImportValueKind kind;
    union
    {
        String *access;
        String *file;
        String *library;
        String *package;
        Vec *select; // Vec<Vec<LilyImportValue*>*>*
    };
} LilyImportValue;

/**
 *
 * @brief Construct LilyImportValue type.
 */
CONSTRUCTOR(LilyImportValue *, LilyImportValue, enum LilyImportValueKind kind);

/**
 *
 * @brief Construct LilyImportValue type (LILY_IMPORT_VALUE_KIND_ACCESS).
 */
VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, access, String *access);

/**
 *
 * @brief Construct LilyImportValue type (LILY_IMPORT_VALUE_KIND_FILE).
 */
VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, file, String *file);

/**
 *
 * @brief Construct LilyImportValue type (LILY_IMPORT_VALUE_KIND_LIBRARY).
 */
VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    library,
                    String *library);

/**
 *
 * @brief Construct LilyImportValue type (LILY_IMPORT_VALUE_KIND_PACKAGE).
 */
VARIANT_CONSTRUCTOR(LilyImportValue *,
                    LilyImportValue,
                    package,
                    String *package);

/**
 *
 * @brief Construct LilyImportValue type (LILY_IMPORT_VALUE_KIND_SELECT).
 */
VARIANT_CONSTRUCTOR(LilyImportValue *, LilyImportValue, select, Vec *select);

/**
 *
 * @brief Convert LilyImportValueKind in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyImportValueKind, enum LilyImportValueKind self);
#endif

/**
 *
 * @brief Convert LilyImportValue in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyImportValue, const LilyImportValue *self);
#endif

/**
 *
 * @brief Print debug LilyImportValue struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyImportValue, const LilyImportValue *self);
#endif

/**
 *
 * @brief Free LilyImportValue type.
 */
DESTRUCTOR(LilyImportValue, LilyImportValue *self);

typedef struct LilyImport
{
    Vec *values; // Vec<LilyImportValue*>*
    String *as;
} LilyImport;

/**
 *
 * @brief Construct LilyImport type.
 */
CONSTRUCTOR(LilyImport *, LilyImport, Vec *values, String *as);

/**
 *
 * @brief Free LilyImport type.
 */
DESTRUCTOR(LilyImport, LilyImport *self);

typedef struct LilyPrecompile
{
    const LilyPreparser *preparser;
    LilyPackage *package;
    Usize count_error;
} LilyPrecompile;

/**
 *
 * @brief Construct LilyPrecompile type.
 */
CONSTRUCTOR(LilyPrecompile,
            LilyPrecompile,
            const LilyPreparser *preparser,
            LilyPackage *package);

/**
 *
 * @brief Run the pre-compiler.
 */
void
run__LilyPrecompile(LilyPrecompile *self);

#endif // LILY_CORE_LILY_PRECOMPILE_H