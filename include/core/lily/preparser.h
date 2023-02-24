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
 * @brief Construct LilyPreparserImport type.
 */
CONSTRUCTOR(LilyPreparserImport *,
            LilyPreparserImport,
            String *value,
            String *as,
            Location location);

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
 * @brief Construct LilyPreparserMacro type.
 */
CONSTRUCTOR(LilyPreparserMacro *,
            LilyPreparserMacro,
            String *name,
            Vec *tokens,
            Location location);

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
 * @brief Construct LilyPreparserSubPackage type.
 */
CONSTRUCTOR(LilyPreparserSubPackage *,
            LilyPreparserSubPackage,
            enum LilyVisibility visibility,
            String *name);

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
 * @brief Construct LilyPreparserPackage type.
 */
CONSTRUCTOR(LilyPreparserPackage *, LilyPreparserPackage, String *name);

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

/**
 *
 * @brief Construct LilyPreparserModule type.
 */
inline CONSTRUCTOR(LilyPreparserModule,
                   LilyPreparserModule,
                   String *name,
                   Vec *body,
                   enum LilyVisibility visibility)
{
    return (LilyPreparserModule){ .name = name,
                                  .body = body,
                                  .visibility = visibility };
}

/**
 *
 * @brief Free LilyPreparserModule type.
 */
DESTRUCTOR(LilyPreparserModule, const LilyPreparserModule *self);

typedef struct LilyPreparserTest
{
    String *name;
    Vec *body; // Vec<LilyToken*>*
} LilyPreparserTest;

/**
 *
 * @brief Construct LilyPreparserTest type.
 */
inline CONSTRUCTOR(LilyPreparserTest,
                   LilyPreparserTest,
                   String *name,
                   Vec *body)
{
    return (LilyPreparserTest){
        .name = name,
        .body = body,
    };
}

/**
 *
 * @brief Free LilyPreparserTest type.
 */
DESTRUCTOR(LilyPreparserTest, const LilyPreparserTest *self);

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

/**
 *
 * @brief Construct LilyPreparserFun type.
 */
inline CONSTRUCTOR(LilyPreparserFun,
                   LilyPreparserFun,
                   String *name,
                   Vec *generic_params,
                   Vec *params,
                   Vec *return_data_type,
                   Vec *body,
                   Vec *req,
                   Vec *when,
                   enum LilyVisibility visibility,
                   bool is_async,
                   bool is_operator,
                   bool req_is_comptime,
                   bool when_is_comptime)
{
    return (LilyPreparserFun){ .name = name,
                               .generic_params = generic_params,
                               .params = params,
                               .return_data_type = return_data_type,
                               .body = body,
                               .req = req,
                               .when = when,
                               .visibility = visibility,
                               .is_async = is_async,
                               .is_operator = is_operator,
                               .req_is_comptime = req_is_comptime,
                               .when_is_comptime = when_is_comptime };
}

/**
 *
 * @brief Free LilyPreparserFun type.
 */
DESTRUCTOR(LilyPreparserFun, const LilyPreparserFun *self);

typedef struct LilyPreparserClass
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*
    Vec *implements;     // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
} LilyPreparserClass;

/**
 *
 * @brief Construct LilyPreparserClass type.
 */
inline CONSTRUCTOR(LilyPreparserClass,
                   LilyPreparserClass,
                   String *name,
                   Vec *generic_params,
                   Vec *inherits,
                   Vec *implements,
                   Vec *body,
                   enum LilyVisibility visibility)
{
    return (LilyPreparserClass){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .implements = implements,
                                 .body = body };
}

/**
 *
 * @brief Free LilyPreparserClass type.
 */
DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self);

typedef struct LilyPreparserTrait
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<LilyToken*>*
    enum LilyVisibility visibility;
} LilyPreparserTrait;

/**
 *
 * @brief Construct LilyPreparserTrait type.
 */
inline CONSTRUCTOR(LilyPreparserTrait,
                   LilyPreparserTrait,
                   String *name,
                   Vec *generic_params,
                   Vec *inherits,
                   Vec *body,
                   enum LilyVisibility visibility)
{
    return (LilyPreparserTrait){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .body = body,
                                 .visibility = visibility };
}

/**
 *
 * @brief Free LilyPreparserTrait type.
 */
DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self);

typedef struct LilyPreparserRecordObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*
    Vec *implements;     // Vec<Vec<LilyToken*>*>*
    Vec *body;           // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
} LilyPreparserRecordObject;

/**
 *
 * @brief Construct LilyPreparserRecordObject type.
 */
inline CONSTRUCTOR(LilyPreparserRecordObject,
                   LilyPreparserRecordObject,
                   String *name,
                   Vec *generic_params,
                   Vec *implements,
                   Vec *body,
                   enum LilyVisibility visibility)
{
    return (LilyPreparserRecordObject){ .name = name,
                                        .generic_params = generic_params,
                                        .implements = implements,
                                        .body = body,
                                        .visibility = visibility };
}

/**
 *
 * @brief Free LilyPreparserRecordObject type.
 */
DESTRUCTOR(LilyPreparserRecordObject, const LilyPreparserRecordObject *self);

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
                   String *package_name)
{
    return (LilyPreparser){ .scanner = scanner,
                            .public_imports = NEW(Vec),
                            .private_imports = NEW(Vec),
                            .public_macros = NEW(Vec),
                            .private_macros = NEW(Vec),
                            .package = NEW(LilyPreparserPackage, package_name),
                            .current = NULL,
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