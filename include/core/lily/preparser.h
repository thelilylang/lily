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
    Location location;
} LilyPreparserImport;

/**
 *
 * @brief Convert LilyPreparserImport in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyPreparserImport, const LilyPreparserImport *self);
#endif

/**
 *
 * @brief Print debug LilyPreparserImport struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyPreparserImport, const LilyPreparserImport *self);
#endif

/**
 *
 * @brief Free LilyPreparserImport type.
 */
DESTRUCTOR(LilyPreparserImport, LilyPreparserImport *self);

typedef struct LilyPreparserMacro
{
    String *name;
    Vec *tokens; // Vec<LilyToken*>*
    Location location;
} LilyPreparserMacro;

/**
 *
 * @brief Convert LilyPreparserMacro in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMacro, const LilyPreparserMacro *self);
#endif

/**
 *
 * @brief Print debug LilyPreparserMacro struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyPreparserMacro, const LilyPreparserMacro *self);
#endif

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
 * @brief Convert LilyPreparserSubPackage in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserSubPackage,
               const LilyPreparserSubPackage *self);
#endif

/**
 *
 * @brief Print debug LilyPreparserSubPackage struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug,
               LilyPreparserSubPackage,
               const LilyPreparserSubPackage *self);
#endif

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
 * @brief Convert LilyPreparserPackage in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserPackage,
               const LilyPreparserPackage *self);
#endif

/**
 *
 * @brief Print debug LilyPreparserPackage struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyPreparserPackage, const LilyPreparserPackage *self);
#endif

/**
 *
 * @brief Free LilyPreparserPackage type.
 */
DESTRUCTOR(LilyPreparserPackage, LilyPreparserPackage *self);

typedef struct LilyPreparserModule
{
    String *name;
    Vec *body; // Vec<LilyPreparserDecl*>*
    enum LilyVisibility visibility;
} LilyPreparserModule;

typedef struct LilyPreparserTest
{
    String *name;
    Vec *body; // Vec<LilyToken*>*
} LilyPreparserTest;

typedef struct LilyPreparserFun
{
    String *name;
    Vec *generic_params;   // Vec<Vec<LilyToken*>*>*
    Vec *params;           // Vec<Vec<LilyToken*>*>*
    Vec *return_data_type; // Vec<LilyToken*>*
    Vec *body;             // Vec<LilyToken*>*
    Vec *req;              // Vec<LilyToken*>*
    Vec *when;             // Vec<LilyToken*>*
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool req_is_comptime;
    bool when_is_comptime;
} LilyPreparserFun;

typedef struct LilyPreparserClass
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*
    Vec *implements;     // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
} LilyPreparserClass;

typedef struct LilyPreparserTrait
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<LilyToken*>*
    enum LilyVisibility visibility;
} LilyPreparserTrait;

typedef struct LilyPreparserRecordObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *implements;     // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
} LilyPreparserRecordObject;

typedef struct LilyPreparserEnumObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *implements;     // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
} LilyPreparserEnumObject;

enum LilyPreparserObjectKind
{
    LILY_PREPARSER_OBJECT_KIND_CLASS,
    LILY_PREPARSER_OBJECT_KIND_TRAIT,
    LILY_PREPARSER_OBJECT_KIND_RECORD,
    LILY_PREPARSER_OBJECT_KIND_ENUM
};

typedef struct LilyPreparserObject
{
    enum LilyPreparserObjectKind kind;
    union
    {
        LilyPreparserClass class;
        LilyPreparserTrait trait;
        LilyPreparserRecordObject record;
        LilyPreparserEnumObject enum_;
    };
} LilyPreparserObject;

typedef struct LilyPreparserConstantInfo
{
    String *name;
    Vec *expr;      // Vec<LilyToken*>*
    Vec *data_type; // Vec<LilyToken*>*
    enum LilyVisibility visibility;
} LilyPreparserConstantInfo;

enum LilyPreparserConstantKind
{
    LILY_PREPARSER_CONSTANT_KIND_SIMPLE,
    LILY_PREPARSER_CONSTANT_KIND_MULTIPLE
};

typedef struct LilyPreparserConstant
{
    enum LilyPreparserConstantKind kind;
    union
    {
        LilyPreparserConstantInfo *simple;
        Vec *multiple; // Vec<LilyPreparserConstantInfo*>*
    };
} LilyPreparserConstant;

typedef struct LilyPreparser
{
    LilyScanner *scanner;
    Vec *public_imports;  // Vec<LilyPreparserImport*>*
    Vec *private_imports; // Vec<LilyPreparserImport*>*
    Vec *public_macros;   // Vec<LilyPreparserMacro*>*
    Vec *private_macros;  // Vec<LilyPreparserMacro*>*
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
                   String *package_name);

/**
 *
 * @brief Run the preparser.
 */
void
run__LilyPreparser(LilyPreparser *self);

#endif // LILY_CORE_LILY_PREPARSER_H
