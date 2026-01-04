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

#ifndef LILY_CORE_LILY_PREPARSER_H
#define LILY_CORE_LILY_PREPARSER_H

#include <base/macros.h>
#include <base/string.h>

#include <core/lily/scanner/scanner.h>
#include <core/lily/scanner/token.h>
#include <core/lily/shared/visibility.h>

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
    Vec *params; // Vec<Vec<LilyToken* (&)>*>*?
    // NOTE: all the tokens are not free, except for the last tokens which must
    // be free.
    Vec *tokens; // Vec<LilyToken* (&)>*
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
    String *global_name;
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

typedef struct LilyPreparserMacroExpand
{
    String *name;
    Vec *params; // Vec<Vec<LilyToken* (&)>*>*?
} LilyPreparserMacroExpand;

/**
 *
 * @brief Convert LilyPreparserMacroExpand in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserMacroExpand,
               const LilyPreparserMacroExpand *self);
#endif

typedef struct LilyPreparserTest
{
    String *name;
    // TODO: change data type of body (Vec<LilyToken*>* ->
    // Vec<LilyPreparserFunBodyItem*>*)
    // TODO: update its constructor and its destructor.
    Vec *body; // Vec<LilyToken* (&)>*
} LilyPreparserTest;

typedef struct LilyPreparserFunBodyItem LilyPreparserFunBodyItem;

typedef struct LilyPreparserFunBodyItemExprs
{
    Vec *tokens; // Vec<LilyToken* (&)>*
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

typedef struct LilyPreparserFunBodyItemLambda
{
    String *name;          // String*?
    Vec *params;           // Vec<Vec<LilyToken* (&)>*>*?
    Vec *return_data_type; // Vec<LilyToken* (&)>*?
    LilyPreparserFunBodyItem *item;
    Vec *params_call; // Vec<Vec<LilyToken* (&)>*>*?
} LilyPreparserFunBodyItemLambda;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemLambda in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemLambda,
               const LilyPreparserFunBodyItemLambda *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtAsm
{
    Vec *params; // Vec<Vec<LilyToken* (&)>*>*
} LilyPreparserFunBodyItemStmtAsm;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtAsm in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtAsm,
               const LilyPreparserFunBodyItemStmtAsm *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtAwait
{
    Vec *expr; // Vec<LilyToken* (&)>*
} LilyPreparserFunBodyItemStmtAwait;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtAwait in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtAwait,
               const LilyPreparserFunBodyItemStmtAwait *self);
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

typedef struct LilyPreparserFunBodyItemStmtBreak
{
    String *name; // String*?
} LilyPreparserFunBodyItemStmtBreak;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtBreak in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtBreak,
               const LilyPreparserFunBodyItemStmtBreak *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtDefer
{
    LilyPreparserFunBodyItem *item;
} LilyPreparserFunBodyItemStmtDefer;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtDefer in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtDefer,
               const LilyPreparserFunBodyItemStmtDefer *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtDrop
{
    Vec *expr; // Vec<LilyToken* (&)>*
} LilyPreparserFunBodyItemStmtDrop;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtDrop in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtDrop,
               const LilyPreparserFunBodyItemStmtDrop *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtFor
{
    String *name; // String*?
    Vec *expr;    // Vec<LilyToken* (&)>*
    Vec *capture; // Vec<LilyToken* (&)>*?
    Vec *block;   // Vec<LilyPreparserFunBodyItem*>*
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
    Vec *if_expr;       // Vec<LilyToken* (&)>*
    Vec *if_capture;    // Vec<LilyToken* (&)>*?
    Vec *if_block;      // Vec<LilyPreparserFunBodyItem*>*
    Vec *elif_exprs;    // Vec<Vec<LilyToken* (&)>*?
    Vec *elif_captures; // Vec<Vec<LilyToken* (&)>*?>*?
    Vec *elif_blocks;   // Vec<Vec<LilyPreparserFunBodyItem*>*>*?
    Vec *else_block;    // Vec<LilyPreparserFunBodyItem>*?
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
    Vec *expr;          // Vec<LilyToken* (&)>*
    Vec *patterns;      // Vec<Vec<LilyToken* (&)>*>*
    Vec *pattern_conds; // Vec<Vec<LilyToken* (&)>*?>*
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

typedef struct LilyPreparserFunBodyItemStmtNext
{
    String *name; // String*?
} LilyPreparserFunBodyItemStmtNext;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtNext in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtNext,
               const LilyPreparserFunBodyItemStmtNext *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtRaise
{
    Vec *expr; // Vec<LilyToken* (&)>*
} LilyPreparserFunBodyItemStmtRaise;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtRaise in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtRaise,
               const LilyPreparserFunBodyItemStmtRaise *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtReturn
{
    Vec *expr; // Vec<LilyToken* (&)>*?
} LilyPreparserFunBodyItemStmtReturn;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtReturn in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtReturn,
               const LilyPreparserFunBodyItemStmtReturn *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtTry
{
    Vec *block;       // Vec<LilyPreparserFunBodyItem*>*
    Vec *catch_expr;  // Vec<LilyToken* (&)>*?
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

typedef struct LilyPreparserFunBodyItemStmtUnsafe
{
    Vec *block; // Vec<LilyPreparserFunBodyItem*>*
} LilyPreparserFunBodyItemStmtUnsafe;

/**
 *
 * @brief Convert LilyPreparserFunBodyItemStmtUnsafe in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtUnsafe,
               const LilyPreparserFunBodyItemStmtUnsafe *self);
#endif

typedef struct LilyPreparserFunBodyItemStmtVariable
{
    String *name;
    Vec *data_type; // Vec<LilyToken* (&)>*?
    Vec *expr;      // Vec<LilyToken* (&)>*
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
    Vec *expr;  // Vec<LilyToken* (&)>*
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
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE,
    LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE
};

/**
 *
 * @brief Convert LilyPreparserFunBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemKind,
               enum LilyPreparserFunBodyItemKind self);
#endif

// LilyPreparserFunBodyItem is used for body of functions and methods.
struct LilyPreparserFunBodyItem
{
    enum LilyPreparserFunBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserFunBodyItemExprs exprs;
        LilyPreparserFunBodyItemLambda lambda;
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserFunBodyItemStmtAsm stmt_asm;
        LilyPreparserFunBodyItemStmtAwait stmt_await;
        LilyPreparserFunBodyItemStmtBlock stmt_block;
        LilyPreparserFunBodyItemStmtBreak stmt_break;
        LilyPreparserFunBodyItemStmtDefer stmt_defer;
        LilyPreparserFunBodyItemStmtDrop stmt_drop;
        LilyPreparserFunBodyItemStmtFor stmt_for;
        LilyPreparserFunBodyItemStmtIf stmt_if;
        LilyPreparserFunBodyItemStmtMatch stmt_match;
        LilyPreparserFunBodyItemStmtNext stmt_next;
        LilyPreparserFunBodyItemStmtRaise stmt_raise;
        LilyPreparserFunBodyItemStmtReturn stmt_return;
        LilyPreparserFunBodyItemStmtTry stmt_try;
        LilyPreparserFunBodyItemStmtUnsafe stmt_unsafe;
        LilyPreparserFunBodyItemStmtVariable stmt_var;
        LilyPreparserFunBodyItemStmtWhile stmt_while;
    };
};

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

/**
 *
 * @brief Free LilyPreparserFunBodyItem type.
 */
DESTRUCTOR(LilyPreparserFunBodyItem, LilyPreparserFunBodyItem *self);

typedef struct LilyPreparserFun
{
    String *name;
    String *object_impl;   // String*?
    Vec *generic_params;   // Vec<Vec<LilyToken* (&)>*>*?
    Vec *params;           // Vec<Vec<LilyToken* (&)>*>*?
    Vec *return_data_type; // Vec<LilyToken* (&)>*?
    Vec *body;             // Vec<LilyPreparserFunBodyItem*>*
    Vec *req;              // Vec<Vec<LilyToken* (&)>*>*?
    Vec *when;             // Vec<Vec<LilyToken* (&)>*>*?
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
    Vec *expr;      // Vec<LilyToken* (&)>*
    Vec *data_type; // Vec<LilyToken* (&)>*
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
               enum LilyPreparserConstantKind self);
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
    LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND,
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
               enum LilyPreparserClassBodyItemKind self);
#endif

typedef LilyPreparserFun LilyPreparserMethod;

/**
 *
 * @brief Convert LilyPreparserMethod in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMethod, const LilyPreparserMethod *self);
#endif

typedef struct LilyPreparserAttribute
{
    String *name;
    Vec *data_type;    // Vec<LilyToken* (&)>*
    Vec *default_expr; // Vec<LilyToken* (&)>*?
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
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserMethod method;
    };
} LilyPreparserClassBodyItem;

/**
 *
 * @brief Convert LilyPreparserClassBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserClassBodyItem,
               const LilyPreparserClassBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserClassBodyItem type.
 */
DESTRUCTOR(LilyPreparserClassBodyItem, LilyPreparserClassBodyItem *self);

typedef struct LilyPreparserClass
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *inherits;       // Vec<Vec<LilyToken* (&)>*>*?
    Vec *implements;     // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserClassBodyItem*>*
    enum LilyVisibility visibility;
    bool is_close;
} LilyPreparserClass;

/**
 *
 * @brief Convert LilyPreparserClass in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserClass, const LilyPreparserClass *self);
#endif

enum LilyPreparserTraitBodyItemKind
{
    LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE,
    LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND,
    LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE,
};

/**
 *
 * @brief Convert LilyPreparserTraitBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTraitBodyItemKind,
               enum LilyPreparserTraitBodyItemKind self);
#endif

typedef struct LilyPreparserPrototype
{
    String *name;
    Vec *generic_params;   // Vec<Vec<LilyToken* (&)>*>*?
    Vec *params;           // Vec<Vec<LilyToken* (&)>*>*?
    Vec *return_data_type; // Vec<LilyToken* (&)>*?
} LilyPreparserPrototype;

/**
 *
 * @brief Convert LilyPreparserPrototype in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserPrototype,
               const LilyPreparserPrototype *self);
#endif

typedef struct LilyPreparserTraitBodyItem
{
    enum LilyPreparserTraitBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserAttribute attribute;
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserPrototype prototype;
    };
} LilyPreparserTraitBodyItem;

/**
 *
 * @brief Convert LilyPreparserTraitBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTraitBodyItem,
               const LilyPreparserTraitBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserTraitBodyItem type.
 */
DESTRUCTOR(LilyPreparserTraitBodyItem, LilyPreparserTraitBodyItem *self);

typedef struct LilyPreparserTrait
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *inherits;       // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserTraitBodyItem*>*
    enum LilyVisibility visibility;
    bool is_close;
} LilyPreparserTrait;

/**
 *
 * @brief Convert LilyPreparserTrait in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserTrait, const LilyPreparserTrait *self);
#endif

typedef struct LilyPreparserRecordField
{
    String *name;
    Vec *data_type;                 // Vec<LilyToken* (&)>*
    Vec *optional_expr;             // Vec<LilyToken* (&)>*?
    enum LilyVisibility visibility; // only used in record object
    bool is_mut;                    // only used in record type
} LilyPreparserRecordField;

/**
 *
 * @brief Convert LilyPreparserRecordField in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordField,
               const LilyPreparserRecordField *self);
#endif

enum LilyPreparserRecordObjectBodyItemKind
{
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT,
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD,
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND,
    LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD,
};

/**
 *
 * @brief Convert LilyPreparserRecordObjectItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObjectBodyItemKind,
               enum LilyPreparserRecordObjectBodyItemKind self);
#endif

typedef struct LilyPreparserRecordObjectBodyItem
{
    enum LilyPreparserRecordObjectBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserRecordField field;
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserMethod method;
    };
} LilyPreparserRecordObjectBodyItem;

/**
 *
 * @brief Convert LilyPreparserRecordObjectBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObjectBodyItem,
               const LilyPreparserRecordObjectBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserRecordObjectBodyItem type.
 */
DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
           LilyPreparserRecordObjectBodyItem *self);

typedef struct LilyPreparserRecordObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *implements;     // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserRecordObjectBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserRecordObject;

/**
 *
 * @brief Convert LilyPreparserRecordObject in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObject,
               const LilyPreparserRecordObject *self);
#endif

typedef struct LilyPreparserEnumVariant
{
    String *name;
    Vec *data_type; // Vec<LilyToken* (&)>*?
} LilyPreparserEnumVariant;

/**
 *
 * @brief Convert LilyPreparserEnumVariant in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumVariant,
               const LilyPreparserEnumVariant *self);
#endif

enum LilyPreparserEnumObjectBodyItemKind
{
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT,
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND,
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD,
    LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT,
};

/**
 *
 * @brief Convert LilyPreparserEnumObjectBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObjectBodyItemKind,
               enum LilyPreparserEnumObjectBodyItemKind self);
#endif

typedef struct LilyPreparserEnumObjectBodyItem
{
    enum LilyPreparserEnumObjectBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserMethod method;
        LilyPreparserEnumVariant variant;
    };
} LilyPreparserEnumObjectBodyItem;

/**
 *
 * @brief Convert LilyPreparserEnumObjectBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObjectBodyItem,
               const LilyPreparserEnumObjectBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserEnumObjectBodyItem type.
 */
DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
           LilyPreparserEnumObjectBodyItem *self);

typedef struct LilyPreparserEnumObject
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *implements;     // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserEnumObjectBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserEnumObject;

/**
 *
 * @brief Convert LilyPreparserEnumObject in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObject,
               const LilyPreparserEnumObject *self);
#endif

enum LilyPreparserObjectKind
{
    LILY_PREPARSER_OBJECT_KIND_CLASS,
    LILY_PREPARSER_OBJECT_KIND_TRAIT,
    LILY_PREPARSER_OBJECT_KIND_RECORD,
    LILY_PREPARSER_OBJECT_KIND_ENUM
};

/**
 *
 * @brief Convert LilyPreparserObjectKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserObjectKind,
               enum LilyPreparserObjectKind self);
#endif

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

/**
 *
 * @brief Convert LilyPreparserObject in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserObject, const LilyPreparserObject *self);
#endif

typedef struct LilyPreparserAlias
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *data_type;      // Vec<LilyToken* (&)>*
    enum LilyVisibility visibility;
} LilyPreparserAlias;

/**
 *
 * @brief Convert LilyPreparserAlias in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserAlias, const LilyPreparserAlias *self);
#endif

enum LilyPreparserEnumBodyItemKind
{
    LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND,
    LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT
};

/**
 *
 * @brief Convert LilyPreparserAlias in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumBodyItemKind,
               enum LilyPreparserEnumBodyItemKind self);
#endif

typedef struct LilyPreparserEnumBodyItem
{
    enum LilyPreparserEnumBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserEnumVariant variant;
    };
} LilyPreparserEnumBodyItem;

/**
 *
 * @brief Convert LilyPreparserEnumBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumBodyItem,
               const LilyPreparserEnumBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserEnumBodyItem type.
 */
DESTRUCTOR(LilyPreparserEnumBodyItem, LilyPreparserEnumBodyItem *self);

typedef struct LilyPreparserEnum
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserEnumBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserEnum;

/**
 *
 * @brief Convert LilyPreparserEnum in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserEnum, const LilyPreparserEnum *self);
#endif

enum LilyPreparserRecordBodyItemKind
{
    LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD,
    LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND,
};

/**
 *
 * @brief Convert LilyPreparserRecordBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordBodyItemKind,
               enum LilyPreparserRecordBodyItemKind self);
#endif

typedef struct LilyPreparserRecordBodyItem
{
    enum LilyPreparserRecordBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserRecordField field;
        LilyPreparserMacroExpand macro_expand;
    };
} LilyPreparserRecordBodyItem;

/**
 *
 * @brief Convert LilyPreparserRecordBodyItem in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordBodyItem,
               const LilyPreparserRecordBodyItem *self);
#endif

/**
 *
 * @brief Free LilyPreparserRecordBodyItem type.
 */
DESTRUCTOR(LilyPreparserRecordBodyItem, LilyPreparserRecordBodyItem *self);

typedef struct LilyPreparserRecord
{
    String *name;
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body;           // Vec<LilyPreparserRecordBodyItem*>*
    enum LilyVisibility visibility;
} LilyPreparserRecord;

/**
 *
 * @brief Convert LilyPreparserRecord in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserRecord, const LilyPreparserRecord *self);
#endif

enum LilyPreparserTypeKind
{
    LILY_PREPARSER_TYPE_KIND_ALIAS,
    LILY_PREPARSER_TYPE_KIND_ENUM,
    LILY_PREPARSER_TYPE_KIND_RECORD,
};

/**
 *
 * @brief Convert LilyPreparserTypeKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTypeKind,
               enum LilyPreparserTypeKind self);
#endif

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

/**
 *
 * @brief Convert LilyPreparserType in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserType, const LilyPreparserType *self);
#endif

typedef struct LilyPreparserLibConstantPrototype
{
    String *name;
    Vec *data_type; // Vec<LilyToken* (&)>*
} LilyPreparserLibConstantPrototype;

/**
 *
 * @brief Convert LilyPreparserLibConstantPrototype in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibConstantPrototype,
               const LilyPreparserLibConstantPrototype *self);
#endif

typedef struct LilyPreparserLibFunPrototype
{
    String *name;
    String *new_name;      // String*?
    Vec *params;           // Vec<Vec<LilyToken* (&)>*>*?
    Vec *return_data_type; // Vec<LilyToken* (&)>*
} LilyPreparserLibFunPrototype;

/**
 *
 * @brief Convert LilyPreparserLibFunPrototype in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibFunPrototype,
               const LilyPreparserLibFunPrototype *self);
#endif

typedef struct LilyPreparserLibTypePrototype
{
    // TODO: complete this type definition
} LilyPreparserLibTypePrototype;

typedef struct LilyPreparserLibObjectPrototype
{
    // TODO: complete this type definition
} LilyPreparserLibObjectPrototype;

enum LilyPreparserLibBodyItemKind
{
    LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT,
    LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN,
    LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE,
    LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT,
};

/**
 *
 * @brief Convert LilyPreparserLibBodyItemKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibBodyItemKind,
               enum LilyPreparserLibBodyItemKind self);
#endif

typedef struct LilyPreparserLibBodyItem
{
    enum LilyPreparserLibBodyItemKind kind;
    Location location;
    union
    {
        LilyPreparserLibConstantPrototype constant;
        LilyPreparserLibFunPrototype fun;
        LilyPreparserLibTypePrototype type;
        LilyPreparserLibObjectPrototype object;
    };
} LilyPreparserLibBodyItem;

/**
 *
 * @brief Convert LilyPreparserLibBodyItem in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibBodyItem,
               const LilyPreparserLibBodyItem *self);
#endif

enum LilyPreparserLibFrom
{
    LILY_PREPARSER_LIB_FROM_CC,
    LILY_PREPARSER_LIB_FROM_CPP,
};

/**
 *
 * @brief Convert LilyPreparserLibFrom in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyPreparserLibFrom, enum LilyPreparserLibFrom self);
#endif

typedef struct LilyPreparserLib
{
    String *name; // String*?
    enum LilyPreparserLibFrom from;
    Vec *body; // Vec<LilyPreparserLibBodyItem*>*
} LilyPreparserLib;

/**
 *
 * @brief Convert LilyPreparserLib in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserLib, const LilyPreparserLib *self);
#endif

typedef struct LilyPreparserError
{
    String *name;
    Vec *data_type;      // Vec<LilyToken* (&)>*?
    Vec *generic_params; // Vec<Vec<LilyToken* (&)>*>*?
    enum LilyVisibility visibility;
} LilyPreparserError;

/**
 *
 * @brief Convert LilyPreparserError in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserError, const LilyPreparserError *self);
#endif

typedef struct LilyPreparserUse
{
    Vec *path; // Vec<LilyToken* (&)>*
} LilyPreparserUse;

/**
 *
 * @brief Convert LilyPreparserUse in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserUse, const LilyPreparserUse *self);
#endif

typedef struct LilyPreparserInclude
{
    Vec *path; // Vec<LilyToken* (&)>*
} LilyPreparserInclude;

/**
 *
 * @brief Convert LilyPreparserInclude in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserInclude,
               const LilyPreparserInclude *self);
#endif

enum LilyPreparserDeclKind
{
    LILY_PREPARSER_DECL_KIND_CONSTANT,
    LILY_PREPARSER_DECL_KIND_ERROR,
    LILY_PREPARSER_DECL_KIND_FUN,
    LILY_PREPARSER_DECL_KIND_INCLUDE,
    LILY_PREPARSER_DECL_KIND_LIB,
    LILY_PREPARSER_DECL_KIND_MACRO_EXPAND,
    LILY_PREPARSER_DECL_KIND_MODULE,
    LILY_PREPARSER_DECL_KIND_OBJECT,
    LILY_PREPARSER_DECL_KIND_TYPE,
    LILY_PREPARSER_DECL_KIND_USE
};

/**
 *
 * @brief Convert LilyPreparserDeclKind in string.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserDeclKind,
               enum LilyPreparserDeclKind self);
#endif

typedef struct LilyPreparserDecl
{
    enum LilyPreparserDeclKind kind;
    Location location;
    union
    {
        LilyPreparserConstant constant;
        LilyPreparserError error;
        LilyPreparserFun fun;
        LilyPreparserInclude include;
        LilyPreparserLib lib;
        LilyPreparserMacroExpand macro_expand;
        LilyPreparserModule module;
        LilyPreparserObject object;
        LilyPreparserType type;
        LilyPreparserUse use;
    };
} LilyPreparserDecl;

/**
 *
 * @brief Convert LilyPreparserDecl in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserDecl, const LilyPreparserDecl *self);
#endif

/**
 *
 * @brief Print debug LilyPreparserDecl struct.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
void
IMPL_FOR_DEBUG(debug, LilyPreparserDecl, const LilyPreparserDecl *self);
#endif

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
CONSTRUCTOR(LilyPreparserInfo, LilyPreparserInfo, String *package_name);

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
    Usize count_warning;
    const char *default_package_access; // const char*?
    bool destroy_all; // It's true when run until preparser or run until
                      // precompiler is enable (or in the preparser or
                      // precompiler test). NOTE: These are just a few examples,
                      // you can apply it in other cases if necessary.
} LilyPreparser;

/**
 *
 * @brief Construct LilyPreparser type.
 */
CONSTRUCTOR(LilyPreparser,
            LilyPreparser,
            const File *file,
            const Vec *tokens,
            const char *default_package_access,
            bool destroy_all);

/**
 *
 * @brief Preparse the body of record.
 * @return Vec<LilyPreparserRecordBodyItem*>*?
 */
Vec *
preparse_record_body__LilyPreparser(LilyPreparser *self);

/**
 *
 * @brief Preparse the body of enum.
 * @return Vec<LilyPreparserEnumBodyItem*>*?
 */
Vec *
preparse_enum_body__LilyPreparser(LilyPreparser *self);

/*
 *
 * @brief Preparse the body of class.
 * @return Vec<LilyPreparserClassBodyItem*>*?
 */
Vec *
preparse_class_body__LilyPreparser(LilyPreparser *self);

/*
 *
 * @brief Preparse the body of record object.
 * @return Vec<LilyPreparserRecordObjectBodyItem*>*?
 */
Vec *
preparse_record_object_body__LilyPreparser(LilyPreparser *self);

/**
 *
 * @brief Preparse the body of enum object.
 * @return Vec<LilyPreparserEnumObjectBodyItem*>*?
 */
Vec *
preparse_enum_object_body__LilyPreparser(LilyPreparser *self);

/**
 *
 * @brief Preparse the body of trait.
 * @return Vec<LilyPreparserTraitBodyItem*>*?
 */
Vec *
preparse_trait_body__LilyPreparser(LilyPreparser *self);

/**
 *
 * @brief Preparse a block (stmt, expr, ...).
 */
LilyPreparserFunBodyItem *
preparse_block__LilyPreparser(LilyPreparser *self,
                              bool (*must_close)(LilyPreparser *),
                              bool parse_semicolon);

/**
 *
 * @brief Preparse a lambda expression.
 * @return LilyPreparserFunBodyItem*?
 */
LilyPreparserFunBodyItem *
preparse_lambda__LilyPreparser(LilyPreparser *self);

/**
 *
 * @brief Run the preparser.
 */
void
run__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info);

#endif // LILY_CORE_LILY_PREPARSER_H
