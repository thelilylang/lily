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

/**
 *
 * @brief Convert LilyPreparserModule in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserModule, const LilyPreparserModule *self);
#endif

typedef struct LilyPreparserTest
{
    String *name;
    // TODO: change data type of body (Vec<LilyToken*>* ->
    // Vec<LilyPreparserFunBodyItem*>*)
    // TODO: update its constructor and its destructor.
    Vec *body; // Vec<LilyToken*>*
} LilyPreparserTest;

typedef struct LilyPreparserFunBodyItemExprs
{
    Vec *tokens; // Vec<LilyToken*>*
} LilyPreparserFunBodyItemExprs;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemExprs in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemExprs,
               const LilyPreparserFunBodyItemExprs *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtBlock
{
    Vec *block; // Vec<LilyPreparserFunBodyItem*>*
} LilyPreparserFunBodyItemStmtBlock;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtBlock in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtBlock,
               const LilyPreparserFunBodyItemStmtBlock *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtFor
{
    Vec *expr;  // Vec<LilyToken*>*
    Vec *block; // Vec<LilyPreparserFunBodyItem*>*
} LilyPreparserFunBodyItemStmtFor;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtFor in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtFor,
               const LilyPreparserFunBodyItemStmtFor *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtIf
{
    Vec *if_expr;     // Vec<LilyToken*>*
    Vec *if_block;    // Vec<LilyToken*>*
    Vec *elif_exprs;  // Vec<Vec<LilyToken*>*?
    Vec *elif_blocks; // Vec<Vec<LilyPreparserFunBodyItem*>*>*?
    Vec *else_block;  // Vec<LilyPreparserFunBodyItem>*?
} LilyPreparserFunBodyItemStmtIf;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtIf in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtIf,
               const LilyPreparserFunBodyItemStmtIf *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtMatch
{
    Vec *expr;          // Vec<LilyToken*>*
    Vec *patterns;      // Vec<Vec<LilyToken*>*>*
    Vec *pattern_conds; // Vec<Vec<LilyToken*>*?>*
    Vec *blocks;        // Vec<LilyPreparserFunBodyItem*>*
} LilyPreparserFunBodyItemStmtMatch;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtMatch in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtMatch,
               const LilyPreparserFunBodyItemStmtMatch *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtTry
{
    Vec *block;       // Vec<LilyPreparserFunBodyItem*>*
    Vec *catch_expr;  // Vec<LilyToken*>*?
    Vec *catch_block; // Vec<LilyPreparserFunBodyItem*>*?
} LilyPreparserFunBodyItemStmtTry;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtTry in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtTry,
               const LilyPreparserFunBodyItemStmtTry *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtVariable
{
    String *name;
    Vec *data_type; // Vec<LilyToken*>*?
    Vec *expr;      // Vec<LilyToken*>*
    bool is_mut;
    bool is_trace;
    bool is_ref;
    bool is_drop;
} LilyPreparserFunBodyItemStmtVariable;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtVariable in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtVariable,
               const LilyPreparserFunBodyItemStmtVariable *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtWhile
{
    Vec *expr;  // Vec<LilyToken*>*
    Vec *block; // Vec<LilyPreparserFunBodyItem*>*
} LilyPreparserFunBodyItemStmtWhile;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtWhile in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtWhile,
               const LilyPreparserFunBodyItemStmtWhile *self);
#endif

enum LilyPreparserFunBodyItemKind
{
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE
};

// LilyPreparserFunBodyItem is used for body of functions and methods.
typedef struct LilyPreparserFunBodyItem
{
    enum LilyPreparserFunBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserFunBodyItemExprs exprs;
        LilyPreparserFunBodyItemStmtBlock stmt_block;
        LilyPreparserFunBodyItemStmtFor stmt_for;
        LilyPreparserFunBodyItemStmtIf stmt_if;
        LilyPreparserFunBodyItemStmtMatch stmt_match;
        LilyPreparserFunBodyItemStmtTry stmt_try;
        LilyPreparserFunBodyItemStmtVariable stmt_var;
        LilyPreparserFunBodyItemStmtWhile stmt_while;
    };
} LilyPreparserFunBodyItem;

/**
 *
 * @brief Convert LilyPreparserFunBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItem,
               const LilyPreparserFunBodyItem *self);
#endif

typedef struct LilyPreparserFun
{
    String *name;
    String *object_impl;   // String*?
    Vec *generic_params;   // Vec<Vec<LilyToken*>*>*?
    Vec *params;           // Vec<Vec<LilyToken*>*>*?
    Vec *return_data_type; // Vec<LilyToken*>*
    Vec *body;             // Vec<LilyPreparserFunBodyItem*>*
    Vec *req;              // Vec<Vec<LilyToken*>*>*
    Vec *when;             // Vec<Vec<LilyToken*>*>*
    enum LilyVisibility visibility;
    bool is_async;
    bool is_operator;
    bool req_is_comptime;
    bool when_is_comptime;
} LilyPreparserFun;

/**
 *
 * @brief Convert LilyPreparserFun in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserFun, const LilyPreparserFun *self);
#endif

typedef struct LilyPreparserConstantInfo
{
    String *name;
    Vec *expr;      // Vec<LilyToken*>*
    Vec *data_type; // Vec<LilyToken*>*
    enum LilyVisibility visibility;
} LilyPreparserConstantInfo;

/**
 *
 * @brief Convert LilyPreparserConstantInfo in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstantInfo,
               const LilyPreparserConstantInfo *self);
#endif

enum LilyPreparserConstantKind
{
    LILY_PREPARSER_CONSTANT_KIND_SIMPLE,
    LILY_PREPARSER_CONSTANT_KIND_MULTIPLE
};

/**
 *
 * @brief Convert LilyPreparserConstantKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstantKind,
               enum LilyPreparserConstantKind kind);
#endif

typedef struct LilyPreparserConstant
{
    enum LilyPreparserConstantKind kind;
    union
    {
        LilyPreparserConstantInfo *simple;
        Vec *multiple; // Vec<LilyPreparserConstantInfo*>*
    };
} LilyPreparserConstant;

/**
 *
 * @brief Convert LilyPreparserConstant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstant,
               const LilyPreparserConstant *self);
#endif

enum LilyPreparserClassBodyItemKind
{
    LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE,
    LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD,
};

/**
 *
 * @brief Convert LilyPreparserClassBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserClassBodyItemKind,
               enum LilyPreparserClassBodyItemKind kind);
#endif

typedef LilyPreparserFun LilyPreparserMethod;

/**
 *
 * @brief Convert LilyPreparserMethod in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
inline String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMethod, const LilyPreparserMethod *self)
{
    return to_string__Debug__LilyPreparserFun(self);
}
#endif

typedef struct LilyPreparserAttribute
{
    String *name;
    Vec *data_type;    // Vec<LilyToken*>*
    Vec *default_expr; // Vec<LilyToken*>*?
    enum LilyVisibility visibility;
    bool is_get;
    bool is_set;
} LilyPreparserAttribute;

/**
 *
 * @brief Convert LilyPreparserAttribute in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserAttribute,
               const LilyPreparserAttribute *self);
#endif

typedef struct LilyPreparserClassBodyItem
{
    enum LilyPreparserClassBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserAttribute attribute;
        LilyPreparserMethod method;
    };
} LilyPreparserClassBodyItem;

typedef struct LilyPreparserClass
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*?
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*?
    Vec *implements;     // Vec<Vec<LilyToken*>*>*?
    Vec *body;           // Vec<LilyPreparserClassBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserClass;

enum LilyPreparserTraitBodyItemKind
{
    LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE,
    LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE,
};

typedef struct LilyPreparserPrototype
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*?
    Vec *params;         // Vec<Vec<LilyToken*>*>*?
} LilyPreparserPrototype;

typedef struct LilyPreparserTraitBodyItem
{
    enum LilyPreparserTraitBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserPrototype prototype;
        LilyPreparserAttribute attribute;
    };
} LilyPreparserTraitBodyItem;

typedef struct LilyPreparserTrait
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*?
    Vec *inherits;       // Vec<Vec<LilyToken*>*>*?
    Vec *body;           // Vec<LilyPreparserTraitBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserTrait;

typedef struct LilyPreparserRecordField
{
    String *name;
    Vec *data_type;     // Vec<LilyToken*>*
    Vec *optional_expr; // Vec<LilyToken*>*?
    enum LilyVisibility visibility;
    Location location;
} LilyPreparserRecordField;

enum LilyPreparserRecordObjectBodyItemKind
{
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT,
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD,
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD,
};

typedef struct LilyPreparserRecordObjectBodyItem
{
    enum LilyPreparserRecordObjectBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserRecordField field;
        LilyPreparserMethod method;
    };
} LilyPreparserRecordObjectBodyItem;

typedef struct LilyPreparserRecordObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*?
    Vec *implements;     // Vec<Vec<LilyToken*>*>*?
    Vec *body;           // Vec<LilyPreparserRecordObjectBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserRecordObject;

typedef struct LilyPreparserEnumVariant
{
    String *name;
    Vec *data_type; // Vec<LilyToken*>*
    Location location;
} LilyPreparserEnumVariant;

enum LilyPreparserEnumObjectBodyItemKind
{
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT,
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD,
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT,
};

typedef struct LilyPreparserEnumObjectBodyItem
{
    enum LilyPreparserEnumObjectBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserMethod method;
        LilyPreparserEnumVariant variant;
    };
} LilyPreparserEnumObjectBodyItem;

typedef struct LilyPreparserEnumObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken*>*>*?
    Vec *implements;     // Vec<Vec<LilyToken*>*>*?
    Vec *body;           // Vec<LilyPreparserEnumObjectBodyItem*>*
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

typedef struct LilyPreparserAlias
{
    String *name;
    Vec *data_type; // Vec<LilyToken*>*
    enum LilyVisibility visibility;
} LilyPreparserAlias;

typedef struct LilyPreparserEnum
{
    String *name;
    Vec *variants; // Vec<Vec<LilyPreparserEnumVariant>*>*
    enum LilyVisibility visibility;
} LilyPreparserEnum;

typedef struct LilyPreparserRecord
{
    String *name;
    Vec *fields; // Vec<LilyPreparserRecordField*>*
    enum LilyVisibility visibility;
} LilyPreparserRecord;

enum LilyPreparserTypeKind
{
    LILY_PREPARSER_TYPE_KIND_ALIAS,
    LILY_PREPARSER_TYPE_KIND_ENUM,
    LILY_PREPARSER_TYPE_KIND_RECORD,
};

typedef struct LilyPreparserType
{
    enum LilyPreparserTypeKind kind;
    union
    {
        LilyPreparserAlias alias;
        LilyPreparserEnum enum_;
        LilyPreparserRecord record;
    };
} LilyPreparserType;

enum LilyPreparserDeclKind
{
    LILY_PREPARSER_DECL_KIND_CONSTANT,
    LILY_PREPARSER_DECL_KIND_FUN,
    LILY_PREPARSER_DECL_KIND_MODULE,
    LILY_PREPARSER_DECL_KIND_OBJECT,
    LILY_PREPARSER_DECL_KIND_TYPE,
};

typedef struct LilyPreparserDecl
{
    enum LilyPreparserDeclKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserFun fun;
        LilyPreparserModule module;
        LilyPreparserObject object;
        LilyPreparserType type;
    };
} LilyPreparserDecl;

typedef struct LilyPreparserInfo
{
    Vec *public_imports;  // Vec<LilyPreparserImport*>*
    Vec *private_imports; // Vec<LilyPreparserImport*>*
    Vec *public_macros;   // Vec<LilyPreparserMacro*>*
    Vec *private_macros;  // Vec<LilyPreparserMacro*>*
    Vec *decls;           // Vec<LilyPreparserDecl*>*
    LilyPreparserPackage *package;
} LilyPreparserInfo;

/**
 *
 * @brief Construct LilyPreparserInfo type.
 */
inline CONSTRUCTOR(LilyPreparserInfo, LilyPreparserInfo, String *package_name);

/**
 *
 * @brief Free LilyPreparserInfo type.
 */
DESTRUCTOR(LilyPreparserInfo, const LilyPreparserInfo *self);

typedef struct LilyPreparser
{
    const File *file;
    const Vec *tokens; // Vec<LilyToken*>*(&)
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
                   const File *file,
                   const Vec *tokens);

/**
 *
 * @brief Run the preparser.
 */
void
run__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info);

#endif // LILY_CORE_LILY_PREPARSER_H
