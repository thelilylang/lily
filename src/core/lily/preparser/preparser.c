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

#include <base/assert.h>
#include <base/new.h>

#include <core/lily/diagnostic/error.h>
#include <core/lily/lily.h>
#include <core/lily/preparser/preparser.h>
#include <core/lily/scanner/token.h>
#include <core/shared/diagnostic.h>

#include <ctype.h>
#include <stdio.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#include <base/print.h>
#endif

// Construct LilyPreparserImport type.
static CONSTRUCTOR(LilyPreparserImport *,
                   LilyPreparserImport,
                   String *value,
                   String *as,
                   Location location);

// Construct LilyPreparserMacro type.
static CONSTRUCTOR(LilyPreparserMacro *,
                   LilyPreparserMacro,
                   String *name,
                   Vec *params,
                   Vec *tokens,
                   Location location);

// Construct LilyPreparserSubPackage type.
static CONSTRUCTOR(LilyPreparserSubPackage *,
                   LilyPreparserSubPackage,
                   enum LilyVisibility visibility,
                   String *name,
                   String *global_name);

// Construct LilyPreparserPackage type.
static CONSTRUCTOR(LilyPreparserPackage *, LilyPreparserPackage, String *name);

// Construct LilyPreparserModule type.
static inline CONSTRUCTOR(LilyPreparserModule,
                          LilyPreparserModule,
                          String *name,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserModule type.
static DESTRUCTOR(LilyPreparserModule, const LilyPreparserModule *self);

// Construct LilyPreparserTest type.
static inline CONSTRUCTOR(LilyPreparserTest,
                          LilyPreparserTest,
                          String *name,
                          Vec *body);

// Free LilyPreparserTest type.
static DESTRUCTOR(LilyPreparserTest, const LilyPreparserTest *self);

// Construct LilyPreparserFunBodyItemExprs type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemExprs,
                          LilyPreparserFunBodyItemExprs,
                          Vec *tokens);

// Free LilyPreparserFunBodyItemExprs type.
static DESTRUCTOR(LilyPreparserFunBodyItemExprs,
                  const LilyPreparserFunBodyItemExprs *self);

// Construct LilyPreparserFunBodyItemLambda type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemLambda,
                          LilyPreparserFunBodyItemLambda,
                          String *name,
                          Vec *params,
                          Vec *return_data_type,
                          LilyPreparserFunBodyItem *item,
                          Vec *params_call);

// Free LilyPreparserFunBodyItemExprs type.
static DESTRUCTOR(LilyPreparserFunBodyItemLambda,
                  const LilyPreparserFunBodyItemLambda *self);

// Construct LilyPreparserFunBodyItemStmtAsm type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtAsm,
                          LilyPreparserFunBodyItemStmtAsm,
                          Vec *params);

// Free LilyPreparserFunBodyItemStmtAsm type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtAsm,
                  const LilyPreparserFunBodyItemStmtAsm *self);

// Construct LilyPreparserFunBodyItemStmtAwait type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtAwait,
                          LilyPreparserFunBodyItemStmtAwait,
                          Vec *expr);

// Free LilyPreparserFunBodyItemStmtAwait type.
static inline DESTRUCTOR(LilyPreparserFunBodyItemStmtAwait,
                         const LilyPreparserFunBodyItemStmtAwait *self);

// Construct LilyPreparserFunBodyItemStmtBlock type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
                          LilyPreparserFunBodyItemStmtBlock,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtBlock type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
                  const LilyPreparserFunBodyItemStmtBlock *self);

// Construct LilyPreparserFunBodyItemStmtBreak type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtBreak,
                          LilyPreparserFunBodyItemStmtBreak,
                          String *name);

// Free LilyPreparserFunBodyItemStmtBreak type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtBreak,
                  const LilyPreparserFunBodyItemStmtBreak *self);

// Construct LilyPreparserFunBodyItemStmtDefer type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtDefer,
                          LilyPreparserFunBodyItemStmtDefer,
                          LilyPreparserFunBodyItem *item);

// Free LilyPreparserFunBodyItemStmtDefer type.
static inline DESTRUCTOR(LilyPreparserFunBodyItemStmtDefer,
                         const LilyPreparserFunBodyItemStmtDefer *self);

// Construct LilyPreparserFunBodyItemStmtDrop type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtDrop,
                          LilyPreparserFunBodyItemStmtDrop,
                          Vec *expr);

// Free LilyPreparserFunBodyItemStmtDrop type.
static inline DESTRUCTOR(LilyPreparserFunBodyItemStmtDrop,
                         const LilyPreparserFunBodyItemStmtDrop *self);

// Construct LilyPreparserFunBodyItemStmtFor type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtFor,
                          LilyPreparserFunBodyItemStmtFor,
                          String *name,
                          Vec *expr,
                          Vec *capture,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtFor type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtFor,
                  const LilyPreparserFunBodyItemStmtFor *self);

// Construct LilyPreparserFunBodyItemStmtIf type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtIf,
                          LilyPreparserFunBodyItemStmtIf,
                          Vec *if_expr,
                          Vec *if_capture,
                          Vec *if_block,
                          Vec *elif_exprs,
                          Vec *elif_captures,
                          Vec *elif_blocks,
                          Vec *else_block);

// Free LilyPreparserFunBodyItemStmtIf type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtIf,
                  const LilyPreparserFunBodyItemStmtIf *self);

// Construct LilyPreparserFunBodyItemStmtMatch type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtMatch,
                          LilyPreparserFunBodyItemStmtMatch,
                          Vec *expr,
                          Vec *patterns,
                          Vec *pattern_conds,
                          Vec *blocks);

// Free LilyPreparserFunBodyItemStmtMatch type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtMatch,
                  const LilyPreparserFunBodyItemStmtMatch *self);

// Construct LilyPreparserFunBodyItemStmtNext type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtNext,
                          LilyPreparserFunBodyItemStmtNext,
                          String *name);

// Free LilyPreparserFunBodyItemStmtNext type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtNext,
                  const LilyPreparserFunBodyItemStmtNext *self);

// Construct LilyPreparserFunBodyItemStmtRaise type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtRaise,
                          LilyPreparserFunBodyItemStmtRaise,
                          Vec *expr);

// Free LilyPreparserFunBodyItemStmtRaise type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtRaise,
                  const LilyPreparserFunBodyItemStmtRaise *self);

// Construct LilyPreparserFunBodyItemStmtReturn type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtReturn,
                          LilyPreparserFunBodyItemStmtReturn,
                          Vec *expr);

// Free LilyPreparserFunBodyItemStmtReturn type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtReturn,
                  const LilyPreparserFunBodyItemStmtReturn *self);

// Construct LilyPreparserFunBodyItemStmtTry type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtTry,
                          LilyPreparserFunBodyItemStmtTry,
                          Vec *block,
                          Vec *catch_expr,
                          Vec *catch_block);

// Free LilyPreparserFunBodyItemStmtTry type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtTry,
                  const LilyPreparserFunBodyItemStmtTry *self);

// Construct LilyPreparserFunBodyItemStmtUnsafe type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtUnsafe,
                          LilyPreparserFunBodyItemStmtUnsafe,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtUnsafe type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtUnsafe,
                  const LilyPreparserFunBodyItemStmtUnsafe *self);

// Construct LilyPreparserFunBodyItemStmtVariable type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtVariable,
                          LilyPreparserFunBodyItemStmtVariable,
                          String *name,
                          Vec *data_type,
                          Vec *expr,
                          bool is_mut,
                          bool is_trace,
                          bool is_ref,
                          bool is_drop);

// Free LilyPreparserFunBodyItemStmtVariable type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtVariable,
                  const LilyPreparserFunBodyItemStmtVariable *self);

// Construct LilyPreparserFunBodyItemWhile type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtWhile,
                          LilyPreparserFunBodyItemStmtWhile,
                          Vec *expr,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtWhile type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtWhile,
                  const LilyPreparserFunBodyItemStmtWhile *self);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           exprs,
                           LilyPreparserFunBodyItemExprs exprs,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           lambda,
                           LilyPreparserFunBodyItemLambda lambda,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           macro_expand,
                           LilyPreparserMacroExpand macro_expand,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_asm,
                           LilyPreparserFunBodyItemStmtAsm stmt_asm,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_await,
                           LilyPreparserFunBodyItemStmtAwait stmt_await,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_block,
                           LilyPreparserFunBodyItemStmtBlock stmt_block,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_break,
                           LilyPreparserFunBodyItemStmtBreak stmt_break,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_defer,
                           LilyPreparserFunBodyItemStmtDefer stmt_defer,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_drop,
                           LilyPreparserFunBodyItemStmtDrop stmt_drop,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_for,
                           LilyPreparserFunBodyItemStmtFor stmt_for,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_if,
                           LilyPreparserFunBodyItemStmtIf stmt_if,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_match,
                           LilyPreparserFunBodyItemStmtMatch stmt_match,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_next,
                           LilyPreparserFunBodyItemStmtNext stmt_next,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_raise,
                           LilyPreparserFunBodyItemStmtRaise stmt_raise,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_return,
                           LilyPreparserFunBodyItemStmtReturn stmt_return,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_try,
                           LilyPreparserFunBodyItemStmtTry stmt_try,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_unsafe,
                           LilyPreparserFunBodyItemStmtUnsafe stmt_unsafe,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_var,
                           LilyPreparserFunBodyItemStmtVariable stmt_var,
                           Location location);

// Construct LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_while,
                           LilyPreparserFunBodyItemStmtWhile stmt_while,
                           Location location);

// Free LilyPreparserFunBodyItem type (LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          exprs,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          lambda,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          macro_expand,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_asm,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_await,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_block,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_break,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_defer,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_drop,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_for,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_if,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_match,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_next,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_raise,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_return,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_try,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_unsafe,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_var,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_while,
                          LilyPreparserFunBodyItem *self);

// Construct LilyPreparserFun type.
static inline CONSTRUCTOR(LilyPreparserFun,
                          LilyPreparserFun,
                          String *name,
                          String *object_impl,
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
                          bool when_is_comptime);

// Free LilyPreparserFun type.
static DESTRUCTOR(LilyPreparserFun, const LilyPreparserFun *self);

// Construct LilyPreparserAttribute type.
static inline CONSTRUCTOR(LilyPreparserAttribute,
                          LilyPreparserAttribute,
                          String *name,
                          Vec *data_type,
                          Vec *default_expr,
                          bool is_get,
                          bool is_set,
                          enum LilyVisibility visibility);

// Free LilyPreparserAttribute type.
static DESTRUCTOR(LilyPreparserAttribute, const LilyPreparserAttribute *self);

// Construct LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUT).
static VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                           LilyPreparserClassBodyItem,
                           attribute,
                           LilyPreparserAttribute attribute,
                           Location location);

// Construct LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                           LilyPreparserClassBodyItem,
                           macro_expand,
                           LilyPreparserMacroExpand macro_expand,
                           Location location);

// Construct LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD)
static VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                           LilyPreparserClassBodyItem,
                           method,
                           LilyPreparserMethod method,
                           Location location);

// Free LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUT).
static VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                          attribute,
                          LilyPreparserClassBodyItem *self);

// Free LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                          macro_expand,
                          LilyPreparserClassBodyItem *self);

// Free LilyPreparserClassBodyItem type
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                          method,
                          LilyPreparserClassBodyItem *self);

// Construct LilyPreparserClass type.
static inline CONSTRUCTOR(LilyPreparserClass,
                          LilyPreparserClass,
                          String *name,
                          Vec *generic_params,
                          Vec *inherits,
                          Vec *implements,
                          Vec *body,
                          enum LilyVisibility visibility,
                          bool is_close);

// Free LilyPreparserClass type.
static DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self);

// Construct LilyPreparserPrototype type.
static inline CONSTRUCTOR(LilyPreparserPrototype,
                          LilyPreparserPrototype,
                          String *name,
                          Vec *generic_params,
                          Vec *params,
                          Vec *return_data_type);

// Free LilyPreparserPrototype type.
static DESTRUCTOR(LilyPreparserPrototype, const LilyPreparserPrototype *self);

// Construct LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE).
static VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                           LilyPreparserTraitBodyItem,
                           attribute,
                           Location location,
                           LilyPreparserAttribute attribute);

// Construct LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                           LilyPreparserTraitBodyItem,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Construct LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE).
static VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                           LilyPreparserTraitBodyItem,
                           prototype,
                           Location location,
                           LilyPreparserPrototype prototype);

// Free LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE).
static VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                          attribute,
                          LilyPreparserTraitBodyItem *self);

// Free LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                          macro_expand,
                          LilyPreparserTraitBodyItem *self);

// Free LilyPreparserTraitBodyItem type
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE).
static VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                          prototype,
                          LilyPreparserTraitBodyItem *self);

// Construct LilyPreparserTrait type.
static inline CONSTRUCTOR(LilyPreparserTrait,
                          LilyPreparserTrait,
                          String *name,
                          Vec *generic_params,
                          Vec *inherits,
                          Vec *body,
                          enum LilyVisibility visibility,
                          bool is_close);

// Free LilyPreparserTrait type.
static DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self);

// Construct LilyPreparserRecordField type.
static inline CONSTRUCTOR(LilyPreparserRecordField *,
                          LilyPreparserRecordField,
                          String *name,
                          Vec *data_type,
                          Vec *optional_expr,
                          enum LilyVisibility visibility,
                          bool is_mut);

// Free LilyPreparserRecordField type.
static DESTRUCTOR(LilyPreparserRecordField,
                  const LilyPreparserRecordField *self);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                           LilyPreparserRecordObjectBodyItem,
                           constant,
                           Location location,
                           LilyPreparserConstant constant);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_FIELD).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                           LilyPreparserRecordObjectBodyItem,
                           field,
                           Location location,
                           LilyPreparserRecordField field);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                           LilyPreparserRecordObjectBodyItem,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_METHOD).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                           LilyPreparserRecordObjectBodyItem,
                           method,
                           Location location,
                           LilyPreparserMethod method);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                          constant,
                          LilyPreparserRecordObjectBodyItem *self);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_FIELD).
static VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                          field,
                          LilyPreparserRecordObjectBodyItem *self);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                          macro_expand,
                          LilyPreparserRecordObjectBodyItem *self);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                          method,
                          LilyPreparserRecordObjectBodyItem *self);

// Construct LilyPreparserRecordObject type.
static inline CONSTRUCTOR(LilyPreparserRecordObject,
                          LilyPreparserRecordObject,
                          String *name,
                          Vec *generic_params,
                          Vec *implements,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserRecordObject type.
static DESTRUCTOR(LilyPreparserRecordObject,
                  const LilyPreparserRecordObject *self);

// Construct LilyPreparserEnumVariant type.
static inline CONSTRUCTOR(LilyPreparserEnumVariant *,
                          LilyPreparserEnumVariant,
                          String *name,
                          Vec *data_type);

// Free LilyPreparserEnumVariant type.
static DESTRUCTOR(LilyPreparserEnumVariant,
                  const LilyPreparserEnumVariant *self);

// Construct LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                           LilyPreparserEnumObjectBodyItem,
                           constant,
                           Location location,
                           LilyPreparserConstant constant);

// Construct LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                           LilyPreparserEnumObjectBodyItem,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Construct LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_METHOD).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                           LilyPreparserEnumObjectBodyItem,
                           method,
                           Location location,
                           LilyPreparserMethod method);

// Construct LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_VARIANT).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                           LilyPreparserEnumObjectBodyItem,
                           variant,
                           Location location,
                           LilyPreparserEnumVariant variant);

// Free LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          constant,
                          LilyPreparserEnumObjectBodyItem *self);

// Free LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          macro_expand,
                          LilyPreparserEnumObjectBodyItem *self);

// Free LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          method,
                          LilyPreparserEnumObjectBodyItem *self);

// Free LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_VARIANT).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          variant,
                          LilyPreparserEnumObjectBodyItem *self);

// Construct LilyPreparserEnumObject type.
static inline CONSTRUCTOR(LilyPreparserEnumObject,
                          LilyPreparserEnumObject,
                          String *name,
                          Vec *generic_params,
                          Vec *implements,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserEnumObject type.
static DESTRUCTOR(LilyPreparserEnumObject, const LilyPreparserEnumObject *self);

// Construct LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_CLASS).
static inline VARIANT_CONSTRUCTOR(LilyPreparserObject,
                                  LilyPreparserObject,
                                  class,
                                  LilyPreparserClass class);

// Construct LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_TRAIT).
static inline VARIANT_CONSTRUCTOR(LilyPreparserObject,
                                  LilyPreparserObject,
                                  trait,
                                  LilyPreparserTrait trait);

// Construct LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_RECORD).
static inline VARIANT_CONSTRUCTOR(LilyPreparserObject,
                                  LilyPreparserObject,
                                  record,
                                  LilyPreparserRecordObject record);

// Construct LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_ENUM).
static inline VARIANT_CONSTRUCTOR(LilyPreparserObject,
                                  LilyPreparserObject,
                                  enum_,
                                  LilyPreparserEnumObject enum_);

// Free LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_CLASS).
static inline VARIANT_DESTRUCTOR(LilyPreparserObject,
                                 class,
                                 const LilyPreparserObject *self);

// Free LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_TRAIT).
static inline VARIANT_DESTRUCTOR(LilyPreparserObject,
                                 trait,
                                 const LilyPreparserObject *self);

// Free LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyPreparserObject,
                                 record,
                                 const LilyPreparserObject *self);

// Free LilyPreparserObject type (LILY_PREPARSER_OBJECT_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(LilyPreparserObject,
                                 enum_,
                                 const LilyPreparserObject *self);

// Free LilyPreparserObject type.
static DESTRUCTOR(LilyPreparserObject, const LilyPreparserObject *self);

// Construct LilyPreparserAlias type.
static inline CONSTRUCTOR(LilyPreparserAlias,
                          LilyPreparserAlias,
                          String *name,
                          Vec *generic_params,
                          Vec *data_type,
                          enum LilyVisibility visibility);

// Free LilyPreparserAlias type.
static DESTRUCTOR(LilyPreparserAlias, const LilyPreparserAlias *self);

// Construct LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumBodyItem *,
                           LilyPreparserEnumBodyItem,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Construct LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumBodyItem *,
                           LilyPreparserEnumBodyItem,
                           variant,
                           Location location,
                           LilyPreparserEnumVariant variant);

// Free LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserEnumBodyItem,
                          macro_expand,
                          LilyPreparserEnumBodyItem *self);

// Free LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT).
static VARIANT_DESTRUCTOR(LilyPreparserEnumBodyItem,
                          variant,
                          LilyPreparserEnumBodyItem *self);

// Construct LilyPreparserEnum type.
static inline CONSTRUCTOR(LilyPreparserEnum,
                          LilyPreparserEnum,
                          String *name,
                          Vec *generic_params,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserEnum type.
static DESTRUCTOR(LilyPreparserEnum, const LilyPreparserEnum *self);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordBodyItem *,
                           LilyPreparserRecordBodyItem,
                           field,
                           Location location,
                           LilyPreparserRecordField field);

// Construct LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserRecordBodyItem *,
                           LilyPreparserRecordBodyItem,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD)
static VARIANT_DESTRUCTOR(LilyPreparserRecordBodyItem,
                          field,
                          LilyPreparserRecordBodyItem *self);

// Free LilyPreparserRecordObjectBodyItem type
// (LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND)
static VARIANT_DESTRUCTOR(LilyPreparserRecordBodyItem,
                          macro_expand,
                          LilyPreparserRecordBodyItem *self);

// Construct LilyPreparserRecord type.
static inline CONSTRUCTOR(LilyPreparserRecord,
                          LilyPreparserRecord,
                          String *name,
                          Vec *generic_params,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserRecord type.
static DESTRUCTOR(LilyPreparserRecord, const LilyPreparserRecord *self);

// Construct LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_ALIAS).
static inline VARIANT_CONSTRUCTOR(LilyPreparserType,
                                  LilyPreparserType,
                                  alias,
                                  LilyPreparserAlias alias);

// Construct LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_ENUM).
static inline VARIANT_CONSTRUCTOR(LilyPreparserType,
                                  LilyPreparserType,
                                  enum_,
                                  LilyPreparserEnum enum_);

// Construct LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_RECORD).
static inline VARIANT_CONSTRUCTOR(LilyPreparserType,
                                  LilyPreparserType,
                                  record,
                                  LilyPreparserRecord record);

// Free LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_ALIAS).
static inline VARIANT_DESTRUCTOR(LilyPreparserType,
                                 alias,
                                 const LilyPreparserType *self);

// Free LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_ENUM).
static inline VARIANT_DESTRUCTOR(LilyPreparserType,
                                 enum_,
                                 const LilyPreparserType *self);

// Free LilyPreparserType type (LILY_PREPARSER_TYPE_KIND_RECORD).
static inline VARIANT_DESTRUCTOR(LilyPreparserType,
                                 record,
                                 const LilyPreparserType *self);

// Free LilyPreparserType type.
static DESTRUCTOR(LilyPreparserType, const LilyPreparserType *self);

// Construct LilyPreparserLibConstantPrototype type.
static inline CONSTRUCTOR(LilyPreparserLibConstantPrototype,
                          LilyPreparserLibConstantPrototype,
                          String *name,
                          Vec *data_type);

// Free LilyPreparserLibConstantPrototype type.
static DESTRUCTOR(LilyPreparserLibConstantPrototype,
                  const LilyPreparserLibConstantPrototype *self);

// Construct LilyPreparserLibFunPrototype type.
static inline CONSTRUCTOR(LilyPreparserLibFunPrototype,
                          LilyPreparserLibFunPrototype,
                          String *name,
                          String *new_name,
                          Vec *params,
                          Vec *return_data_type);

// Free LilyPreparserLibFunPrototype type.
static DESTRUCTOR(LilyPreparserLibFunPrototype,
                  const LilyPreparserLibFunPrototype *self);

// Construct LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT).
static VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                           LilyPreparserLibBodyItem,
                           constant,
                           Location location,
                           LilyPreparserLibConstantPrototype constant);

// Construct LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN).
static VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                           LilyPreparserLibBodyItem,
                           fun,
                           Location location,
                           LilyPreparserLibFunPrototype fun);

// Construct LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE).
static VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                           LilyPreparserLibBodyItem,
                           type,
                           Location location,
                           LilyPreparserLibTypePrototype type);

// Construct LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT).
static VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                           LilyPreparserLibBodyItem,
                           object,
                           Location location,
                           LilyPreparserLibObjectPrototype object);

// Free LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                          constant,
                          LilyPreparserLibBodyItem *self);

// Free LilyPreparserLibBodyItem type (LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                          fun,
                          LilyPreparserLibBodyItem *self);

// Free LilyPreparserLibBodyItem type (LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE).
static VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                          type,
                          LilyPreparserLibBodyItem *self);

// Free LilyPreparserLibBodyItem type
// (LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                          object,
                          LilyPreparserLibBodyItem *self);

// Free LilyPreparserLibBodyItem type.
static DESTRUCTOR(LilyPreparserLibBodyItem, LilyPreparserLibBodyItem *self);

// Construct LilyPreparserLib type.
static inline CONSTRUCTOR(LilyPreparserLib,
                          LilyPreparserLib,
                          String *name,
                          enum LilyPreparserLibFrom from,
                          Vec *body);

// Free LilyPreparserLib type.
static DESTRUCTOR(LilyPreparserLib, const LilyPreparserLib *self);

// Construct LilyPreparserError type.
static inline CONSTRUCTOR(LilyPreparserError,
                          LilyPreparserError,
                          String *name,
                          Vec *data_type,
                          Vec *generic_params,
                          enum LilyVisibility visibility);

// Free LilyPreparserError type.
static DESTRUCTOR(LilyPreparserError, const LilyPreparserError *self);

// Construct LilyPreparserUse type.
static inline CONSTRUCTOR(LilyPreparserUse, LilyPreparserUse, Vec *path);

// Free LilyPreparserUse type.
static inline DESTRUCTOR(LilyPreparserUse, const LilyPreparserUse *self);

// Construct LilyPreparserInclude type.
static inline CONSTRUCTOR(LilyPreparserInclude,
                          LilyPreparserInclude,
                          Vec *path);

// Free LilyPreparserInclude type.
static inline DESTRUCTOR(LilyPreparserInclude,
                         const LilyPreparserInclude *self);

// Construct LilyPreparserMacroExpand type.
static inline CONSTRUCTOR(LilyPreparserMacroExpand,
                          LilyPreparserMacroExpand,
                          String *name,
                          Vec *params);

// Free LilyPreparserMacroExpand type.
static DESTRUCTOR(LilyPreparserMacroExpand,
                  const LilyPreparserMacroExpand *self);

// Construct LilyPreparserConstantInfo type.
static CONSTRUCTOR(LilyPreparserConstantInfo *,
                   LilyPreparserConstantInfo,
                   String *name,
                   Vec *expr,
                   Vec *data_type,
                   enum LilyVisibility visibility);

// Free LilyPreparserConstantInfo type.
static DESTRUCTOR(LilyPreparserConstantInfo, LilyPreparserConstantInfo *self);

// Construct LilyPreparserConstant (LILY_PREPARSER_CONSTANT_KIND_SIMPLE).
static inline VARIANT_CONSTRUCTOR(LilyPreparserConstant,
                                  LilyPreparserConstant,
                                  simple,
                                  LilyPreparserConstantInfo *simple);

// Construct LilyPreparserConstant (LILY_PREPARSER_CONSTANT_KIND_MULTIPLE).
static inline VARIANT_CONSTRUCTOR(LilyPreparserConstant,
                                  LilyPreparserConstant,
                                  multiple,
                                  Vec *multiple);

// Free LilyPreparserConstant (LILY_PREPARSER_CONSTANT_KIND_SIMPLE).
static inline VARIANT_DESTRUCTOR(LilyPreparserConstant,
                                 simple,
                                 const LilyPreparserConstant *self);

// Free LilyPreparserConstant (LILY_PREPARSER_CONSTANT_KIND_MULTIPLE).
static VARIANT_DESTRUCTOR(LilyPreparserConstant,
                          multiple,
                          const LilyPreparserConstant *self);

// Free LilyPreparserConstant type.
static DESTRUCTOR(LilyPreparserConstant, const LilyPreparserConstant *self);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_CONSTANT).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           constant,
                           Location location,
                           LilyPreparserConstant constant);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_ERROR).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           error,
                           Location location,
                           LilyPreparserError error);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_FUN).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           fun,
                           Location location,
                           LilyPreparserFun fun);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_INCLUDE).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           include,
                           Location location,
                           LilyPreparserInclude include);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_LIB).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           lib,
                           Location location,
                           LilyPreparserLib lib);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_MACRO_EXPAND).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           macro_expand,
                           Location location,
                           LilyPreparserMacroExpand macro_expand);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_MODULE).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           module,
                           Location location,
                           LilyPreparserModule module);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_OBJECT).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           object,
                           Location location,
                           LilyPreparserObject object);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_TYPE).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           type,
                           Location location,
                           LilyPreparserType type);

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_USE).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           use,
                           Location location,
                           LilyPreparserUse use);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, constant, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_ERROR).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, error, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, fun, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_INCLUDE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, include, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_LIB).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, lib, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_MACRO_EXPAND).
static VARIANT_DESTRUCTOR(LilyPreparserDecl,
                          macro_expand,
                          LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_MODULE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, module, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, object, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_TYPE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, type, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_USE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, use, LilyPreparserDecl *self);

// Free LilyPreparserDecl type.
static DESTRUCTOR(LilyPreparserDecl, LilyPreparserDecl *self);

// Advance to the one position and update the current.
static inline void
next_token__LilyPreparser(LilyPreparser *self);

// Advance to n position and update the current.
static void
jump__LilyPreparser(LilyPreparser *self, Usize n);

// Peek token at position + n.
static LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n);

static bool
is_start_a_new_block__LilyPreparser(const LilyPreparser *self);

static void
leave_block_finish_by_end__LilyPreparser(LilyPreparser *self);

static void
leave_brace_block__LilyPreparser(LilyPreparser *self);

static void
go_to_next_block__LilyPreparser(LilyPreparser *self);

/// @brief Push all tokens between left and right paren.
/// @note Works only with comma separator
/// @return Vec<Vec<LilyToken* (&)>*>*
static Vec *
preparse_paren_with_comma_sep__LilyPreparser(LilyPreparser *self);

/// @skip Push all tokens between left and right hook.
/// @note Works only with comma separator
/// @return Vec<Vec<LilyToken* (&)>*>*
static Vec *
preparse_hook_with_comma_sep__LilyPreparser(LilyPreparser *self);

/// @skip Push all tokens between left and right brace.
/// @note Works only with comma separator
/// @return Vec<Vec<LilyToken* (&)>*>*
static Vec *
preparse_brace_with_comma_sep__Lilypreparser(LilyPreparser *self);

static LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self);

static LilyPreparserMacro *
preparse_macro__LilyPreparser(LilyPreparser *self);

// NOTE: return 1 for success, 0 for failed.
static int
preparse_package__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info);

static LilyPreparserDecl *
preparse_module_body__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_module__LilyPreparser(LilyPreparser *self);

static void
preparse_test__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `if` block could be closed.
static inline bool
must_close_if_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `else` block could be closed.
static inline bool
must_close_else_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `for` block could be closed.
static inline bool
must_close_for_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `match` block could be closed.
static inline bool
must_close_match_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `while` block could be closed.
static inline bool
must_close_while_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `try` block could be closed.
static inline bool
must_close_try_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `catch` block could be closed.
static inline bool
must_close_catch_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `defer` block could be closed.
static inline bool
must_close_defer_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `match case` could be closed.
static inline bool
must_close_match_case__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `lambda` block could be closed.
static inline bool
must_close_lambda_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the basic block could be closed.
static inline bool
must_close_basic_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the basic brace block could be closed.
static inline bool
must_close_basic_brace_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the unsafe block could be closed.
static inline bool
must_close_unsafe_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `fun` block could be closed.
static inline bool
must_close_fun_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_macro_expand_for_fun__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_asm_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_await_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_break_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_defer_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_drop_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_if_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_for_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_while_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_raise_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_return_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_try_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_match_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_next_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_basic_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_basic_brace_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserFunBodyItem *
preparse_variable_block__LilyPreparser(LilyPreparser *self,
                                       bool is_mut,
                                       bool is_trace,
                                       bool is_ref,
                                       bool is_drop);

static bool
must_preparse_exprs(LilyPreparser *self);

/// @brief Preparse body for function and method.
/// @param must_close Param to add different way to stop the preparser of the
/// body.
/// @return Vec<LilyPreparserFunBodyItem*>*?
Vec *
preparse_body__LilyPreparser(LilyPreparser *self,
                             bool (*must_close)(LilyPreparser *));

static LilyPreparserDecl *
preparse_fun__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_constant__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_constant_multiple__LilyPreparser(LilyPreparser *self);

static LilyPreparserClassBodyItem *
preparse_attribute_for_class__LilyPreparser(LilyPreparser *self,
                                            Location location);

static LilyPreparserTraitBodyItem *
preparse_attribute_for_trait__LilyPreparser(LilyPreparser *self,
                                            Location location);

static LilyPreparserTraitBodyItem *
preparse_macro_expand_for_trait__LilyPreparser(LilyPreparser *self);

static LilyPreparserClassBodyItem *
preparse_macro_expand_for_class__LilyPreparser(LilyPreparser *self);

static LilyPreparserClassBodyItem *
preparse_method_for_class__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *impls,
                              Vec *inherits,
                              Vec *generic_params,
                              bool is_close);

static LilyPreparserTraitBodyItem *
preparse_prototype__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *inherits,
                              Vec *generic_params,
                              bool is_close);

static LilyPreparserRecordObjectBodyItem *
preparse_constant_for_record__LilyPreparser(LilyPreparser *self);

static LilyPreparserRecordObjectBodyItem *
preparse_macro_expand_for_record_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserRecordObjectBodyItem *
preparse_method_for_record__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self,
                                      String *name,
                                      Vec *impls,
                                      Vec *generic_params);

static LilyPreparserEnumObjectBodyItem *
preparse_constant_for_enum__LilyPreparser(LilyPreparser *self);

static LilyPreparserEnumObjectBodyItem *
preparse_macro_expand_for_enum_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserEnumObjectBodyItem *
preparse_method_for_enum__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self,
                                    String *name,
                                    Vec *impls,
                                    Vec *generic_params);

static LilyPreparserDecl *
preparse_object__LilyPreparser(LilyPreparser *self, bool is_close);

static LilyPreparserDecl *
preparse_type__LilyPreparser(LilyPreparser *self);

static LilyPreparserRecordField *
preparse_record_field__LilyPreparser(LilyPreparser *self, bool is_mut);

static LilyPreparserRecordBodyItem *
preparse_macro_expand_for_record__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self,
                               String *name,
                               Vec *generic_params);

static LilyPreparserEnumVariant *
preparse_enum_variant__LilyPreparser(LilyPreparser *self);

static LilyPreparserEnumBodyItem *
preparse_macro_expand_for_enum__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self,
                             String *name,
                             Vec *generic_params);

static LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *generic_params);

static LilyPreparserLibBodyItem *
preparse_lib_constant_prototype__LilyPreparser(LilyPreparser *self);

static LilyPreparserLibBodyItem *
preparse_lib_fun_prototype__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_lib__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_error__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_use__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_include__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_macro_expand__LilyPreparser(LilyPreparser *self);

static void
preparse_preprocess__LilyPreparser(LilyPreparser *self);

static void
preparse_when_condition__LilyPreparser(LilyPreparser *self);

#define PREPARSE_CLOSING(closing, sep)                                         \
    next_token__LilyPreparser(self);                                           \
                                                                               \
    Vec *res = NEW(Vec);                                                       \
                                                                               \
    while (self->current->kind != closing) {                                   \
        Vec *item = NEW(Vec);                                                  \
                                                                               \
        while (self->current->kind != sep && self->current->kind != closing) { \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_L_BRACE:                                  \
                    do {                                                       \
                        push__Vec(item, self->current);                        \
                        next_token__LilyPreparser(self);                       \
                    } while (self->current->kind != LILY_TOKEN_KIND_R_BRACE);  \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_L_HOOK:                                   \
                    do {                                                       \
                        push__Vec(item, self->current);                        \
                        next_token__LilyPreparser(self);                       \
                    } while (self->current->kind != LILY_TOKEN_KIND_R_HOOK);   \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_L_PAREN:                                  \
                    do {                                                       \
                        push__Vec(item, self->current);                        \
                        next_token__LilyPreparser(self);                       \
                    } while (self->current->kind != LILY_TOKEN_KIND_R_PAREN);  \
                                                                               \
                    break;                                                     \
                default:                                                       \
                    break;                                                     \
            }                                                                  \
                                                                               \
            if (self->current->kind != sep &&                                  \
                self->current->kind != closing) {                              \
                push__Vec(item, self->current);                                \
                next_token__LilyPreparser(self);                               \
            }                                                                  \
        }                                                                      \
                                                                               \
        push__Vec(res, item);                                                  \
                                                                               \
        if (self->current->kind != closing) {                                  \
            next_token__LilyPreparser(self);                                   \
        }                                                                      \
    }                                                                          \
                                                                               \
    next_token__LilyPreparser(self);                                           \
                                                                               \
    return res;

#define GET_NAME(self, detail)                                                 \
    String *name = NULL;                                                       \
                                                                               \
    switch (self->current->kind) {                                             \
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:                                \
            name = clone__String(self->current->identifier_normal);            \
            next_token__LilyPreparser(self);                                   \
            break;                                                             \
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:                                \
            name = clone__String(self->current->identifier_string);            \
            next_token__LilyPreparser(self);                                   \
            break;                                                             \
        default:                                                               \
            emit__Diagnostic(                                                  \
              NEW_VARIANT(Diagnostic,                                          \
                          simple_lily_error,                                   \
                          self->file,                                          \
                          &self->current->location,                            \
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER), \
                          NULL,                                                \
                          NULL,                                                \
                          detail),                                             \
              &self->count_error);                                             \
            name = from__String("__error__");                                  \
    }

#define PUSH_TOKEN(v, token) \
    push__Vec(v, token);     \
    next_token__LilyPreparser(self)

#define PREPARSE_UNTIL(v, until)                  \
    {                                             \
        int count = 0;                            \
                                                  \
        while (until ? 1 : count > 0) {           \
            switch (self->current->kind) {        \
                case LILY_TOKEN_KIND_L_BRACE:     \
                case LILY_TOKEN_KIND_L_HOOK:      \
                case LILY_TOKEN_KIND_L_PAREN:     \
                    PUSH_TOKEN(v, self->current); \
                    ++count;                      \
                                                  \
                    break;                        \
                case LILY_TOKEN_KIND_R_BRACE:     \
                case LILY_TOKEN_KIND_R_HOOK:      \
                case LILY_TOKEN_KIND_R_PAREN:     \
                    PUSH_TOKEN(v, self->current); \
                    --count;                      \
                                                  \
                    break;                        \
                default:                          \
                    PUSH_TOKEN(v, self->current); \
            }                                     \
        }                                         \
    }

static threadlocal enum LilyVisibility visibility_decl =
  LILY_VISIBILITY_PRIVATE;
static threadlocal Location location_decl;
static threadlocal Location location_fun_body_item;
static threadlocal bool destroy_all = false;

CONSTRUCTOR(LilyPreparserImport *,
            LilyPreparserImport,
            String *value,
            String *as,
            Location location)
{
    LilyPreparserImport *self = lily_malloc(sizeof(LilyPreparserImport));

    self->value = value;
    self->as = as;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyPreparserImport, const LilyPreparserImport *self)
{
    if (self->as) {
        return format(
          "LilyPreparserImport{{ value = {S}, as = {S}, location = {sa} }",
          self->value,
          self->as,
          to_string__Debug__Location(&self->location));
    }

    return format(
      "LilyPreparserImport{{ value = {S}, as = NULL, location = {sa} }",
      self->value,
      to_string__Debug__Location(&self->location));
}

void
IMPL_FOR_DEBUG(debug, LilyPreparserImport, const LilyPreparserImport *self)
{
    PRINTLN("{sa}", to_string__Debug__LilyPreparserImport(self));
}
#endif

DESTRUCTOR(LilyPreparserImport, LilyPreparserImport *self)
{
    FREE(String, self->value);

    if (self->as) {
        FREE(String, self->as);
    }

    lily_free(self);
}

CONSTRUCTOR(LilyPreparserMacro *,
            LilyPreparserMacro,
            String *name,
            Vec *params,
            Vec *tokens,
            Location location)
{
    LilyPreparserMacro *self = lily_malloc(sizeof(LilyPreparserMacro));

    self->name = name;
    self->params = params;
    self->tokens = tokens;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMacro, const LilyPreparserMacro *self)
{
    String *res =
      format__String("LilyPreparserMacro{{ name = {S}, params =", self->name);

    if (self->params) {
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", tokens =");
    DEBUG_VEC_STR(self->tokens, res, LilyToken);

    {
        char *s = format(", location = {sa} }",
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyPreparserMacro, const LilyPreparserMacro *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyPreparserMacro(self));
}
#endif

DESTRUCTOR(LilyPreparserMacro, LilyPreparserMacro *self)
{
    FREE(String, self->name);

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }

    FREE(LilyToken, pop__Vec(self->tokens));
    FREE(Vec, self->tokens);

    lily_free(self);
}

CONSTRUCTOR(LilyPreparserSubPackage *,
            LilyPreparserSubPackage,
            enum LilyVisibility visibility,
            String *name,
            String *global_name)
{
    LilyPreparserSubPackage *self =
      lily_malloc(sizeof(LilyPreparserSubPackage));

    self->name = name;
    self->global_name = global_name;
    self->visibility = visibility;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserSubPackage,
               const LilyPreparserSubPackage *self)
{
    switch (self->visibility) {
        case LILY_VISIBILITY_PRIVATE:
            return format("LilyPreparserSubPackage{{ name = {S}, global_name = "
                          "{S}, visibility = "
                          "LILY_VISIBILITY_PRIVATE }",
                          self->name,
                          self->global_name);
        case LILY_VISIBILITY_PUBLIC:
            return format("LilyPreparserSubPackage{{ name = {S}, global_name = "
                          "{S}, visibility = "
                          "LILY_VISIBILITY_PUBLIC }",
                          self->name,
                          self->global_name);
        default:
            UNREACHABLE("static visibility is not expected in this case");
    }
}

void
IMPL_FOR_DEBUG(debug,
               LilyPreparserSubPackage,
               const LilyPreparserSubPackage *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyPreparserSubPackage(self));
}
#endif

DESTRUCTOR(LilyPreparserSubPackage, LilyPreparserSubPackage *self)
{
    FREE(String, self->name);
    FREE(String, self->global_name);

    lily_free(self);
}

CONSTRUCTOR(LilyPreparserPackage *, LilyPreparserPackage, String *name)
{
    LilyPreparserPackage *self = lily_malloc(sizeof(LilyPreparserPackage));

    self->name = name;
    self->sub_packages = NEW(Vec);

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserPackage,
               const LilyPreparserPackage *self)
{
    String *res = NEW(String);

    if (self->name) {
        push_str__String(res, "LilyPreparserPackage{ name = ");
        push_str__String(res, self->name->buffer);
        push_str__String(res, ", sub_packages =");
    } else {
        push_str__String(res,
                         "LilyPreparserPackage{ name = NULL, sub_packages =");
    }

    DEBUG_VEC_STR(self->sub_packages, res, LilyPreparserSubPackage);

    push_str__String(res, " }");

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyPreparserPackage, const LilyPreparserPackage *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyPreparserPackage(self));
}
#endif

DESTRUCTOR(LilyPreparserPackage, LilyPreparserPackage *self)
{
    FREE_BUFFER_ITEMS(self->sub_packages->buffer,
                      self->sub_packages->len,
                      LilyPreparserSubPackage);
    FREE(Vec, self->sub_packages);
    lily_free(self);
}

CONSTRUCTOR(LilyPreparserModule,
            LilyPreparserModule,
            String *name,
            Vec *body,
            enum LilyVisibility visibility)
{
    return (LilyPreparserModule){ .name = name,
                                  .body = body,
                                  .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserModule, const LilyPreparserModule *self)
{
    String *res =
      format__String("LilyPreparserModule{{ name = {S}, body =", self->name);

    DEBUG_VEC_STRING(self->body, res, LilyPreparserDecl);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserModule, const LilyPreparserModule *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyPreparserDecl);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserTest, LilyPreparserTest, String *name, Vec *body)
{
    return (LilyPreparserTest){
        .name = name,
        .body = body,
    };
}

DESTRUCTOR(LilyPreparserTest, const LilyPreparserTest *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserFunBodyItemExprs,
            LilyPreparserFunBodyItemExprs,
            Vec *tokens)
{
    return (LilyPreparserFunBodyItemExprs){ .tokens = tokens };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemExprs,
               const LilyPreparserFunBodyItemExprs *self)
{
    String *res = from__String("LilyPreparserFunBodyItemExprs{ tokens =");

    DEBUG_VEC_STR(self->tokens, res, LilyToken);

    push_str__String(res, "}");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemExprs,
           const LilyPreparserFunBodyItemExprs *self)
{
    FREE(Vec, self->tokens);
}

CONSTRUCTOR(LilyPreparserFunBodyItemLambda,
            LilyPreparserFunBodyItemLambda,
            String *name,
            Vec *params,
            Vec *return_data_type,
            LilyPreparserFunBodyItem *item,
            Vec *params_call)
{
    return (LilyPreparserFunBodyItemLambda){ .name = name,
                                             .params = params,
                                             .return_data_type =
                                               return_data_type,
                                             .item = item,
                                             .params_call = params_call };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemLambda,
               const LilyPreparserFunBodyItemLambda *self)
{
    String *res = NULL;

    if (self->name) {
        res = format__String(
          "LilyPreparserFunBodyItemLambda{{ name = {S}, params =", self->name);
    } else {
        res =
          from__String("LilyPreparserFunBodyItemLambda{ name = NULL, params =");
    }

    if (self->params) {
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_data_type =");

    if (self->return_data_type) {
        DEBUG_VEC_STR(self->return_data_type, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", item = ");

    {
        String *s = to_string__Debug__LilyPreparserFunBodyItem(self->item);

        APPEND_AND_FREE(res, s);
    }

    push_str__String(res, ", params_call =");

    if (self->params_call) {
        DEBUG_VEC_STR_2(self->params_call, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemLambda,
           const LilyPreparserFunBodyItemLambda *self)
{
    if (self->name && destroy_all) {
        FREE(String, self->name);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(Vec, self->return_data_type);
    }

    FREE(LilyPreparserFunBodyItem, self->item);

    if (self->params_call) {
        FREE_BUFFER_ITEMS(
          self->params_call->buffer, self->params_call->len, Vec);
        FREE(Vec, self->params_call);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtAsm,
            LilyPreparserFunBodyItemStmtAsm,
            Vec *params)
{
    return (LilyPreparserFunBodyItemStmtAsm){ .params = params };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtAsm,
               const LilyPreparserFunBodyItemStmtAsm *self)
{
    String *res = from__String("LilyPreparserFunBodyStmtAsm{ params =");

    DEBUG_VEC_STR_2(self->params, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtAsm,
           const LilyPreparserFunBodyItemStmtAsm *self)
{
    FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
    FREE(Vec, self->params);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtAwait,
            LilyPreparserFunBodyItemStmtAwait,
            Vec *expr)
{
    return (LilyPreparserFunBodyItemStmtAwait){ .expr = expr };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtAwait,
               const LilyPreparserFunBodyItemStmtAwait *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtAwait{{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtAwait,
           const LilyPreparserFunBodyItemStmtAwait *self)
{
    FREE(Vec, self->expr);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
            LilyPreparserFunBodyItemStmtBlock,
            Vec *block)
{
    return (LilyPreparserFunBodyItemStmtBlock){ .block = block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtBlock,
               const LilyPreparserFunBodyItemStmtBlock *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtBlock{ block =");

    DEBUG_VEC_STRING(self->block, res, LilyPreparserFunBodyItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
           const LilyPreparserFunBodyItemStmtBlock *self)
{
    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtBreak,
            LilyPreparserFunBodyItemStmtBreak,
            String *name)
{
    return (LilyPreparserFunBodyItemStmtBreak){ .name = name };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtBreak,
               const LilyPreparserFunBodyItemStmtBreak *self)
{
    if (self->name) {
        return format__String(
          "LilyPreparserFunBodyItemStmtBreak{{ name = {S} }", self->name);
    } else {
        return from__String(
          "LilyPreparserFunBodyItemStmtBreak{{ name = NULL }");
    }
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtBreak,
           const LilyPreparserFunBodyItemStmtBreak *self)
{
    if (self->name && destroy_all) {
        FREE(String, self->name);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtDefer,
            LilyPreparserFunBodyItemStmtDefer,
            LilyPreparserFunBodyItem *item)
{
    return (LilyPreparserFunBodyItemStmtDefer){ .item = item };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtDefer,
               const LilyPreparserFunBodyItemStmtDefer *self)
{
    return format__String(
      "LilyPreparserFunBodyItemStmtDefer{{ item = {Sr} }",
      to_string__Debug__LilyPreparserFunBodyItem(self->item));
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtDefer,
           const LilyPreparserFunBodyItemStmtDefer *self)
{
    FREE(LilyPreparserFunBodyItem, self->item);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtDrop,
            LilyPreparserFunBodyItemStmtDrop,
            Vec *expr)
{
    return (LilyPreparserFunBodyItemStmtDrop){ .expr = expr };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtDrop,
               const LilyPreparserFunBodyItemStmtDrop *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtDrop{{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtDrop,
           const LilyPreparserFunBodyItemStmtDrop *self)
{
    FREE(Vec, self->expr);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtFor,
            LilyPreparserFunBodyItemStmtFor,
            String *name,
            Vec *expr,
            Vec *capture,
            Vec *block)
{
    return (LilyPreparserFunBodyItemStmtFor){
        .name = name, .expr = expr, .capture = capture, .block = block
    };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtFor,
               const LilyPreparserFunBodyItemStmtFor *self)
{
    String *res = NULL;

    if (self->name) {
        res = format__String("LilyPreparserFunBodyItemStmtFor{{ name = {S}",
                             self->name);
    } else {
        res = from__String("LilyPreparserFunBodyItemStmtFor{ name = NULL");
    }

    push_str__String(res, ", expr =");
    DEBUG_VEC_STR(self->expr, res, LilyToken);
    push_str__String(res, ", block =");

    DEBUG_VEC_STR(self->block, res, LilyToken);

    push_str__String(res, ", capture =");

    if (self->capture) {
        DEBUG_VEC_STR(self->capture, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtFor,
           const LilyPreparserFunBodyItemStmtFor *self)
{
    if (self->name && destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->expr);

    if (self->capture) {
        FREE(Vec, self->capture);
    }

    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtIf,
            LilyPreparserFunBodyItemStmtIf,
            Vec *if_expr,
            Vec *if_capture,
            Vec *if_block,
            Vec *elif_exprs,
            Vec *elif_captures,
            Vec *elif_blocks,
            Vec *else_block)
{
    return (LilyPreparserFunBodyItemStmtIf){ .if_expr = if_expr,
                                             .if_capture = if_capture,
                                             .if_block = if_block,
                                             .elif_exprs = elif_exprs,
                                             .elif_captures = elif_captures,
                                             .elif_blocks = elif_blocks,
                                             .else_block = else_block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtIf,
               const LilyPreparserFunBodyItemStmtIf *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtIf{ if_expr =");

    DEBUG_VEC_STR(self->if_expr, res, LilyToken);
    push_str__String(res, ", if_block =");

    DEBUG_VEC_STR(self->if_block, res, LilyToken);

    push_str__String(res, ", if_capture =");

    if (self->if_capture) {
        DEBUG_VEC_STR(self->if_capture, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    if (self->elif_exprs) {
        push_str__String(res, ", elif_exprs =");
        DEBUG_VEC_STR_2(self->elif_exprs, res, LilyToken);

        if (!self->elif_blocks) {
            UNREACHABLE("self->elif_blocks cannot be NULL when "
                        "self->elif_exprs is not NULL.");
        }

        push_str__String(res, ", elif_blocks =");
        DEBUG_VEC_STRING_2(self->elif_blocks, res, LilyPreparserFunBodyItem);
    }

    push_str__String(res, ", elif_captures =");

    if (self->elif_captures) {
        push_str__String(res, "{");

        for (Usize i = 0; i < self->elif_captures->len; ++i) {
            Vec *elif_capture =
              get__Vec(self->elif_captures, i); // Vec<LilyToken* (&)>* (&)

            if (elif_capture) {
                DEBUG_VEC_STR(elif_capture, res, LilyToken);
            } else {
                push_str__String(res, "NULL");
            }

            if (i + 1 != self->elif_captures->len) {
                push_str__String(res, ", ");
                continue;
            }

            push_str__String(res, " }");
        }
    } else {
        push_str__String(res, " NULL");
    }

    if (self->else_block) {
        push_str__String(res, ", else_block =");
        DEBUG_VEC_STRING(self->else_block, res, LilyPreparserFunBodyItem);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtIf,
           const LilyPreparserFunBodyItemStmtIf *self)
{
    FREE(Vec, self->if_expr);

    if (self->if_capture) {
        FREE(Vec, self->if_capture);
    }

    FREE_BUFFER_ITEMS(
      self->if_block->buffer, self->if_block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->if_block);

    if (self->elif_exprs) {
        FREE_BUFFER_ITEMS(self->elif_exprs->buffer, self->elif_exprs->len, Vec);
        FREE(Vec, self->elif_exprs);
    }

    if (self->elif_captures) {
        for (Usize i = 0; i < self->elif_captures->len; ++i) {
            Vec *elif_capture = get__Vec(self->elif_captures, i);

            if (elif_capture) {
                FREE(Vec, elif_capture);
            }
        }

        FREE(Vec, self->elif_captures);
    }

    if (self->elif_blocks) {
        FREE_BUFFER_ITEMS_2(self->elif_blocks->buffer,
                            self->elif_blocks->len,
                            LilyPreparserFunBodyItem);
        FREE(Vec, self->elif_blocks);
    }

    if (self->else_block) {
        FREE_BUFFER_ITEMS(self->else_block->buffer,
                          self->else_block->len,
                          LilyPreparserFunBodyItem);
        FREE(Vec, self->else_block);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtMatch,
            LilyPreparserFunBodyItemStmtMatch,
            Vec *expr,
            Vec *patterns,
            Vec *pattern_conds,
            Vec *blocks)
{
    return (LilyPreparserFunBodyItemStmtMatch){ .expr = expr,
                                                .patterns = patterns,
                                                .pattern_conds = pattern_conds,
                                                .blocks = blocks };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtMatch,
               const LilyPreparserFunBodyItemStmtMatch *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtMatch{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);

    push_str__String(res, ", patterns =");
    DEBUG_VEC_STR_2(self->patterns, res, LilyToken);

    push_str__String(res, ", pattern_conds=");

    for (Usize i = 0; i < self->pattern_conds->len; ++i) {
        String *item = NEW(String);
        Vec *pattern_cond = get__Vec(self->pattern_conds, i);

        if (pattern_cond) {
            DEBUG_VEC_STR(pattern_cond, item, LilyToken);
        } else {
            push_str__String(item, "NULL");
        }

        APPEND_AND_FREE(res, item);

        if (i != self->pattern_conds->len - 1) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, ", blocks=");
    DEBUG_VEC_STRING(self->blocks, res, LilyPreparserFunBodyItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtMatch,
           const LilyPreparserFunBodyItemStmtMatch *self)
{
    FREE(Vec, self->expr);

    FREE_BUFFER_ITEMS(self->patterns->buffer, self->patterns->len, Vec);
    FREE(Vec, self->patterns);

    for (Usize i = 0; i < self->pattern_conds->len; ++i) {
        Vec *item = get__Vec(self->pattern_conds, i);

        if (item) {
            FREE(Vec, item);
        }
    }

    FREE(Vec, self->pattern_conds);

    FREE_BUFFER_ITEMS(
      self->blocks->buffer, self->blocks->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->blocks);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtNext,
            LilyPreparserFunBodyItemStmtNext,
            String *name)
{
    return (LilyPreparserFunBodyItemStmtNext){ .name = name };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtNext,
               const LilyPreparserFunBodyItemStmtNext *self)
{
    if (self->name) {
        return format__String("LilyPreparserFunBodyItemStmtNext{{ name = {S} }",
                              self->name);
    }

    return from__String("LilyPreparserFunBodyItemStmtNext{ name = NULL }");
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtNext,
           const LilyPreparserFunBodyItemStmtNext *self)
{
    if (self->name && destroy_all) {
        FREE(String, self->name);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtRaise,
            LilyPreparserFunBodyItemStmtRaise,
            Vec *expr)
{
    return (LilyPreparserFunBodyItemStmtRaise){ .expr = expr };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtRaise,
               const LilyPreparserFunBodyItemStmtRaise *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtRaise{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtRaise,
           const LilyPreparserFunBodyItemStmtRaise *self)
{
    FREE(Vec, self->expr);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtReturn,
            LilyPreparserFunBodyItemStmtReturn,
            Vec *expr)
{
    return (LilyPreparserFunBodyItemStmtReturn){ .expr = expr };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtReturn,
               const LilyPreparserFunBodyItemStmtReturn *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtReturn{ expr =");

    if (self->expr) {
        DEBUG_VEC_STR(self->expr, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtReturn,
           const LilyPreparserFunBodyItemStmtReturn *self)
{
    if (self->expr) {
        FREE(Vec, self->expr);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtTry,
            LilyPreparserFunBodyItemStmtTry,
            Vec *block,
            Vec *catch_expr,
            Vec *catch_block)
{
    return (LilyPreparserFunBodyItemStmtTry){ .block = block,
                                              .catch_expr = catch_expr,
                                              .catch_block = catch_block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtTry,
               const LilyPreparserFunBodyItemStmtTry *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtTry{ block =");

    DEBUG_VEC_STRING(self->block, res, LilyPreparserFunBodyItem);

    if (self->catch_expr) {
        push_str__String(res, ", catch_expr =");
        DEBUG_VEC_STR(self->catch_expr, res, LilyToken);

        if (!self->catch_block) {
            UNREACHABLE("self->catch_block cannot be NULL when "
                        "self->catch_expr is not NULL");
        }

        push_str__String(res, ", catch_block =");
        DEBUG_VEC_STRING(self->catch_block, res, LilyPreparserFunBodyItem);
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtTry,
           const LilyPreparserFunBodyItemStmtTry *self)
{
    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);

    if (self->catch_block) {
        if (self->catch_expr) {
            FREE(Vec, self->catch_expr);
        }

        FREE_BUFFER_ITEMS(self->catch_block->buffer,
                          self->catch_block->len,
                          LilyPreparserFunBodyItem);
        FREE(Vec, self->catch_block);
    }
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtUnsafe,
            LilyPreparserFunBodyItemStmtUnsafe,
            Vec *block)
{
    return (LilyPreparserFunBodyItemStmtUnsafe){ .block = block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtUnsafe,
               const LilyPreparserFunBodyItemStmtUnsafe *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtUnsafe{ block =");

    DEBUG_VEC_STRING(self->block, res, LilyPreparserFunBodyItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtUnsafe,
           const LilyPreparserFunBodyItemStmtUnsafe *self)
{
    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtVariable,
            LilyPreparserFunBodyItemStmtVariable,
            String *name,
            Vec *data_type,
            Vec *expr,
            bool is_mut,
            bool is_trace,
            bool is_ref,
            bool is_drop)
{
    return (LilyPreparserFunBodyItemStmtVariable){ .name = name,
                                                   .data_type = data_type,
                                                   .expr = expr,
                                                   .is_mut = is_mut,
                                                   .is_trace = is_trace,
                                                   .is_ref = is_ref,
                                                   .is_drop = is_drop };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtVariable,
               const LilyPreparserFunBodyItemStmtVariable *self)
{
    String *res = format__String(
      "LilyPreparserFunBodyItemStmtVariable{{ name = {S}", self->name);

    if (self->data_type) {
        push_str__String(res, ", data_type =");
        DEBUG_VEC_STR(self->data_type, res, LilyToken);
    }

    push_str__String(res, ", expr =");
    DEBUG_VEC_STR(self->expr, res, LilyToken);

    {
        char *s = format(
          ", is_mut = {b}, is_trace = {b}, is_ref = {b}, is_drop = {b} }",
          self->is_mut,
          self->is_trace,
          self->is_ref,
          self->is_drop);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtVariable,
           const LilyPreparserFunBodyItemStmtVariable *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->data_type) {
        FREE(Vec, self->data_type);
    }

    FREE(Vec, self->expr);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtWhile,
            LilyPreparserFunBodyItemStmtWhile,
            Vec *expr,
            Vec *block)
{
    return (LilyPreparserFunBodyItemStmtWhile){ .expr = expr, .block = block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtWhile,
               const LilyPreparserFunBodyItemStmtWhile *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtWhile{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);

    push_str__String(res, ", block =");
    DEBUG_VEC_STRING(self->block, res, LilyPreparserFunBodyItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtWhile,
           const LilyPreparserFunBodyItemStmtWhile *self)
{
    FREE(Vec, self->expr);
    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemKind,
               enum LilyPreparserFunBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    exprs,
                    LilyPreparserFunBodyItemExprs exprs,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS;
    self->exprs = exprs;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    lambda,
                    LilyPreparserFunBodyItemLambda lambda,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA;
    self->lambda = lambda;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_asm,
                    LilyPreparserFunBodyItemStmtAsm stmt_asm,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM;
    self->stmt_asm = stmt_asm;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    macro_expand,
                    LilyPreparserMacroExpand macro_expand,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND;
    self->macro_expand = macro_expand;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_await,
                    LilyPreparserFunBodyItemStmtAwait stmt_await,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT;
    self->stmt_await = stmt_await;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_block,
                    LilyPreparserFunBodyItemStmtBlock stmt_block,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK;
    self->stmt_block = stmt_block;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_break,
                    LilyPreparserFunBodyItemStmtBreak stmt_break,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK;
    self->stmt_break = stmt_break;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_defer,
                    LilyPreparserFunBodyItemStmtDefer stmt_defer,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER;
    self->stmt_defer = stmt_defer;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_drop,
                    LilyPreparserFunBodyItemStmtDrop stmt_drop,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP;
    self->stmt_drop = stmt_drop;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_for,
                    LilyPreparserFunBodyItemStmtFor stmt_for,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR;
    self->stmt_for = stmt_for;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_if,
                    LilyPreparserFunBodyItemStmtIf stmt_if,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF;
    self->stmt_if = stmt_if;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_match,
                    LilyPreparserFunBodyItemStmtMatch stmt_match,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH;
    self->stmt_match = stmt_match;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_next,
                    LilyPreparserFunBodyItemStmtNext stmt_next,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT;
    self->stmt_next = stmt_next;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_raise,
                    LilyPreparserFunBodyItemStmtRaise stmt_raise,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE;
    self->stmt_raise = stmt_raise;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_return,
                    LilyPreparserFunBodyItemStmtReturn stmt_return,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN;
    self->stmt_return = stmt_return;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_try,
                    LilyPreparserFunBodyItemStmtTry stmt_try,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY;
    self->stmt_try = stmt_try;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_unsafe,
                    LilyPreparserFunBodyItemStmtUnsafe stmt_unsafe,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE;
    self->stmt_unsafe = stmt_unsafe;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_var,
                    LilyPreparserFunBodyItemStmtVariable stmt_var,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE;
    self->stmt_var = stmt_var;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                    LilyPreparserFunBodyItem,
                    stmt_while,
                    LilyPreparserFunBodyItemStmtWhile stmt_while,
                    Location location)
{
    LilyPreparserFunBodyItem *self =
      lily_malloc(sizeof(LilyPreparserFunBodyItem));

    self->kind = LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE;
    self->stmt_while = stmt_while;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItem,
               const LilyPreparserFunBodyItem *self)
{
    String *res =
      format__String("LilyPreparserFunBodyItem{{ kind = {s}, location = {sa}",
                     to_string__Debug__LilyPreparserFunBodyItemKind(self->kind),
                     to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS: {
            String *s =
              to_string__Debug__LilyPreparserFunBodyItemExprs(&self->exprs);

            push_str__String(res, ", exprs = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA: {
            String *s =
              to_string__Debug__LilyPreparserFunBodyItemLambda(&self->lambda);

            push_str__String(res, ", lambda =");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND: {
            String *s =
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand);

            push_str__String(res, ", macro_expand =");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtAsm(
              &self->stmt_asm);

            push_str__String(res, ", stmt_asm = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtAwait(
              &self->stmt_await);

            push_str__String(res, ", stmt_await = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtBlock(
              &self->stmt_block);

            push_str__String(res, ", stmt_block = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtBreak(
              &self->stmt_break);

            push_str__String(res, ", stmt_break = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtDefer(
              &self->stmt_defer);

            push_str__String(res, ", stmt_defer = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtDrop(
              &self->stmt_drop);

            push_str__String(res, ", stmt_drop = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtFor(
              &self->stmt_for);

            push_str__String(res, ", stmt_for = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF: {
            String *s =
              to_string__Debug__LilyPreparserFunBodyItemStmtIf(&self->stmt_if);

            push_str__String(res, ", stmt_if = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtMatch(
              &self->stmt_match);

            push_str__String(res, ", stmt_match = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtNext(
              &self->stmt_next);

            push_str__String(res, ", stmt_next = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtRaise(
              &self->stmt_raise);

            push_str__String(res, ", stmt_raise = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtReturn(
              &self->stmt_return);

            push_str__String(res, ", stmt_return = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtTry(
              &self->stmt_try);

            push_str__String(res, ", stmt_try = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtUnsafe(
              &self->stmt_unsafe);

            push_str__String(res, ", stmt_unsafe = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtVariable(
              &self->stmt_var);

            push_str__String(res, ", stmt_var = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtWhile(
              &self->stmt_while);

            push_str__String(res, ", stmt_while = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    push_str__String(res, " }");

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   exprs,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemExprs, &self->exprs);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   lambda,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemLambda, &self->lambda);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   macro_expand,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_asm,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtAsm, &self->stmt_asm);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_await,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtAwait, &self->stmt_await);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_block,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtBlock, &self->stmt_block);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_break,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtBreak, &self->stmt_break);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_defer,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtDefer, &self->stmt_defer);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_drop,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtDrop, &self->stmt_drop);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_for,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtFor, &self->stmt_for);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_if,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtIf, &self->stmt_if);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_match,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtMatch, &self->stmt_match);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_next,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtNext, &self->stmt_next);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_raise,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtRaise, &self->stmt_raise);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_return,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtReturn, &self->stmt_return);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_try,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtTry, &self->stmt_try);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_unsafe,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtUnsafe, &self->stmt_unsafe);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_var,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtVariable, &self->stmt_var);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                   stmt_while,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtWhile, &self->stmt_while);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserFunBodyItem, LilyPreparserFunBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS:
            FREE_VARIANT(LilyPreparserFunBodyItem, exprs, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA:
            FREE_VARIANT(LilyPreparserFunBodyItem, lambda, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserFunBodyItem, macro_expand, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_ASM:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_asm, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_AWAIT:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_await, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_block, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BREAK:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_break, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DEFER:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_defer, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_DROP:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_drop, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_for, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_if, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_match, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_NEXT:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_next, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RAISE:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_raise, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_RETURN:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_return, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_try, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_unsafe, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_VARIABLE:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_var, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_while, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserFun,
            LilyPreparserFun,
            String *name,
            String *object_impl,
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
                               .object_impl = object_impl,
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserFun, const LilyPreparserFun *self)
{
    String *res = format__String("LilyPreparserFun{{ name = {S}", self->name);

    if (self->object_impl) {
        char *s = format(", object_impl = {S}", self->object_impl);

        PUSH_STR_AND_FREE(res, s);
    } else {
        push_str__String(res, ", object_impl = NULL");
    }

    if (self->generic_params) {
        push_str__String(res, ", generic_params=");
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, ", generic_params = NULL");
    }

    if (self->params) {
        push_str__String(res, ", params=");
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, ", params = NULL");
    }

    push_str__String(res, ", return_data_type =");

    if (self->return_data_type) {
        DEBUG_VEC_STR(self->return_data_type, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserFunBodyItem);

    push_str__String(res, ", req =");

    if (self->req) {
        DEBUG_VEC_STR_2(self->req, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", when =");

    if (self->when) {
        DEBUG_VEC_STR_2(self->when, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", visibility = {s}, is_async = {b}, is_operator = "
                         "{b}, req_is_comptime = {b}, when_is_comptime = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_async,
                         self->is_operator,
                         self->req_is_comptime,
                         self->when_is_comptime);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFun, const LilyPreparserFun *self)
{
    if (destroy_all) {
        FREE(String, self->name);

        if (self->object_impl) {
            FREE(String, self->object_impl);
        }
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec)
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(Vec, self->return_data_type);
    }

    if (self->req) {
        FREE(Vec, self->req);
    }

    if (self->when) {
        FREE(Vec, self->when);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserConstantInfo *,
            LilyPreparserConstantInfo,
            String *name,
            Vec *expr,
            Vec *data_type,
            enum LilyVisibility visibility)
{
    LilyPreparserConstantInfo *self =
      lily_malloc(sizeof(LilyPreparserConstantInfo));

    self->name = name;
    self->expr = expr;
    self->data_type = data_type;
    self->visibility = visibility;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstantInfo,
               const LilyPreparserConstantInfo *self)
{
    String *res = format__String(
      "LilyPreparserConstantInfo{{ name = {S}, expr =", self->name);

    DEBUG_VEC_STR(self->expr, res, LilyToken);

    push_str__String(res, ", data_type =");
    DEBUG_VEC_STR(self->data_type, res, LilyToken);

    push_str__String(res, ", visibility = ");
    push_str__String(res, to_string__Debug__LilyVisibility(self->visibility));
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserConstantInfo, LilyPreparserConstantInfo *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->expr);
    FREE(Vec, self->data_type);
    lily_free(self);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstantKind,
               enum LilyPreparserConstantKind self)
{
    switch (self) {
        case LILY_PREPARSER_CONSTANT_KIND_SIMPLE:
            return "LILY_PREPARSER_CONSTANT_KIND_SIMPLE";
        case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
            return "LILY_PREPARSER_CONSTANT_KIND_MULTIPLE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserConstant,
                    LilyPreparserConstant,
                    simple,
                    LilyPreparserConstantInfo *simple)
{
    return (LilyPreparserConstant){ .kind = LILY_PREPARSER_CONSTANT_KIND_SIMPLE,
                                    .simple = simple };
}

VARIANT_CONSTRUCTOR(LilyPreparserConstant,
                    LilyPreparserConstant,
                    multiple,
                    Vec *multiple)
{
    return (LilyPreparserConstant){ .kind =
                                      LILY_PREPARSER_CONSTANT_KIND_MULTIPLE,
                                    .multiple = multiple };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserConstant,
               const LilyPreparserConstant *self)
{
    String *res =
      format__String("LilyPreparserConstant{{ kind = {s}",
                     to_string__Debug__LilyPreparserConstantKind(self->kind));

    switch (self->kind) {
        case LILY_PREPARSER_CONSTANT_KIND_SIMPLE: {
            char *s =
              format(", simple = {Sr} }",
                     to_string__Debug__LilyPreparserConstantInfo(self->simple));
            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
            DEBUG_VEC_STRING(self->multiple, res, LilyPreparserConstantInfo);
            push_str__String(res, " }");

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserConstant,
                   simple,
                   const LilyPreparserConstant *self)
{
    FREE(LilyPreparserConstantInfo, self->simple);
}

VARIANT_DESTRUCTOR(LilyPreparserConstant,
                   multiple,
                   const LilyPreparserConstant *self)
{
    FREE_BUFFER_ITEMS(
      self->multiple->buffer, self->multiple->len, LilyPreparserConstantInfo);
    FREE(Vec, self->multiple);
}

DESTRUCTOR(LilyPreparserConstant, const LilyPreparserConstant *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
            FREE_VARIANT(LilyPreparserConstant, multiple, self);
            break;
        case LILY_PREPARSER_CONSTANT_KIND_SIMPLE:
            FREE_VARIANT(LilyPreparserConstant, simple, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserClassBodyItemKind,
               enum LilyPreparserClassBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE:
            return "LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE";
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD:
            return "LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMethod, const LilyPreparserMethod *self)
{
    return to_string__Debug__LilyPreparserFun(self);
}
#endif

CONSTRUCTOR(LilyPreparserAttribute,
            LilyPreparserAttribute,
            String *name,
            Vec *data_type,
            Vec *default_expr,
            bool is_get,
            bool is_set,
            enum LilyVisibility visibility)
{
    return (LilyPreparserAttribute){ .name = name,
                                     .data_type = data_type,
                                     .default_expr = default_expr,
                                     .is_get = is_get,
                                     .is_set = is_set,
                                     .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserAttribute,
               const LilyPreparserAttribute *self)
{
    String *res = format__String(
      "LilyPreparserAttribute{{ name = {S}, data_type =", self->name);

    DEBUG_VEC_STR(self->data_type, res, LilyToken);

    if (self->default_expr) {
        push_str__String(res, ", default_expr =");
        DEBUG_VEC_STR(self->default_expr, res, LilyToken);
    }

    {
        char *s = format(", visibility = {s}, is_get = {b}, is_set = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_get,
                         self->is_set);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserAttribute, const LilyPreparserAttribute *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->data_type);

    if (self->default_expr) {
        FREE(Vec, self->default_expr);
    }
}

VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                    LilyPreparserClassBodyItem,
                    attribute,
                    LilyPreparserAttribute attribute,
                    Location location)
{
    LilyPreparserClassBodyItem *self =
      lily_malloc(sizeof(LilyPreparserClassBodyItem));

    self->kind = LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

static VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                           LilyPreparserClassBodyItem,
                           macro_expand,
                           LilyPreparserMacroExpand macro_expand,
                           Location location)
{
    LilyPreparserClassBodyItem *self =
      lily_malloc(sizeof(LilyPreparserClassBodyItem));

    self->kind = LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserClassBodyItem *,
                    LilyPreparserClassBodyItem,
                    method,
                    LilyPreparserMethod method,
                    Location location)
{
    LilyPreparserClassBodyItem *self =
      lily_malloc(sizeof(LilyPreparserClassBodyItem));

    self->kind = LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserClassBodyItem,
               const LilyPreparserClassBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserClassBodyItem{{ kind = {s}, location = {sa}",
      to_string__Debug__LilyPreparserClassBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE: {
            String *s =
              to_string__Debug__LilyPreparserAttribute(&self->attribute);

            push_str__String(res, ", attribute = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND: {
            String *s =
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand);

            push_str__String(res, ", macro_expand = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD: {
            String *s = to_string__Debug__LilyPreparserMethod(&self->method);

            push_str__String(res, ", method = ");
            APPEND_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    push_str__String(res, " }");

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                   attribute,
                   LilyPreparserClassBodyItem *self)
{
    FREE(LilyPreparserAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                   macro_expand,
                   LilyPreparserClassBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                   method,
                   LilyPreparserClassBodyItem *self)
{
    FREE(LilyPreparserFun, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserClassBodyItem, LilyPreparserClassBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyPreparserClassBodyItem, attribute, self);
            break;
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserClassBodyItem, macro_expand, self);
            break;
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyPreparserClassBodyItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserClass,
            LilyPreparserClass,
            String *name,
            Vec *generic_params,
            Vec *inherits,
            Vec *implements,
            Vec *body,
            enum LilyVisibility visibility,
            bool is_close)
{
    return (LilyPreparserClass){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .implements = implements,
                                 .body = body,
                                 .is_close = is_close };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserClass, const LilyPreparserClass *self)
{
    String *res = format__String(
      "LilyPreparserClass{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", inherits =");

    if (self->inherits) {
        DEBUG_VEC_STR_2(self->inherits, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", implements =");

    if (self->implements) {
        DEBUG_VEC_STR_2(self->implements, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserClassBodyItem);

    {
        char *s = format(", visibility = {s}, is_close = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_close);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    if (self->inherits) {
        FREE_BUFFER_ITEMS(self->inherits->buffer, self->inherits->len, Vec);
        FREE(Vec, self->inherits);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS(self->implements->buffer, self->implements->len, Vec);
        FREE(Vec, self->implements);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserClassBodyItem);
    FREE(Vec, self->body);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTraitBodyItemKind,
               enum LilyPreparserTraitBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE:
            return "LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE";
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE:
            return "LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyPreparserPrototype,
            LilyPreparserPrototype,
            String *name,
            Vec *generic_params,
            Vec *params,
            Vec *return_data_type)
{
    return (LilyPreparserPrototype){ .name = name,
                                     .generic_params = generic_params,
                                     .params = params,
                                     .return_data_type = return_data_type };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserPrototype,
               const LilyPreparserPrototype *self)
{
    String *res = format__String(
      "LilyPreparserPrototype{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_data_type =");

    if (self->return_data_type) {
        DEBUG_VEC_STR(self->return_data_type, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserPrototype, const LilyPreparserPrototype *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }

    if (self->return_data_type) {
        FREE(Vec, self->return_data_type);
    }
}

VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                    LilyPreparserTraitBodyItem,
                    attribute,
                    Location location,
                    LilyPreparserAttribute attribute)
{
    LilyPreparserTraitBodyItem *self =
      lily_malloc(sizeof(LilyPreparserTraitBodyItem));

    self->kind = LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE;
    self->location = location;
    self->attribute = attribute;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                    LilyPreparserTraitBodyItem,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserTraitBodyItem *self =
      lily_malloc(sizeof(LilyPreparserTraitBodyItem));

    self->kind = LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserTraitBodyItem *,
                    LilyPreparserTraitBodyItem,
                    prototype,
                    Location location,
                    LilyPreparserPrototype prototype)
{
    LilyPreparserTraitBodyItem *self =
      lily_malloc(sizeof(LilyPreparserTraitBodyItem));

    self->kind = LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE;
    self->location = location;
    self->prototype = prototype;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTraitBodyItem,
               const LilyPreparserTraitBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserTraitBodyItem{{ kind = {s}, location = {sa}",
      to_string__Debug__LilyPreparserTraitBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE: {
            char *s = format(
              ", attribute = {Sr} }",
              to_string__Debug__LilyPreparserAttribute(&self->attribute));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND: {
            char *s = format(
              ", macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE: {
            char *s = format(
              ", prototype = {Sr} }",
              to_string__Debug__LilyPreparserPrototype(&self->prototype));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                   attribute,
                   LilyPreparserTraitBodyItem *self)
{
    FREE(LilyPreparserAttribute, &self->attribute);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                   macro_expand,
                   LilyPreparserTraitBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                   prototype,
                   LilyPreparserTraitBodyItem *self)
{
    FREE(LilyPreparserPrototype, &self->prototype);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserTraitBodyItem, LilyPreparserTraitBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE:
            FREE_VARIANT(LilyPreparserTraitBodyItem, attribute, self);
            break;

        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserTraitBodyItem, macro_expand, self);
            break;

        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE:
            FREE_VARIANT(LilyPreparserTraitBodyItem, prototype, self);
            break;

        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserTrait,
            LilyPreparserTrait,
            String *name,
            Vec *generic_params,
            Vec *inherits,
            Vec *body,
            enum LilyVisibility visibility,
            bool is_close)
{
    return (LilyPreparserTrait){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .body = body,
                                 .visibility = visibility,
                                 .is_close = is_close };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserTrait, const LilyPreparserTrait *self)
{
    String *res = format__String(
      "LilyPreparserTrait{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", inherits =");

    if (self->inherits) {
        DEBUG_VEC_STR_2(self->inherits, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserTraitBodyItem);

    {
        char *s = format(", visibility = {s}, is_close = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_close);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    if (self->inherits) {
        FREE_BUFFER_ITEMS(self->inherits->buffer, self->inherits->len, Vec);
        FREE(Vec, self->inherits);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserTraitBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserRecordField *,
            LilyPreparserRecordField,
            String *name,
            Vec *data_type,
            Vec *optional_expr,
            enum LilyVisibility visibility,
            bool is_mut)
{
    LilyPreparserRecordField *self =
      lily_malloc(sizeof(LilyPreparserRecordField));

    self->name = name;
    self->data_type = data_type;
    self->optional_expr = optional_expr;
    self->visibility = visibility;
    self->is_mut = is_mut;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordField,
               const LilyPreparserRecordField *self)
{
    String *res = format__String(
      "LilyPreparserRecordField{{ name = {S}, data_type =", self->name);

    DEBUG_VEC_STR(self->data_type, res, LilyToken);

    push_str__String(res, ", optional_expr =");

    if (self->optional_expr) {
        DEBUG_VEC_STR(self->optional_expr, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", visibility = {s}, is_mut = {b} }",
                         to_string__Debug__LilyVisibility(self->visibility),
                         self->is_mut);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserRecordField, const LilyPreparserRecordField *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->data_type);

    if (self->optional_expr) {
        FREE(Vec, self->optional_expr);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObjectBodyItemKind,
               enum LilyPreparserRecordObjectBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT:
            return "LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT";
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD:
            return "LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD";
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD:
            return "LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                    LilyPreparserRecordObjectBodyItem,
                    constant,
                    Location location,
                    LilyPreparserConstant constant)
{
    LilyPreparserRecordObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordObjectBodyItem));

    self->kind = LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                    LilyPreparserRecordObjectBodyItem,
                    field,
                    Location location,
                    LilyPreparserRecordField field)
{
    LilyPreparserRecordObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordObjectBodyItem));

    self->kind = LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD;
    self->location = location;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                    LilyPreparserRecordObjectBodyItem,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserRecordObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordObjectBodyItem));

    self->kind = LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserRecordObjectBodyItem *,
                    LilyPreparserRecordObjectBodyItem,
                    method,
                    Location location,
                    LilyPreparserMethod method)
{
    LilyPreparserRecordObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordObjectBodyItem));

    self->kind = LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObjectBodyItem,
               const LilyPreparserRecordObjectBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserRecordObjectBodyItem{{ kind = {s}, location = {sa}",
      to_string__Debug__LilyPreparserRecordObjectBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT: {
            char *s =
              format(", constant = {Sr} }",
                     to_string__Debug__LilyPreparserConstant(&self->constant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD: {
            char *s =
              format(", field = {Sr} }",
                     to_string__Debug__LilyPreparserRecordField(&self->field));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND: {
            char *s = format(
              ", macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD: {
            char *s =
              format(", method = {Sr} }",
                     to_string__Debug__LilyPreparserMethod(&self->method));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                   constant,
                   LilyPreparserRecordObjectBodyItem *self)
{
    switch (self->constant.kind) {
        case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
            FREE_BUFFER_ITEMS(self->constant.multiple->buffer,
                              self->constant.multiple->len,
                              LilyPreparserConstantInfo);
            FREE(Vec, self->constant.multiple);

            break;
        case LILY_PREPARSER_CONSTANT_KIND_SIMPLE:
            FREE(LilyPreparserConstantInfo, self->constant.simple);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                   field,
                   LilyPreparserRecordObjectBodyItem *self)
{
    FREE(LilyPreparserRecordField, &self->field);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                   macro_expand,
                   LilyPreparserRecordObjectBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                   method,
                   LilyPreparserRecordObjectBodyItem *self)
{
    FREE(LilyPreparserFun, &self->method);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
           LilyPreparserRecordObjectBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserRecordObjectBodyItem, constant, self);
            break;
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_FIELD:
            FREE_VARIANT(LilyPreparserRecordObjectBodyItem, field, self);
            break;
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserRecordObjectBodyItem, macro_expand, self);
            break;
        case LILY_PREPARSER_RECORD_OBJECT_BODY_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyPreparserRecordObjectBodyItem, method, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserRecordObject,
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

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordObject,
               const LilyPreparserRecordObject *self)
{
    String *res = format__String(
      "LilyPreparserRecordObject{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", implements =");

    if (self->implements) {
        DEBUG_VEC_STR_2(self->implements, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserRecordObjectBodyItem);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserRecordObject, const LilyPreparserRecordObject *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS(self->implements->buffer, self->implements->len, Vec);
        FREE(Vec, self->implements);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserRecordObjectBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserEnumVariant *,
            LilyPreparserEnumVariant,
            String *name,
            Vec *data_type)
{
    LilyPreparserEnumVariant *self =
      lily_malloc(sizeof(LilyPreparserEnumVariant));

    self->name = name;
    self->data_type = data_type;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumVariant,
               const LilyPreparserEnumVariant *self)
{
    String *res = format__String(
      "LilyPreparserEnumVariant{{ name = {S}, data_type =", self->name);

    if (self->data_type) {
        DEBUG_VEC_STR(self->data_type, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserEnumVariant, const LilyPreparserEnumVariant *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->data_type) {
        FREE(Vec, self->data_type);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObjectBodyItemKind,
               enum LilyPreparserEnumObjectBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT:
            return "LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT";
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD:
            return "LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD";
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT:
            return "LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                    LilyPreparserEnumObjectBodyItem,
                    constant,
                    Location location,
                    LilyPreparserConstant constant)
{
    LilyPreparserEnumObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumObjectBodyItem));

    self->kind = LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                    LilyPreparserEnumObjectBodyItem,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserEnumObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumObjectBodyItem));

    self->kind = LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                    LilyPreparserEnumObjectBodyItem,
                    method,
                    Location location,
                    LilyPreparserMethod method)
{
    LilyPreparserEnumObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumObjectBodyItem));

    self->kind = LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD;
    self->location = location;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                    LilyPreparserEnumObjectBodyItem,
                    variant,
                    Location location,
                    LilyPreparserEnumVariant variant)
{
    LilyPreparserEnumObjectBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumObjectBodyItem));

    self->kind = LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT;
    self->location = location;
    self->variant = variant;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObjectBodyItem,
               const LilyPreparserEnumObjectBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserEnumObjectBodyItem{{ kind = {s}, location = {sa}",
      to_string__Debug__LilyPreparserEnumObjectBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT: {
            char *s =
              format(", constant = {Sr} }",
                     to_string__Debug__LilyPreparserConstant(&self->constant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND: {
            char *s = format(
              ", macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD: {
            char *s =
              format(", method = {Sr} }",
                     to_string__Debug__LilyPreparserMethod(&self->method));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT: {
            char *s = format(
              ", variant = {Sr} }",
              to_string__Debug__LilyPreparserEnumVariant(&self->variant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                   constant,
                   LilyPreparserEnumObjectBodyItem *self)
{
    switch (self->constant.kind) {
        case LILY_PREPARSER_CONSTANT_KIND_MULTIPLE:
            FREE_BUFFER_ITEMS(self->constant.multiple->buffer,
                              self->constant.multiple->len,
                              LilyPreparserConstantInfo);
            FREE(Vec, self->constant.multiple);

            break;
        case LILY_PREPARSER_CONSTANT_KIND_SIMPLE:
            FREE(LilyPreparserConstantInfo, self->constant.simple);

            break;
        default:
            UNREACHABLE("unknown variant");
    }

    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                   macro_expand,
                   LilyPreparserEnumObjectBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                   method,
                   LilyPreparserEnumObjectBodyItem *self)
{
    FREE(LilyPreparserFun, &self->method);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                   variant,
                   LilyPreparserEnumObjectBodyItem *self)
{
    if (destroy_all) {
        FREE(String, self->variant.name);
    }

    if (self->variant.data_type) {
        FREE(Vec, self->variant.data_type);
    }

    lily_free(self);
}

DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
           LilyPreparserEnumObjectBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserEnumObjectBodyItem, constant, self);
            break;

        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserEnumObjectBodyItem, macro_expand, self);
            break;

        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_METHOD:
            FREE_VARIANT(LilyPreparserEnumObjectBodyItem, method, self);
            break;

        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_VARIANT:
            FREE_VARIANT(LilyPreparserEnumObjectBodyItem, variant, self);
            break;

        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserEnumObject,
            LilyPreparserEnumObject,
            String *name,
            Vec *generic_params,
            Vec *implements,
            Vec *body,
            enum LilyVisibility visibility)
{
    return (LilyPreparserEnumObject){ .name = name,
                                      .generic_params = generic_params,
                                      .implements = implements,
                                      .body = body,
                                      .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumObject,
               const LilyPreparserEnumObject *self)
{
    String *res = format__String(
      "LilyPreparserEnumObject{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", implements =");

    if (self->implements) {
        DEBUG_VEC_STR_2(self->implements, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserEnumObjectBodyItem);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserEnumObject, const LilyPreparserEnumObject *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS(self->implements->buffer, self->implements->len, Vec);
        FREE(Vec, self->implements);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserEnumObjectBodyItem);
    FREE(Vec, self->body);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserObjectKind,
               enum LilyPreparserObjectKind self)
{
    switch (self) {
        case LILY_PREPARSER_OBJECT_KIND_CLASS:
            return "LILY_PREPARSER_OBJECT_KIND_CLASS";
        case LILY_PREPARSER_OBJECT_KIND_TRAIT:
            return "LILY_PREPARSER_OBJECT_KIND_TRAIT";
        case LILY_PREPARSER_OBJECT_KIND_RECORD:
            return "LILY_PREPARSER_OBJECT_KIND_RECORD";
        case LILY_PREPARSER_OBJECT_KIND_ENUM:
            return "LILY_PREPARSER_OBJECT_KIND_ENUM";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserObject,
                    LilyPreparserObject,
                    class,
                    LilyPreparserClass class)
{
    return (LilyPreparserObject){ .kind = LILY_PREPARSER_OBJECT_KIND_CLASS,
                                  .class = class };
}

VARIANT_CONSTRUCTOR(LilyPreparserObject,
                    LilyPreparserObject,
                    trait,
                    LilyPreparserTrait trait)
{
    return (LilyPreparserObject){ .kind = LILY_PREPARSER_OBJECT_KIND_TRAIT,
                                  .trait = trait };
}

VARIANT_CONSTRUCTOR(LilyPreparserObject,
                    LilyPreparserObject,
                    record,
                    LilyPreparserRecordObject record)
{
    return (LilyPreparserObject){ .kind = LILY_PREPARSER_OBJECT_KIND_RECORD,
                                  .record = record };
}

VARIANT_CONSTRUCTOR(LilyPreparserObject,
                    LilyPreparserObject,
                    enum_,
                    LilyPreparserEnumObject enum_)
{
    return (LilyPreparserObject){ .kind = LILY_PREPARSER_OBJECT_KIND_ENUM,
                                  .enum_ = enum_ };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserObject, const LilyPreparserObject *self)
{
    String *res =
      format__String("LilyPreparserObject{{ kind = {s}",
                     to_string__Debug__LilyPreparserObjectKind(self->kind));

    switch (self->kind) {
        case LILY_PREPARSER_OBJECT_KIND_CLASS: {
            char *s =
              format(", class = {Sr} }",
                     to_string__Debug__LilyPreparserClass(&self->class));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_OBJECT_KIND_TRAIT: {
            char *s =
              format(", trait = {Sr} }",
                     to_string__Debug__LilyPreparserTrait(&self->trait));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_OBJECT_KIND_RECORD: {
            char *s = format(
              ", record = {Sr} }",
              to_string__Debug__LilyPreparserRecordObject(&self->record));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_OBJECT_KIND_ENUM: {
            char *s =
              format(", enum = {Sr} }",
                     to_string__Debug__LilyPreparserEnumObject(&self->enum_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant")
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserObject, class, const LilyPreparserObject *self)
{
    FREE(LilyPreparserClass, &self->class);
}

VARIANT_DESTRUCTOR(LilyPreparserObject, trait, const LilyPreparserObject *self)
{
    FREE(LilyPreparserTrait, &self->trait);
}

VARIANT_DESTRUCTOR(LilyPreparserObject, record, const LilyPreparserObject *self)
{
    FREE(LilyPreparserRecordObject, &self->record);
}

VARIANT_DESTRUCTOR(LilyPreparserObject, enum_, const LilyPreparserObject *self)
{
    FREE(LilyPreparserEnumObject, &self->enum_);
}

DESTRUCTOR(LilyPreparserObject, const LilyPreparserObject *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_OBJECT_KIND_CLASS:
            FREE_VARIANT(LilyPreparserObject, class, self);
            break;
        case LILY_PREPARSER_OBJECT_KIND_ENUM:
            FREE_VARIANT(LilyPreparserObject, enum_, self);
            break;
        case LILY_PREPARSER_OBJECT_KIND_RECORD:
            FREE_VARIANT(LilyPreparserObject, record, self);
            break;
        case LILY_PREPARSER_OBJECT_KIND_TRAIT:
            FREE_VARIANT(LilyPreparserObject, trait, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordBodyItemKind,
               enum LilyPreparserRecordBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD:
            return "LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD";
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserRecordBodyItem *,
                    LilyPreparserRecordBodyItem,
                    field,
                    Location location,
                    LilyPreparserRecordField field)
{
    LilyPreparserRecordBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordBodyItem));

    self->kind = LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD;
    self->location = location;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserRecordBodyItem *,
                    LilyPreparserRecordBodyItem,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserRecordBodyItem *self =
      lily_malloc(sizeof(LilyPreparserRecordBodyItem));

    self->kind = LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserRecordBodyItem,
               const LilyPreparserRecordBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserRecordBodyItem{{ kind = {s}, location = {sa}",
      to_string__Debug__LilyPreparserRecordBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD: {
            char *s =
              format(", field = {Sr} }",
                     to_string__Debug__LilyPreparserRecordField(&self->field));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND: {
            char *s = format(
              ", macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserRecordBodyItem,
                   field,
                   LilyPreparserRecordBodyItem *self)
{
    FREE(LilyPreparserRecordField, &self->field);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserRecordBodyItem,
                   macro_expand,
                   LilyPreparserRecordBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserRecordBodyItem, LilyPreparserRecordBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_FIELD:
            FREE_VARIANT(LilyPreparserRecordBodyItem, field, self);
            break;
        case LILY_PREPARSER_RECORD_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserRecordBodyItem, macro_expand, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserAlias,
            LilyPreparserAlias,
            String *name,
            Vec *generic_params,
            Vec *data_type,
            enum LilyVisibility visibility)
{
    return (LilyPreparserAlias){ .name = name,
                                 .generic_params = generic_params,
                                 .data_type = data_type,
                                 .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserAlias, const LilyPreparserAlias *self)
{
    String *res = format__String(
      "LilyPreparserAlias{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", data_type =");
    DEBUG_VEC_STR(self->data_type, res, LilyToken);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserAlias, const LilyPreparserAlias *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    FREE(Vec, self->data_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumBodyItemKind,
               enum LilyPreparserEnumBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT:
            return "LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

// Construct LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND).
VARIANT_CONSTRUCTOR(LilyPreparserEnumBodyItem *,
                    LilyPreparserEnumBodyItem,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserEnumBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumBodyItem));

    self->kind = LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

// Construct LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT).
VARIANT_CONSTRUCTOR(LilyPreparserEnumBodyItem *,
                    LilyPreparserEnumBodyItem,
                    variant,
                    Location location,
                    LilyPreparserEnumVariant variant)
{
    LilyPreparserEnumBodyItem *self =
      lily_malloc(sizeof(LilyPreparserEnumBodyItem));

    self->kind = LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT;
    self->location = location;
    self->variant = variant;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserEnumBodyItem,
               const LilyPreparserEnumBodyItem *self)
{
    String *res = format__String(
      "LilyPreparserEnumBodyItem{{ kind = {s}, location = {sa}, ",
      to_string__Debug__LilyPreparserEnumBodyItemKind(self->kind),
      to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND: {
            char *s = format(
              "macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT: {
            char *s = format(
              "variant = {Sr} }",
              to_string__Debug__LilyPreparserEnumVariant(&self->variant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;

    return res;
}
#endif

// Free LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND).
VARIANT_DESTRUCTOR(LilyPreparserEnumBodyItem,
                   macro_expand,
                   LilyPreparserEnumBodyItem *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

// Free LilyPreparserEnumBodyItem type
// (LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT).
VARIANT_DESTRUCTOR(LilyPreparserEnumBodyItem,
                   variant,
                   LilyPreparserEnumBodyItem *self)
{
    FREE(LilyPreparserEnumVariant, &self->variant);
    lily_free(self);
}

// Free LilyPreparserEnumBodyItem type.
DESTRUCTOR(LilyPreparserEnumBodyItem, LilyPreparserEnumBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserEnumBodyItem, macro_expand, self);
            break;
        case LILY_PREPARSER_ENUM_BODY_ITEM_KIND_VARIANT:
            FREE_VARIANT(LilyPreparserEnumBodyItem, variant, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserEnum,
            LilyPreparserEnum,
            String *name,
            Vec *generic_params,
            Vec *body,
            enum LilyVisibility visibility)
{
    return (LilyPreparserEnum){ .name = name,
                                .generic_params = generic_params,
                                .body = body,
                                .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserEnum, const LilyPreparserEnum *self)
{
    String *res = format__String(
      "LilyPreparserEnum{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserEnumBodyItem);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserEnum, const LilyPreparserEnum *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserEnumBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserRecord,
            LilyPreparserRecord,
            String *name,
            Vec *generic_params,
            Vec *body,
            enum LilyVisibility visibility)
{
    return (LilyPreparserRecord){ .name = name,
                                  .generic_params = generic_params,
                                  .body = body,
                                  .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserRecord, const LilyPreparserRecord *self)
{
    String *res = format__String(
      "LilyPreparserRecord{{ name = {S}, generic_params =", self->name);

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserRecordBodyItem);

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserRecord, const LilyPreparserRecord *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->generic_params) {
        FREE_BUFFER_ITEMS(
          self->generic_params->buffer, self->generic_params->len, Vec);
        FREE(Vec, self->generic_params);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserRecordBodyItem);
    FREE(Vec, self->body);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserTypeKind,
               enum LilyPreparserTypeKind self)
{
    switch (self) {
        case LILY_PREPARSER_TYPE_KIND_ALIAS:
            return "LILY_PREPARSER_TYPE_KIND_ALIAS";
        case LILY_PREPARSER_TYPE_KIND_ENUM:
            return "LILY_PREPARSER_TYPE_KIND_ENUM";
        case LILY_PREPARSER_TYPE_KIND_RECORD:
            return "LILY_PREPARSER_TYPE_KIND_RECORD";
        default:
            UNREACHABLE("unknown variant")
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserType,
                    LilyPreparserType,
                    alias,
                    LilyPreparserAlias alias)
{
    return (LilyPreparserType){
        .alias = alias,
        .kind = LILY_PREPARSER_TYPE_KIND_ALIAS,
    };
}

VARIANT_CONSTRUCTOR(LilyPreparserType,
                    LilyPreparserType,
                    enum_,
                    LilyPreparserEnum enum_)
{
    return (LilyPreparserType){ .kind = LILY_PREPARSER_TYPE_KIND_ENUM,
                                .enum_ = enum_ };
}

VARIANT_CONSTRUCTOR(LilyPreparserType,
                    LilyPreparserType,
                    record,
                    LilyPreparserRecord record)
{
    return (LilyPreparserType){ .kind = LILY_PREPARSER_TYPE_KIND_RECORD,
                                .record = record };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserType, const LilyPreparserType *self)
{
    String *res =
      format__String("LilyPreparserType{{ kind = {s}",
                     to_string__Debug__LilyPreparserTypeKind(self->kind));

    switch (self->kind) {
        case LILY_PREPARSER_TYPE_KIND_ALIAS: {
            char *s =
              format(", alias = {Sr} }",
                     to_string__Debug__LilyPreparserAlias(&self->alias));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_TYPE_KIND_ENUM: {
            char *s = format(", enum = {Sr} }",
                             to_string__Debug__LilyPreparserEnum(&self->enum_));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_TYPE_KIND_RECORD: {
            char *s =
              format(", record = {Sr} }",
                     to_string__Debug__LilyPreparserRecord(&self->record));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant")
    }

    return res;
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserType, alias, const LilyPreparserType *self)
{
    FREE(LilyPreparserAlias, &self->alias);
}

VARIANT_DESTRUCTOR(LilyPreparserType, enum_, const LilyPreparserType *self)
{
    FREE(LilyPreparserEnum, &self->enum_);
}

VARIANT_DESTRUCTOR(LilyPreparserType, record, const LilyPreparserType *self)
{
    FREE(LilyPreparserRecord, &self->record);
}

DESTRUCTOR(LilyPreparserType, const LilyPreparserType *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_TYPE_KIND_ALIAS:
            FREE_VARIANT(LilyPreparserType, alias, self);
            break;
        case LILY_PREPARSER_TYPE_KIND_ENUM:
            FREE_VARIANT(LilyPreparserType, enum_, self);
            break;
        case LILY_PREPARSER_TYPE_KIND_RECORD:
            FREE_VARIANT(LilyPreparserType, record, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserLibConstantPrototype,
            LilyPreparserLibConstantPrototype,
            String *name,
            Vec *data_type)
{
    return (LilyPreparserLibConstantPrototype){ .name = name,
                                                .data_type = data_type };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibConstantPrototype,
               const LilyPreparserLibConstantPrototype *self)
{
    String *res = format__String(
      "LilyPreparserLibConstantPrototype{{ name = {S}, data_type =",
      self->name);

    DEBUG_VEC_STR(self->data_type, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserLibConstantPrototype,
           const LilyPreparserLibConstantPrototype *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    FREE(Vec, self->data_type);
}

CONSTRUCTOR(LilyPreparserLibFunPrototype,
            LilyPreparserLibFunPrototype,
            String *name,
            String *new_name,
            Vec *params,
            Vec *return_data_type)
{
    return (LilyPreparserLibFunPrototype){ .name = name,
                                           .new_name = new_name,
                                           .params = params,
                                           .return_data_type =
                                             return_data_type };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibFunPrototype,
               const LilyPreparserLibFunPrototype *self)
{
    String *res = format__String(
      "LilyPreparserLibFunPrototype{{ name = {S}, new_name = ", self->name);

    if (self->new_name) {
        append__String(res, self->new_name);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", params =");

    if (self->params) {
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", return_data_type =");
    DEBUG_VEC_STR(self->return_data_type, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserLibFunPrototype,
           const LilyPreparserLibFunPrototype *self)
{
    if (destroy_all) {
        FREE(String, self->name);

        if (self->new_name) {
            FREE(String, self->new_name);
        }
    }

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }

    FREE(Vec, self->return_data_type);
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibBodyItemKind,
               enum LilyPreparserLibBodyItemKind self)
{
    switch (self) {
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT:
            return "LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT";
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN:
            return "LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN";
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE:
            return "LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE";
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT:
            return "LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

static VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                           LilyPreparserLibBodyItem,
                           constant,
                           Location location,
                           LilyPreparserLibConstantPrototype constant)
{
    LilyPreparserLibBodyItem *self =
      lily_malloc(sizeof(LilyPreparserLibBodyItem));

    self->kind = LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                    LilyPreparserLibBodyItem,
                    fun,
                    Location location,
                    LilyPreparserLibFunPrototype fun)
{
    LilyPreparserLibBodyItem *self =
      lily_malloc(sizeof(LilyPreparserLibBodyItem));

    self->kind = LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN;
    self->location = location;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                    LilyPreparserLibBodyItem,
                    type,
                    Location location,
                    LilyPreparserLibTypePrototype type)
{
    LilyPreparserLibBodyItem *self =
      lily_malloc(sizeof(LilyPreparserLibBodyItem));

    self->kind = LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE;
    self->location = location;
    self->type = type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserLibBodyItem *,
                    LilyPreparserLibBodyItem,
                    object,
                    Location location,
                    LilyPreparserLibObjectPrototype object)
{
    LilyPreparserLibBodyItem *self =
      lily_malloc(sizeof(LilyPreparserLibBodyItem));

    self->kind = LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT;
    self->location = location;
    self->object = object;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserLibBodyItem,
               const LilyPreparserLibBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT:
            return format(
              "LilyPreparserLibBodyItem{{ kind = {s}, location = {sa}, "
              "constant = {Sr} }",
              to_string__Debug__LilyPreparserLibBodyItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyPreparserLibConstantPrototype(
                &self->constant));
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN:
            return format(
              "LilyPreparserLibBodyItem{{ kind = {s}, location = {sa}, "
              "fun= {Sr} }",
              to_string__Debug__LilyPreparserLibBodyItemKind(self->kind),
              to_string__Debug__Location(&self->location),
              to_string__Debug__LilyPreparserLibFunPrototype(&self->fun));
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE:
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT:
            TODO("complete this variant");
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                   constant,
                   LilyPreparserLibBodyItem *self)
{
    FREE(LilyPreparserLibConstantPrototype, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                   fun,
                   LilyPreparserLibBodyItem *self)
{
    FREE(LilyPreparserLibFunPrototype, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                   type,
                   LilyPreparserLibBodyItem *self)
{
    // FREE(LilyPreparserLibTypePrototype, &self->type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserLibBodyItem,
                   object,
                   LilyPreparserLibBodyItem *self)
{
    // FREE(LilyPreparserLibObjectPrototype, &self->object);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserLibBodyItem, LilyPreparserLibBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserLibBodyItem, constant, self);
            break;
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_FUN:
            FREE_VARIANT(LilyPreparserLibBodyItem, fun, self);
            break;
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_TYPE:
            FREE_VARIANT(LilyPreparserLibBodyItem, type, self);
            break;
        case LILY_PREPARSER_LIB_BODY_ITEM_KIND_OBJECT:
            FREE_VARIANT(LilyPreparserLibBodyItem, object, self);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string, LilyPreparserLibFrom, enum LilyPreparserLibFrom self)
{
    switch (self) {
        case LILY_PREPARSER_LIB_FROM_CC:
            return "LILY_PREPARSER_LIB_FROM_CC";
        case LILY_PREPARSER_LIB_FROM_CPP:
            return "LILY_PREPARSER_LIB_FROM_CPP";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyPreparserLib,
            LilyPreparserLib,
            String *name,
            enum LilyPreparserLibFrom from,
            Vec *body)
{
    return (LilyPreparserLib){ .name = name, .from = from, .body = body };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserLib, const LilyPreparserLib *self)
{
    String *res = from__String("LilyPreparserLib{ name = ");

    if (self->name) {
        append__String(res, self->name);
    } else {
        push_str__String(res, "NULL");
    }

    push_str__String(res, ", from = ");
    push_str__String(res, to_string__Debug__LilyPreparserLibFrom(self->from));

    push_str__String(res, ", body =");
    DEBUG_VEC_STR(self->body, res, LilyPreparserLibBodyItem);

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserLib, const LilyPreparserLib *self)
{
    if (self->name && destroy_all) {
        FREE(String, self->name);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserLibBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserError,
            LilyPreparserError,
            String *name,
            Vec *data_type,
            Vec *generic_params,
            enum LilyVisibility visibility)
{
    return (LilyPreparserError){ .name = name,
                                 .data_type = data_type,
                                 .generic_params = generic_params,
                                 .visibility = visibility };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserError, const LilyPreparserError *self)
{
    String *res = format__String("LilyPreparserError{{ name = {S}, data_type =",
                                 self->name);

    if (self->data_type) {
        DEBUG_VEC_STR(self->data_type, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", generic_params =");

    if (self->generic_params) {
        DEBUG_VEC_STR_2(self->generic_params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    {
        char *s = format(", visibility = {s} }",
                         to_string__Debug__LilyVisibility(self->visibility));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserError, const LilyPreparserError *self)
{
    if (destroy_all) {
        FREE(String, self->name);
    }

    if (self->data_type) {
        FREE(Vec, self->data_type);
    }
}

CONSTRUCTOR(LilyPreparserUse, LilyPreparserUse, Vec *path)
{
    return (LilyPreparserUse){ .path = path };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserUse, const LilyPreparserUse *self)
{
    String *res = from__String("LilyPreparserUse{ path =");

    DEBUG_VEC_STR(self->path, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserUse, const LilyPreparserUse *self)
{
    FREE(Vec, self->path);
}

CONSTRUCTOR(LilyPreparserInclude, LilyPreparserInclude, Vec *path)
{
    return (LilyPreparserInclude){ .path = path };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserInclude,
               const LilyPreparserInclude *self)
{
    String *res = from__String("LilyPreparserInclude{ path =");

    DEBUG_VEC_STR(self->path, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserInclude, const LilyPreparserInclude *self)
{
    FREE(Vec, self->path);
}

CONSTRUCTOR(LilyPreparserMacroExpand,
            LilyPreparserMacroExpand,
            String *name,
            Vec *params)
{
    return (LilyPreparserMacroExpand){ .name = name, .params = params };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserMacroExpand,
               const LilyPreparserMacroExpand *self)
{
    String *res = format__String(
      "LilyPreparserMacroExpand{{ name = {S}, params =", self->name);

    if (self->params) {
        DEBUG_VEC_STR_2(self->params, res, LilyToken);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserMacroExpand, const LilyPreparserMacroExpand *self)
{
    FREE(String, self->name);

    if (self->params) {
        FREE_BUFFER_ITEMS(self->params->buffer, self->params->len, Vec);
        FREE(Vec, self->params);
    }
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserDeclKind,
               enum LilyPreparserDeclKind self)
{
    switch (self) {
        case LILY_PREPARSER_DECL_KIND_CONSTANT:
            return "LILY_PREPARSER_DECL_KIND_CONSTANT";
        case LILY_PREPARSER_DECL_KIND_ERROR:
            return "LILY_PREPARSER_DECL_KIND_ERROR";
        case LILY_PREPARSER_DECL_KIND_FUN:
            return "LILY_PREPARSER_DECL_KIND_FUN";
        case LILY_PREPARSER_DECL_KIND_INCLUDE:
            return "LILY_PREPARSER_DECL_KIND_INCLUDE";
        case LILY_PREPARSER_DECL_KIND_LIB:
            return "LILY_PREPARSER_DECL_KIND_LIB";
        case LILY_PREPARSER_DECL_KIND_MACRO_EXPAND:
            return "LILY_PREPARSER_DECL_KIND_MACRO_EXPAND";
        case LILY_PREPARSER_DECL_KIND_MODULE:
            return "LILY_PREPARSER_DECL_KIND_MODULE";
        case LILY_PREPARSER_DECL_KIND_OBJECT:
            return "LILY_PREPARSER_DECL_KIND_OBJECT";
        case LILY_PREPARSER_DECL_KIND_TYPE:
            return "LILY_PREPARSER_DECL_KIND_TYPE";
        case LILY_PREPARSER_DECL_KIND_USE:
            return "LILY_PREPARSER_DECL_KIND_USE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    constant,
                    Location location,
                    LilyPreparserConstant constant)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_CONSTANT;
    self->location = location;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    error,
                    Location location,
                    LilyPreparserError error)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_ERROR;
    self->location = location;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    fun,
                    Location location,
                    LilyPreparserFun fun)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_FUN;
    self->location = location;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    include,
                    Location location,
                    LilyPreparserInclude include)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_INCLUDE;
    self->location = location;
    self->include = include;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    lib,
                    Location location,
                    LilyPreparserLib lib)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_LIB;
    self->location = location;
    self->lib = lib;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    macro_expand,
                    Location location,
                    LilyPreparserMacroExpand macro_expand)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_MACRO_EXPAND;
    self->location = location;
    self->macro_expand = macro_expand;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    module,
                    Location location,
                    LilyPreparserModule module)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_MODULE;
    self->location = location;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    object,
                    Location location,
                    LilyPreparserObject object)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_OBJECT;
    self->location = location;
    self->object = object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    type,
                    Location location,
                    LilyPreparserType type)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_TYPE;
    self->location = location;
    self->type = type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                    LilyPreparserDecl,
                    use,
                    Location location,
                    LilyPreparserUse use)
{
    LilyPreparserDecl *self = lily_malloc(sizeof(LilyPreparserDecl));

    self->kind = LILY_PREPARSER_DECL_KIND_USE;
    self->location = location;
    self->use = use;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserDecl, const LilyPreparserDecl *self)
{
    String *res =
      format__String("LilyPreparserDecl{{ kind = {s}, location = {sa}",
                     to_string__Debug__LilyPreparserDeclKind(self->kind),
                     to_string__Debug__Location(&self->location));

    switch (self->kind) {
        case LILY_PREPARSER_DECL_KIND_CONSTANT: {
            char *s =
              format(", constant = {Sr} }",
                     to_string__Debug__LilyPreparserConstant(&self->constant));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_ERROR: {
            char *s =
              format(", error = {Sr} }",
                     to_string__Debug__LilyPreparserError(&self->error));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_FUN: {
            char *s = format(", fun = {Sr} }",
                             to_string__Debug__LilyPreparserFun(&self->fun));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_INCLUDE: {
            char *s =
              format(", include = {Sr} }",
                     to_string__Debug__LilyPreparserInclude(&self->include));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_LIB: {
            char *s = format(", lib = {Sr} }",
                             to_string__Debug__LilyPreparserLib(&self->lib));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_MACRO_EXPAND: {
            char *s = format(
              ", macro_expand = {Sr} }",
              to_string__Debug__LilyPreparserMacroExpand(&self->macro_expand));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_MODULE: {
            char *s =
              format(", module = {Sr} }",
                     to_string__Debug__LilyPreparserModule(&self->module));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_OBJECT: {
            char *s =
              format(", object = {Sr} }",
                     to_string__Debug__LilyPreparserObject(&self->object));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_TYPE: {
            char *s = format(", type = {Sr} }",
                             to_string__Debug__LilyPreparserType(&self->type));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        case LILY_PREPARSER_DECL_KIND_USE: {
            char *s = format(", use = {Sr} }",
                             to_string__Debug__LilyPreparserUse(&self->use));

            PUSH_STR_AND_FREE(res, s);

            break;
        }
        default:
            UNREACHABLE("unknown variant");
    }

    return res;
}

void
IMPL_FOR_DEBUG(debug, LilyPreparserDecl, const LilyPreparserDecl *self)
{
    PRINTLN("{Sr}", to_string__Debug__LilyPreparserDecl(self));
}
#endif

VARIANT_DESTRUCTOR(LilyPreparserDecl, constant, LilyPreparserDecl *self)
{
    FREE(LilyPreparserConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, error, LilyPreparserDecl *self)
{
    FREE(LilyPreparserError, &self->error);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, fun, LilyPreparserDecl *self)
{
    FREE(LilyPreparserFun, &self->fun);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, include, LilyPreparserDecl *self)
{
    FREE(LilyPreparserInclude, &self->include);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, lib, LilyPreparserDecl *self)
{
    FREE(LilyPreparserLib, &self->lib);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, macro_expand, LilyPreparserDecl *self)
{
    FREE(LilyPreparserMacroExpand, &self->macro_expand);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, module, LilyPreparserDecl *self)
{
    FREE(LilyPreparserModule, &self->module);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, object, LilyPreparserDecl *self)
{
    FREE(LilyPreparserObject, &self->object);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, type, LilyPreparserDecl *self)
{
    FREE(LilyPreparserType, &self->type);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, use, LilyPreparserDecl *self)
{
    FREE(LilyPreparserUse, &self->use);
    lily_free(self);
}

DESTRUCTOR(LilyPreparserDecl, LilyPreparserDecl *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_DECL_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserDecl, constant, self);
            break;

        case LILY_PREPARSER_DECL_KIND_ERROR:
            FREE_VARIANT(LilyPreparserDecl, error, self);
            break;

        case LILY_PREPARSER_DECL_KIND_FUN:
            FREE_VARIANT(LilyPreparserDecl, fun, self);
            break;

        case LILY_PREPARSER_DECL_KIND_INCLUDE:
            FREE_VARIANT(LilyPreparserDecl, include, self);
            break;

        case LILY_PREPARSER_DECL_KIND_LIB:
            FREE_VARIANT(LilyPreparserDecl, lib, self);
            break;

        case LILY_PREPARSER_DECL_KIND_MACRO_EXPAND:
            FREE_VARIANT(LilyPreparserDecl, macro_expand, self);
            break;

        case LILY_PREPARSER_DECL_KIND_MODULE:
            FREE_VARIANT(LilyPreparserDecl, module, self);
            break;

        case LILY_PREPARSER_DECL_KIND_OBJECT:
            FREE_VARIANT(LilyPreparserDecl, object, self);
            break;

        case LILY_PREPARSER_DECL_KIND_TYPE:
            FREE_VARIANT(LilyPreparserDecl, type, self);
            break;

        case LILY_PREPARSER_DECL_KIND_USE:
            FREE_VARIANT(LilyPreparserDecl, use, self);
            break;

        default:
            UNREACHABLE("unknown variant");
    }
}

void
next_token__LilyPreparser(LilyPreparser *self)
{
    self->current = self->position + 1 < self->tokens->len
                      ? get__Vec(self->tokens, ++self->position)
                      : self->current;
}

void
jump__LilyPreparser(LilyPreparser *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        self->position += n;
        self->current = get__Vec(self->tokens, self->position);
    }
}

LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n)
{
    if (self->position + n < self->tokens->len) {
        return get__Vec(self->tokens, self->position + n);
    }

    return NULL;
}

bool
is_start_a_new_block__LilyPreparser(const LilyPreparser *self)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_FUN:
        case LILY_TOKEN_KIND_KEYWORD_MACRO:
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
        case LILY_TOKEN_KIND_KEYWORD_object:
        case LILY_TOKEN_KIND_KEYWORD_PUB:
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
        case LILY_TOKEN_KIND_COMMENT_DOC:
        case LILY_TOKEN_KIND_HASHTAG:
            return true;

        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                if (peeked->kind == LILY_TOKEN_KIND_BANG) {
                    return true;
                }
            }

            return false;
        }

        default:
            return false;
    }
}

void
leave_block_finish_by_end__LilyPreparser(LilyPreparser *self)
{
    // begin .. end
    // if .. end
    // match .. end
    // while .. end
    // for .. end
    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_IF:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_FOR:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_AT:
                leave_brace_block__LilyPreparser(self);
                break;
            default:
                next_token__LilyPreparser(self);
        }
    }

    next_token__LilyPreparser(self); // skip `end` keyword
}

void
leave_brace_block__LilyPreparser(LilyPreparser *self)
{
    // @{ .. }
    while (!must_close_basic_brace_block__LilyPreparser(self)) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_IF:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_KEYWORD_FOR:
                leave_block_finish_by_end__LilyPreparser(self);
                break;
            case LILY_TOKEN_KIND_AT:
                leave_brace_block__LilyPreparser(self);
                break;
            default:
                next_token__LilyPreparser(self);
        }
    }

    next_token__LilyPreparser(self); // skip `}`
}

void
go_to_next_block__LilyPreparser(LilyPreparser *self)
{
    while (!is_start_a_new_block__LilyPreparser(self) &&
           self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_SEMICOLON) {
        next_token__LilyPreparser(self);
    }
}

Vec *
preparse_paren_with_comma_sep__LilyPreparser(LilyPreparser *self)
{
    PREPARSE_CLOSING(LILY_TOKEN_KIND_R_PAREN, LILY_TOKEN_KIND_COMMA);
}

Vec *
preparse_hook_with_comma_sep__LilyPreparser(LilyPreparser *self)
{
    PREPARSE_CLOSING(LILY_TOKEN_KIND_R_HOOK, LILY_TOKEN_KIND_COMMA);
}

Vec *
preparse_brace_with_comma_sep__Lilypreparser(LilyPreparser *self)
{
    PREPARSE_CLOSING(LILY_TOKEN_KIND_R_BRACE, LILY_TOKEN_KIND_COMMA);
}

LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self)
{
    // import <import_value> [as <as_value>] ;
    // <import_value> = <literal_str>
    // <as_value> = <identifier_normal>
    String *import_value = NULL;
    String *as_value = NULL;

    next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_LITERAL_STR:
            import_value = clone__String(self->current->literal_str);
            next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IMPORT_VALUE),
                          NULL,
                          NULL,
                          NULL),
              &self->count_error);
            return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_AS:
            next_token__LilyPreparser(self);

            if (self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_NORMAL) {
                as_value = clone__String(self->current->identifier_normal);
                next_token__LilyPreparser(self);
            } else {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected identifier after `as` keyword")),
                  &self->count_error);

                FREE(String, current_s);

                return NULL;
            }

            break;
        default:
            break;
    }

    END_LOCATION(&location_decl, self->current->location);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);
        }
    }

    return NEW(LilyPreparserImport, import_value, as_value, location_decl);
}

LilyPreparserMacro *
preparse_macro__LilyPreparser(LilyPreparser *self)
{
    // macro <name> [ ( <params> ) ] = { <tokens> } ;
    // <name> = <identifier_normal>
    // <param> = <token> [ <token> ]*
    // <params> = <param> [ , <param> ]*
    // <tokens> = <token> [ <token> ]*
    next_token__LilyPreparser(self);

    // 1. Get name of the macro.
    GET_NAME(self, NULL);

    // 2. Get params of the macro
    Vec *params = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            switch (peeked->kind) {
                case LILY_TOKEN_KIND_R_PAREN:
                    jump__LilyPreparser(self, 2);
                    break;
                default:
                    params = preparse_paren_with_comma_sep__LilyPreparser(self);
            }

            break;
        }

        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `=`")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_BRACE:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `{`")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
    }

    // 3. Get tokens of the macro.
    Vec *tokens = NEW(Vec);

    goto get_tokens;

get_brace_content: {
    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE) {
        push__Vec(tokens, self->current);
        next_token__LilyPreparser(self);
    }

    push__Vec(tokens, self->current);
    next_token__LilyPreparser(self);

    goto get_tokens;
}

get_tokens: {
    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE &&
           self->current->kind != LILY_TOKEN_KIND_L_BRACE &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(tokens, self->current);
        next_token__LilyPreparser(self);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_R_BRACE:
            next_token__LilyPreparser(self);
            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);

            if (tokens->len == 0) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_MACRO_DO_NOTHING),
                              NULL,
                              NULL,
                              NULL),
                  &self->count_error);

                // Clean up allocations

                if (!params) {
                    FREE_BUFFER_ITEMS(params->buffer, params->len, Vec);
                    FREE(Vec, params);
                }

                FREE(String, name);
                FREE(Vec, tokens);

                return NULL;
            } else {
                Location location_eof = clone__Location(
                  &CAST(LilyToken *, last__Vec(tokens))->location);
                start__Location(&location_eof,
                                location_eof.end_line,
                                location_eof.end_column,
                                location_eof.end_position);

                push__Vec(tokens,
                          NEW(LilyToken,
                              LILY_TOKEN_KIND_EOF,
                              location_eof)); // Add EOF.
            }

            break;
        case LILY_TOKEN_KIND_L_BRACE:
            goto get_brace_content;
        case LILY_TOKEN_KIND_EOF: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;` after `}` in macro declaration")),
              &self->count_error);

            FREE(String, current_s);

            return NULL;
        }
        default:
            goto get_tokens;
    }
}

    END_LOCATION(&location_decl, self->current->location);

    return NEW(LilyPreparserMacro, name, params, tokens, location_decl);
}

int
preparse_package__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info)
{
    // package [ <name> ] = <package_content> end
    // <name> = <identifier_normal>
    // <package_content> = <sub_pkg> [ <sub_pkg> ]*
    // <sub_pkg> = [ <pub> ] .<sub_pkg_name> ;
    // <sub_pkg_name> = <identifier_string> | <identifier_normal>
    next_token__LilyPreparser(self); // skip `package` keyword

    // 1. Get package name.
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            if (info->package->name) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError,
                                  LILY_ERROR_KIND_PACKAGE_NAME_ALREADY_DEFINED),
                              NULL,
                              NULL,
                              NULL),
                  &self->count_error);

                return 0;
            }

            info->package->name =
              clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `=`")),
              &self->count_error);

            FREE(String, current_s);

            return 0;
        }
    }

    // 2. Get package content.
    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        String *sub_pkg_name = NULL;
        String *sub_pkg_global_name = NULL;
        enum LilyVisibility visibility = LILY_VISIBILITY_PRIVATE;

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_PUB:
                next_token__LilyPreparser(self);
                visibility = LILY_VISIBILITY_PUBLIC;

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_DOT:
                        goto get_pkg_name;
                    default:
                        goto expected_dot;
                }

                break;
            case LILY_TOKEN_KIND_DOT:
            get_pkg_name: {
                next_token__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                        // Check if self->current->identifier_string contains
                        // only alnum, `_` or `.` characters.
                        if (last__String(self->current->identifier_string) ==
                            '.') {
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW(LilyError,
                                    LILY_ERROR_KIND_UNEXPECTED_CHARACTER),
                                NULL,
                                NULL,
                                from__String("expected identifier after `.`")),
                              &self->count_error);
                        }

                        // Check the validity of the identifier string
                        for (Usize i = 0;
                             i < self->current->identifier_string->len;
                             ++i) {
                            char c =
                              self->current->identifier_string->buffer[i++];

                            if (isalpha(c) || c == '_') {
                                c = self->current->identifier_string->buffer[i];

                                while (c && (isalnum(c) || c == '_')) {
                                    c = self->current->identifier_string
                                          ->buffer[++i];
                                }

                                if (c) {
                                    if (c != '.') {
                                        goto unexpected_character;
                                    }
                                }
                            } else {
                            unexpected_character: {
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_UNEXPECTED_CHARACTER),
                                    NULL,
                                    init__Vec(
                                      1,
                                      from__String("expected only identifier "
                                                   "or `.` character")),
                                    format__String("unexpected `{c}`", c)),
                                  &self->count_error);

                                return 0;
                            }
                            }
                        }

                        sub_pkg_name =
                          clone__String(self->current->identifier_string);

                        if (self->default_package_access) {
                            sub_pkg_global_name = from__String(
                              (char *)self->default_package_access);
                            append__String(sub_pkg_global_name,
                                           self->current->identifier_string);
                        } else {
                            sub_pkg_global_name =
                              clone__String(self->current->identifier_string);
                        }

                        next_token__LilyPreparser(self);

                        goto expected_semicolon;
                    case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                        if (self->default_package_access) {
                            sub_pkg_name =
                              clone__String(self->current->identifier_normal);
                            sub_pkg_global_name = from__String(
                              (char *)self->default_package_access);

                            push__String(sub_pkg_global_name, '.');
                            append__String(sub_pkg_global_name,
                                           self->current->identifier_normal);
                        } else {
                            sub_pkg_name =
                              clone__String(self->current->identifier_normal);
                            sub_pkg_global_name =
                              clone__String(self->current->identifier_normal);
                        }

                        next_token__LilyPreparser(self);

                        while (self->current->kind == LILY_TOKEN_KIND_DOT) {
                            push_str__String(sub_pkg_name, ".");
                            push_str__String(sub_pkg_global_name, ".");

                            next_token__LilyPreparser(self);

                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                                    push_str__String(
                                      sub_pkg_name,
                                      self->current->identifier_normal->buffer);
                                    push_str__String(
                                      sub_pkg_global_name,
                                      self->current->identifier_normal->buffer);
                                    next_token__LilyPreparser(self);

                                    break;
                                default:
                                    goto expected_sub_pkg_name;
                            }
                        }

                        if (self->current->kind !=
                              LILY_TOKEN_KIND_KEYWORD_END &&
                            self->current->kind == LILY_TOKEN_KIND_SEMICOLON) {
                        expected_semicolon: {
                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_SEMICOLON:
                                    next_token__LilyPreparser(self);
                                    break;
                                default: {
                                    String *current_s =
                                      to_string__LilyToken(self->current);

                                    emit__Diagnostic(
                                      NEW_VARIANT(
                                        Diagnostic,
                                        simple_lily_error,
                                        self->file,
                                        &self->current->location,
                                        NEW_VARIANT(LilyError,
                                                    unexpected_token,
                                                    current_s->buffer),
                                        NULL,
                                        NULL,
                                        from__String("expected `;`")),
                                      &self->count_error);

                                    FREE(String, current_s);
                                }
                            }
                        }
                        }

                        break;
                    default:
                    expected_sub_pkg_name: {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                            NULL,
                            NULL,
                            from__String("expected sub-package name")),
                          &self->count_error);

                        return 0;
                    }
                }

                break;
            }
            default:
            expected_dot: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `.`")),
                  &self->count_error);

                FREE(String, current_s);

                return 0;
            }
        }

        push__Vec(info->package->sub_packages,
                  NEW(LilyPreparserSubPackage,
                      visibility,
                      sub_pkg_name,
                      sub_pkg_global_name));
    }

    next_token__LilyPreparser(self);

    return 1;
}

LilyPreparserDecl *
preparse_module_body__LilyPreparser(LilyPreparser *self)
{
    // <module_body> = <module_decl> [ <module_body> ]*
    // <module_decl> = <constant_decl> | <error_decl> | <fun_decl> |
    //                 <module_decl> | <object_decl> | <type_decl>
    location_decl = clone__Location(&self->current->location); // Save location.
    visibility_decl = LILY_VISIBILITY_PRIVATE; // Default visibility is private.

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            visibility_decl = LILY_VISIBILITY_PUBLIC;

            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_VAL:
                    return preparse_constant__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_ERROR:
                    return preparse_error__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_TYPE:
                    return preparse_type__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_MODULE:
                    return preparse_module__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_FUN:
                    return preparse_fun__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_CLOSE:
                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_object:
                            return preparse_object__LilyPreparser(self, true);
                        default:
                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            goto unexpected_token;
                    }
                case LILY_TOKEN_KIND_KEYWORD_object:
                    return preparse_object__LilyPreparser(self, false);
                default: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                                 simple_lily_error,
                                                 self->file,
                                                 &self->current->location,
                                                 NEW_VARIANT(LilyError,
                                                             unexpected_token,
                                                             current_s->buffer),
                                                 NULL,
                                                 NULL,
                                                 NULL),
                                     &self->count_error);

                    visibility_decl = LILY_VISIBILITY_PRIVATE;

                    FREE(String, current_s);

                    return NULL;
                }
            }
        case LILY_TOKEN_KIND_KEYWORD_CLOSE:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_object:
                    return preparse_object__LilyPreparser(self, true);
                default:
                    goto unexpected_token;
            }
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
            return preparse_type__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
            return preparse_error__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
            return preparse_module__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return preparse_fun__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_object:
            return preparse_object__LilyPreparser(self, false);
        case LILY_TOKEN_KIND_KEYWORD_USE:
            return preparse_use__LilyPreparser(self);
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
        case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_BANG:
                        return preparse_macro_expand__LilyPreparser(self);
                    default:
                        goto unexpected_token;
                }
            } else {
                goto unexpected_token;
            }
        }
        default: {
        unexpected_token: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                NULL),
              &self->count_error);

            FREE(String, current_s);

            next_token__LilyPreparser(self);

            return NULL;
        }
        }
    }
}

LilyPreparserDecl *
preparse_module__LilyPreparser(LilyPreparser *self)
{
    // module <name> = <module_body> end
    // <name> = <identifier_normal> [ . <identifier_normal> ]*
    // <module_body> = <module_decl> [ <module_body> ]*
    // <module_decl> = <constant_decl> | <error_decl> | <fun_decl> |
    //                 <module_decl> | <object_decl> | <type_decl>
    next_token__LilyPreparser(self); // skip `module` keyword

    String *name = NULL;
    Vec *body = NEW(Vec);

    // 1. Get the name
parse_module_name: {
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            if (name) {
                push__String(name, '.');
                push_str__String(name,
                                 self->current->identifier_normal->buffer);
                next_token__LilyPreparser(self);
            } else {
                name = clone__String(self->current->identifier_normal);
                next_token__LilyPreparser(self);
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_DOT:
                    next_token__LilyPreparser(self);
                    goto parse_module_name;
                default:
                    break;
            }

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_MODULE_IDENTIFIER),
                NULL,
                init__Vec(1,
                          from__String("here are some examples of module "
                                       "identifier: `X`, `X.Y.Z`")),
                NULL),
              &self->count_error);

            name = from__String("__error__");
    }
}

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `=`")),
              &self->count_error);

            FREE(String, current_s);

            break;
        }
    }

    Location module_location = location_decl;
    enum LilyVisibility module_visibility = visibility_decl;

    // 2. Preparse body.
    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        LilyPreparserDecl *decl = preparse_module_body__LilyPreparser(self);

        if (decl) {
            push__Vec(body, decl);
        } else {
            go_to_next_block__LilyPreparser(self);
        }
    }

    END_LOCATION(&module_location, self->current->location);

    if (self->current->kind == LILY_TOKEN_KIND_EOF) {
        FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserDecl);
        FREE(Vec, body);
        FREE(String, name);

        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->file,
            &module_location,
            NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
            NULL,
            NULL,
            from__String("expected `end` keyword to close the module")),
          &self->count_error);

        return NULL;
    } else {
        next_token__LilyPreparser(self);
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       module,
                       module_location,
                       NEW(LilyPreparserModule, name, body, module_visibility));
}

void
preparse_test__LilyPreparser(LilyPreparser *self)
{
    TODO("preparse test");
}

bool
must_close_if_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_ELIF ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_ELSE ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_else_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_for_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_match_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_while_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_try_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_CATCH ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_catch_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_defer_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_SEMICOLON ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_match_case__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_SEMICOLON ||
           self->current->kind == LILY_TOKEN_KIND_EOF ||
           self->current->kind == LILY_TOKEN_KIND_KEYWORD_END;
}

bool
must_close_lambda_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_SEMICOLON ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_basic_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_basic_brace_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_R_BRACE;
}

bool
must_close_unsafe_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_fun_block__LilyPreparser(LilyPreparser *self)
{
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
        case LILY_TOKEN_KIND_KEYWORD_CLASS:
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
        case LILY_TOKEN_KIND_KEYWORD_END:
        case LILY_TOKEN_KIND_KEYWORD_ERROR:
        case LILY_TOKEN_KIND_KEYWORD_IMPL:
        case LILY_TOKEN_KIND_KEYWORD_IMPORT:
        case LILY_TOKEN_KIND_KEYWORD_INCLUDE:
        case LILY_TOKEN_KIND_KEYWORD_INHERIT:
        case LILY_TOKEN_KIND_KEYWORD_MACRO:
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
        case LILY_TOKEN_KIND_KEYWORD_object:
        case LILY_TOKEN_KIND_KEYWORD_OBJECT:
        case LILY_TOKEN_KIND_KEYWORD_PACKAGE:
        case LILY_TOKEN_KIND_KEYWORD_PUB:
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
        case LILY_TOKEN_KIND_KEYWORD_REQ:
        case LILY_TOKEN_KIND_KEYWORD_TEST:
        case LILY_TOKEN_KIND_KEYWORD_TRAIT:
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
        case LILY_TOKEN_KIND_KEYWORD_WHEN:
        case LILY_TOKEN_KIND_EOF:
            return true;
        default:
            return false;
    }
}

LilyPreparserFunBodyItem *
preparse_macro_expand_for_fun__LilyPreparser(LilyPreparser *self)
{
    // <macro_expand> = <identifier_normal> ! [ (<macro_expand_args>) ] ;
    // <macro_expand_args> = <macro_expand_arg> [ , <macro_expand_arg> ]*
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserFunBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserFunBodyItem,
                      macro_expand,
                      decl->macro_expand,
                      decl->location);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

LilyPreparserFunBodyItem *
preparse_asm_block__LilyPreparser(LilyPreparser *self)
{
    // asm ( <asm_block> ) ;
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `asm` keyword

    // 1. Preparse asm block
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN: {
            Vec *params = preparse_paren_with_comma_sep__LilyPreparser(
              self); // Vec<Vec<LilyToken* (&)>*>*

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_SEMICOLON:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyPreparser(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `;`")),
                      &self->count_error);

                    break;
            }

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_asm,
                               NEW(LilyPreparserFunBodyItemStmtAsm, params),
                               location);
        }
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `(`")),
              &self->count_error);

            return NULL;
    }
}

LilyPreparserFunBodyItem *
preparse_await_block__LilyPreparser(LilyPreparser *self)
{
    // await <await_block> ;
    // <await_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `await` keyword

    // 1. Preparse await block
    Vec *expr = NEW(Vec); // Vec<LilyToken*>*

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    if (expr->len == 0) {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      self->file,
                      &self->current->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_EXPRESSION),
                      NULL,
                      NULL,
                      from__String("unexpected `;`")),
          &self->count_error);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_await,
                               NEW(LilyPreparserFunBodyItemStmtAwait, expr),
                               location);
        }
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `;` to close return block")),
              &self->count_error);

            FREE(Vec, expr);

            return NULL;
        }
    }
}

LilyPreparserFunBodyItem *
preparse_break_block__LilyPreparser(LilyPreparser *self)
{
    // break [ <identifier_normal> ] ;
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `break` keyword

    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON: {
        finish_to_preparse_break_stmt: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_break,
                               NEW(LilyPreparserFunBodyItemStmtBreak, name),
                               location);
        }
        }
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
            name = clone__String(self->current->identifier_normal);

            next_token__LilyPreparser(self);

            goto finish_to_preparse_break_stmt;
        }
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);

            if (name) {
                FREE(String, name);
            }

            return NULL;
        }
    }
}

LilyPreparserFunBodyItem *
preparse_defer_block__LilyPreparser(LilyPreparser *self)
{
    // defer <block> ;
    // <block> = <exprs> | <lambda> | <stmt_for> | <stmt_match> | <stmt_if> |
    //           <stmt_try> | <stmt_unsafe> | <stmt_while> | <stmt_block>
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `defer` keyword

    LilyPreparserFunBodyItem *item = preparse_block__LilyPreparser(
      self, &must_close_defer_block__LilyPreparser, false);

    if (!item) {
        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            // Show a warning except for an element that does not end with a
            // `;`.
            switch (item->kind) {
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_LAMBDA:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_UNSAFE:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_WHILE:
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_warning,
                        self->file,
                        &self->current->location,
                        NEW(LilyWarning, LILY_WARNING_KIND_UNUSED_SEMICOLON),
                        NULL,
                        NULL,
                        NULL),
                      &self->count_warning);
            }

            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default: {
            LilyToken *prev = get__Vec(self->tokens, self->position - 1);

            switch (prev->kind) {
                case LILY_TOKEN_KIND_SEMICOLON:
                    END_LOCATION(&location, prev->location);
                    break;
                default: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                                 simple_lily_error,
                                                 self->file,
                                                 &self->current->location,
                                                 NEW_VARIANT(LilyError,
                                                             unexpected_token,
                                                             current_s->buffer),
                                                 NULL,
                                                 NULL,
                                                 from__String("expected `;`")),
                                     &self->count_error);

                    FREE(String, current_s);

                    return NULL;
                }
            }
        }
    }

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_defer,
                       NEW(LilyPreparserFunBodyItemStmtDefer, item),
                       location);
}

LilyPreparserFunBodyItem *
preparse_drop_block__LilyPreparser(LilyPreparser *self)
{
    // drop <tokens> ;
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `drop` keyword

    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(expr, must_preparse_exprs(self));

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);
            FREE(Vec, expr);

            return NULL;
        }
    }

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_drop,
                       NEW(LilyPreparserFunBodyItemStmtDrop, expr),
                       location);
}

LilyPreparserFunBodyItem *
preparse_if_block__LilyPreparser(LilyPreparser *self)
{
    // if <if_expr> [ :> <if_capture> ] do <if_block> [ <elif_block> ]* [ else
    // <else_block> ] end <if_expr> = <tokens> <if_capture> = <tokens>
    // <if_block> = <tokens>
    // <elif_block> = elif <elif_expr> [ :> <elif_capture> ] do <elif_block>
    // <elif_expr> = <tokens>
    // <elif_capture> = <tokens>
    // <elif_block> = <tokens>
    // <else_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;
    Vec *if_expr = NEW(Vec); // Vec<LilyToken* (&)>*
    Vec *if_capture = NULL;  // Vec<LilyToken* (&)>*?
    Vec *if_block = NULL;

    next_token__LilyPreparser(self); // skip `if` keyword

    // 1. Preparse `if` expression
    PREPARSE_UNTIL(if_expr,
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_R_SHIFT &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
        if_expected_do: {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                NULL,
                NULL,
                from__String(
                  "expected `do` keyword after `if` statement expression")),
              &self->count_error);

            FREE(Vec, if_expr);

            if (if_capture) {
                FREE(Vec, if_capture);
            }

            return NULL;
        }
        case LILY_TOKEN_KIND_COLON_R_SHIFT:
            // Preparse if capture (optional)
            if_capture = NEW(Vec);

            next_token__LilyPreparser(self); // skip `:>`

            PREPARSE_UNTIL(if_capture,
                           self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_DO:
                    goto if_do;
                default:
                    goto if_expected_do;
            }
        case LILY_TOKEN_KIND_KEYWORD_DO:
        if_do: {
            next_token__LilyPreparser(self);

            break;
        }
        default:
            UNREACHABLE("this situation is impossible");
    }

    // 2. Preparse `if` block
    if_block =
      preparse_body__LilyPreparser(self, &must_close_if_block__LilyPreparser);

    if (!if_block) {
        FREE(Vec, if_expr);

        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_ELIF: {
            // 3(A). Preparse `elif` statement. Similar process than `if`
            // statement preparsing.

            Vec *elif_exprs = NEW(Vec);    // Vec<Vec<LilyToken* (&)>*>*
            Vec *elif_captures = NEW(Vec); // Vec<Vec<LilyToken* (&)>*?>*
            Vec *elif_blocks =
              NEW(Vec); // Vec<Vec<LilyPreparserFunBodyItem*>*>*

            while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_ELSE &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                Vec *elif_expr = NEW(Vec); // Vec<LilyToken* (&)>*
                Vec *elif_capture = NULL;  // Vec<LilyToken* (&)>*?
                Vec *elif_block = NULL;    // Vec<LilyPreparserFunBodyItem*>*

                next_token__LilyPreparser(self); // skip `elif` keyword

                // 3(1A). Preparse `elif` expression.
                PREPARSE_UNTIL(
                  elif_expr,
                  self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                    self->current->kind != LILY_TOKEN_KIND_COLON_R_SHIFT &&
                    self->current->kind != LILY_TOKEN_KIND_EOF);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_EOF:
                    elif_expected_do: {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                            NULL,
                            NULL,
                            from__String("expected `do` keyword after `elif` "
                                         "statement expression")),
                          &self->count_error);

                        goto clean_up_elif;
                    }
                    case LILY_TOKEN_KIND_COLON_R_SHIFT:
                        // Preparse elif capture (optional)
                        elif_capture = NEW(Vec);

                        next_token__LilyPreparser(self); // skip `:>`

                        PREPARSE_UNTIL(
                          elif_capture,
                          self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                            self->current->kind != LILY_TOKEN_KIND_EOF);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_DO:
                                goto elif_do;
                            default:
                                goto elif_expected_do;
                        }
                    case LILY_TOKEN_KIND_KEYWORD_DO:
                    elif_do: {
                        next_token__LilyPreparser(self);

                        break;
                    }
                    default:
                        UNREACHABLE("this situation is impossible");
                }

                // 3(2A). Preparse `elif` block.
                elif_block = preparse_body__LilyPreparser(
                  self, &must_close_if_block__LilyPreparser);

                if (!elif_block) {
                    // Clean up allocations

                clean_up_elif: {
                    FREE(Vec, if_expr);

                    FREE_BUFFER_ITEMS(if_block->buffer,
                                      if_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, if_block);

                    FREE_BUFFER_ITEMS(elif_exprs->buffer, elif_exprs->len, Vec);
                    FREE(Vec, elif_exprs);

                    FREE_BUFFER_ITEMS_2(elif_blocks->buffer,
                                        elif_blocks->len,
                                        LilyPreparserFunBodyItem);
                    FREE(Vec, elif_blocks);

                    return NULL;
                }
                }

                push__Vec(elif_exprs, elif_expr);
                push__Vec(elif_captures, elif_capture);
                push__Vec(elif_blocks, elif_block);
            }

            // 3(3A). Preparse `else` statement if it's possible.
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_ELSE: {
                    Vec *else_block = NULL;

                    next_token__LilyPreparser(self);

                    else_block = preparse_body__LilyPreparser(
                      self, &must_close_else_block__LilyPreparser);

                    if (!else_block) {
                        // Clean up allocations
                        goto clean_up_elif;
                    }

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_END: {
                            END_LOCATION(&location, self->current->location);
                            next_token__LilyPreparser(self);

                            return NEW_VARIANT(
                              LilyPreparserFunBodyItem,
                              stmt_if,
                              NEW(LilyPreparserFunBodyItemStmtIf,
                                  if_expr,
                                  if_capture,
                                  if_block,
                                  elif_exprs,
                                  elif_captures,
                                  elif_blocks,
                                  else_block),
                              location);
                        }

                        default:
                            UNREACHABLE("this way is impossible");
                    }

                    break;
                }

                case LILY_TOKEN_KIND_KEYWORD_END: {
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyPreparser(self); // skip `end` keyword

                    return NEW_VARIANT(LilyPreparserFunBodyItem,
                                       stmt_if,
                                       NEW(LilyPreparserFunBodyItemStmtIf,
                                           if_expr,
                                           if_capture,
                                           if_block,
                                           elif_exprs,
                                           elif_captures,
                                           elif_blocks,
                                           NULL),
                                       location);
                }

                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String(
                          "expected `end` keyword to close `elif` block")),
                      &self->count_error);

                    // Clean up allocations

                    FREE(Vec, if_expr);

                    FREE_BUFFER_ITEMS(if_block->buffer,
                                      if_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, if_block);

                    FREE_BUFFER_ITEMS(elif_exprs->buffer, elif_exprs->len, Vec);
                    FREE(Vec, elif_exprs);

                    FREE_BUFFER_ITEMS_2(elif_blocks->buffer,
                                        elif_blocks->len,
                                        LilyPreparserFunBodyItem);
                    FREE(Vec, elif_blocks);

                    return NULL;

                default:
                    UNREACHABLE("this way is impossible");
            }

            break;
        }

        case LILY_TOKEN_KIND_KEYWORD_ELSE: {
            // 3(B). Preparse `else` statement.

            next_token__LilyPreparser(self); // skip `else` keyword

            Vec *else_block = preparse_body__LilyPreparser(
              self, &must_close_else_block__LilyPreparser);

            if (!else_block) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                    NULL,
                    NULL,
                    from__String(
                      "expected `end` keyword to close the `else` block")),
                  &self->count_error);

                // Clean up allocations

                FREE(Vec, if_expr);

                FREE_BUFFER_ITEMS(
                  if_block->buffer, if_block->len, LilyPreparserFunBodyItem);
                FREE(Vec, if_block);

                return NULL;
            }

            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self); // skip `end` keyword

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_if,
                               NEW(LilyPreparserFunBodyItemStmtIf,
                                   if_expr,
                                   if_capture,
                                   if_block,
                                   NULL,
                                   NULL,
                                   NULL,
                                   else_block),
                               location);
        }

        case LILY_TOKEN_KIND_KEYWORD_END: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self); // skip `end` keyword

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_if,
                               NEW(LilyPreparserFunBodyItemStmtIf,
                                   if_expr,
                                   if_capture,
                                   if_block,
                                   NULL,
                                   NULL,
                                   NULL,
                                   NULL),
                               location);
        }

        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                NULL,
                NULL,
                from__String("expected `end` keyword to close the `if` block")),
              &self->count_error);

            // Clean up allocations

            FREE(Vec, if_expr);

            FREE_BUFFER_ITEMS(
              if_block->buffer, if_block->len, LilyPreparserFunBodyItem);
            FREE(Vec, if_block);

            return NULL;

        default:
            UNREACHABLE("this way is impossible");
    }

    return NULL;
}

LilyPreparserFunBodyItem *
preparse_for_block__LilyPreparser(LilyPreparser *self)
{
    // for(<for_name>) <for_expr> [ :> <for_capture> ] do <for_block> end
    // <for_name> = <identifier_normal>
    // <for_expr> = <tokens>
    // <for_capture> = <tokens>
    // <for_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `for` keyword

    // 1. Preparse name of `for` block.
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            next_token__LilyPreparser(self); // skip `(`

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    name = clone__String(self->current->identifier_normal);

                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_R_PAREN:
                            next_token__LilyPreparser(self); // skip `)`
                            break;
                        default: {
                            String *current_s =
                              to_string__LilyToken(self->current);

                            emit__Diagnostic(
                              NEW_VARIANT(Diagnostic,
                                          simple_lily_error,
                                          self->file,
                                          &self->current->location,
                                          NEW_VARIANT(LilyError,
                                                      unexpected_token,
                                                      current_s->buffer),
                                          NULL,
                                          NULL,
                                          from__String("expected `)`")),
                              &self->count_error);

                            FREE(String, current_s);
                        }
                    }

                    break;
                default: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(Diagnostic,
                                  simple_lily_error,
                                  self->file,
                                  &self->current->location,
                                  NEW_VARIANT(LilyError,
                                              unexpected_token,
                                              current_s->buffer),
                                  NULL,
                                  NULL,
                                  from__String("expected identifier normal")),
                      &self->count_error);

                    FREE(String, current_s);

                    next_token__LilyPreparser(self);
                }
            }

            break;
        default:
            break;
    }

    // 2. Preparse for expression.
    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*
    Vec *capture = NULL;  // Vec<LilyToken* (&)>*?

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_R_SHIFT &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
        for_expected_do: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `do` keyword after `for` "
                                       "statement expression")),
              &self->count_error);

            FREE(Vec, expr);

            if (capture) {
                FREE(Vec, capture);
            }

            return NULL;
        }
        case LILY_TOKEN_KIND_COLON_R_SHIFT:
            // Preparse for capture (optional)
            capture = NEW(Vec);

            next_token__LilyPreparser(self); // skip `:>`

            PREPARSE_UNTIL(capture,
                           self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_DO:
                    goto for_do;
                default:
                    goto for_expected_do;
            }
        case LILY_TOKEN_KIND_KEYWORD_DO:
        for_do: {
            next_token__LilyPreparser(self);

            break;
        }
        default:
            UNREACHABLE("this situation is impossible");
    }

    // 3. Preparse `for` block
    Vec *block =
      preparse_body__LilyPreparser(self, &must_close_for_block__LilyPreparser);

    if (!block) {
        FREE(Vec, expr);

        return NULL;
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `end` keyword

    return NEW_VARIANT(
      LilyPreparserFunBodyItem,
      stmt_for,
      NEW(LilyPreparserFunBodyItemStmtFor, name, expr, capture, block),
      location);
}

LilyPreparserFunBodyItem *
preparse_lambda__LilyPreparser(LilyPreparser *self)
{
    // <lambda> <lambda_name> [ (<lambda_params>) ] [ <lambda_return_data_type>
    // ] -> <lambda_body> ; [ \( <lambda_params_call>) ; ]
    // <lambda_name> = <identifier_normal> | <identifier_string>
    // <lambda_params> = <tokens> [ , <tokens> ]*
    // <lambda_return_data_type> = <tokens>
    // <lambda_body> = <tokens>
    // <lambda_params_call> = <tokens> [ , <tokens> ]*
    // <tokens> = <token> [ <token> ]*
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self); // skip `lambda` keyword

    // 1. Get the name of the lambda.
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            name = clone__String(self->current->identifier_string);
            next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

    // 2. Preparse params.
    Vec *params = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            params = preparse_paren_with_comma_sep__LilyPreparser(self);
            break;
        default:
            break;
    }

    // 3. Preparse return data type.
    Vec *return_data_type = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_ARROW:
            break;
        default:
            return_data_type = NEW(Vec);

            PREPARSE_UNTIL(return_data_type,
                           self->current->kind != LILY_TOKEN_KIND_ARROW &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String("expected `->`")),
                      &self->count_error);

                    // Clean up allocations
                    FREE(Vec, return_data_type);

                    return NULL;
                default:
                    next_token__LilyPreparser(self);
            }

            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_ARROW:
            next_token__LilyPreparser(self);

            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `->`")),
              &self->count_error);

            FREE(String, current_s);
        }
    }

    // 4. Preparse fun body item.
    LilyPreparserFunBodyItem *item = preparse_block__LilyPreparser(
      self, &must_close_lambda_block__LilyPreparser, false);

    if (!item) {
        if (params) {
            FREE_BUFFER_ITEMS(params->buffer, params->len, Vec);
            FREE(Vec, params);
        }

        if (return_data_type) {
            FREE(Vec, return_data_type);
        }

        return NULL;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            switch (item->kind) {
                case LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS:
                    next_token__LilyPreparser(self);
                    break;
                default: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(Diagnostic,
                                  simple_lily_error,
                                  self->file,
                                  &self->current->location,
                                  NEW_VARIANT(LilyError,
                                              unexpected_token,
                                              current_s->buffer),
                                  NULL,
                                  NULL,
                                  from__String("you may have forgotten `\\`")),
                      &self->count_error);

                    FREE(String, current_s);

                    next_token__LilyPreparser(self);

                    break;
                }
            }
        default:
            break;
    }

    // 5. Preparse params call
    Vec *params_call = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_BACKSLASH:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_PAREN:
                    params_call =
                      preparse_paren_with_comma_sep__LilyPreparser(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `(`")),
                      &self->count_error);
            }

            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

    END_LOCATION(&location, item->location);

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       lambda,
                       NEW(LilyPreparserFunBodyItemLambda,
                           name,
                           params,
                           return_data_type,
                           item,
                           params_call),
                       location);
}

LilyPreparserFunBodyItem *
preparse_while_block__LilyPreparser(LilyPreparser *self)
{
    // while <while_expr> do <while_block> end
    // <while_expr> = <tokens>
    // <while_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `while` keyword

    // 1. Preparse while expression
    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `do` keyword after "
                                       "`while` statement expression")),
              &self->count_error);

            FREE(Vec, expr);

            return NULL;

        default:
            next_token__LilyPreparser(self);
    }

    // 2. Preparse `while` block
    Vec *block = preparse_body__LilyPreparser(
      self, &must_close_while_block__LilyPreparser);

    if (!block) {
        FREE(Vec, expr);

        return NULL;
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `end` keyword

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_while,
                       NEW(LilyPreparserFunBodyItemStmtWhile, expr, block),
                       location);
}

LilyPreparserFunBodyItem *
preparse_raise_block__LilyPreparser(LilyPreparser *self)
{
    // raise <raise_expr> ;
    // <raise_expr> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `raise` keyword

    Vec *expr = NEW(Vec);

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_raise,
                               NEW(LilyPreparserFunBodyItemStmtRaise, expr),
                               location);
        }
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `;` to close raise block")),
              &self->count_error);

            FREE(Vec, expr);

            return NULL;
        }
    }
}

LilyPreparserFunBodyItem *
preparse_return_block__LilyPreparser(LilyPreparser *self)
{
    // return [ <return_expr> ] ;
    // <return_expr> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `return` keyword

    // 1. Preparse return block
    Vec *expr = NULL; // Vec<LilyToken* (&)>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_return,
                               NEW(LilyPreparserFunBodyItemStmtReturn, NULL),
                               location);
        }
        default:
            expr = NEW(Vec);

            PREPARSE_UNTIL(expr,
                           self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_SEMICOLON: {
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyPreparser(self);

                    return NEW_VARIANT(
                      LilyPreparserFunBodyItem,
                      stmt_return,
                      NEW(LilyPreparserFunBodyItemStmtReturn, expr),
                      location);
                }
                default: {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String("expected `;` to close return block")),
                      &self->count_error);

                    FREE(Vec, expr);

                    return NULL;
                }
            }
    }
}

LilyPreparserFunBodyItem *
preparse_try_block__LilyPreparser(LilyPreparser *self)
{
    // try do <try_block> [ catch <catch_expr> do <catch_block> ] end
    // <try_block> = <tokens>
    // <catch_expr> = <tokens>
    // <catch_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    jump__LilyPreparser(self, 2); // skip `try` and `do` keyword

    // 1. Preparse try block
    Vec *try_block =
      preparse_body__LilyPreparser(self, &must_close_try_block__LilyPreparser);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self); // skip `end` keyword

            return NEW_VARIANT(
              LilyPreparserFunBodyItem,
              stmt_try,
              NEW(LilyPreparserFunBodyItemStmtTry, try_block, NULL, NULL),
              location);
        }

        case LILY_TOKEN_KIND_KEYWORD_CATCH: {
            next_token__LilyPreparser(self);

            // 2. Preparse catch expression
            Vec *catch_expr = NULL; // Vec<LilyToken* (&)>*?

            if (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO) {
                catch_expr = NEW(Vec);

                PREPARSE_UNTIL(catch_expr,
                               self->current->kind !=
                                   LILY_TOKEN_KIND_KEYWORD_DO &&
                                 self->current->kind != LILY_TOKEN_KIND_EOF);
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_DO:
                    next_token__LilyPreparser(self);
                    break;
                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String("expected `do` keyword after `catch` "
                                     "statement expression")),
                      &self->count_error);

                    // Clean up allocations

                    FREE(Vec, catch_expr);

                    FREE_BUFFER_ITEMS(try_block->buffer,
                                      try_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, try_block);

                    return NULL;
                default:
                    UNREACHABLE("this way is impossible");
            }

            // 2. Preparse catch block
            Vec *catch_block = preparse_body__LilyPreparser(
              self, &must_close_catch_block__LilyPreparser);

            if (!catch_block) {
                // Clean up allocations

                FREE_BUFFER_ITEMS(
                  try_block->buffer, try_block->len, LilyPreparserFunBodyItem);
                FREE(Vec, try_block);

                FREE(Vec, catch_expr);

                return NULL;
            }

            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self); // skip `end` keyword

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_try,
                               NEW(LilyPreparserFunBodyItemStmtTry,
                                   try_block,
                                   catch_expr,
                                   catch_block),
                               location);
        }

        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `do` keyword after "
                                       "`while` statement expression")),
              &self->count_error);

            // Clean up allocations

            FREE_BUFFER_ITEMS(
              try_block->buffer, try_block->len, LilyPreparserFunBodyItem);
            FREE(Vec, try_block);

            return NULL;

        default:
            UNREACHABLE("this way is impossible");
    }

    return NULL;
}

LilyPreparserFunBodyItem *
preparse_match_block__LilyPreparser(LilyPreparser *self)
{
    // match <match_expr> do <match_block> end
    // <match_expr> = <tokens>
    // <match_block> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location_match = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `match` keyword

    // 1. Preparse `match` expression
    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_DO:
            next_token__LilyPreparser(self);
            break;

        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `do` keyword after "
                                       "`match` statement expression")),
              &self->count_error);

            FREE(Vec, expr);

            return NULL;

        default:
            UNREACHABLE("this way is impossible");
    }

    Vec *patterns = NEW(Vec);      // Vec<Vec<LilyToken* (&)>*>*
    Vec *pattern_conds = NEW(Vec); // Vec<Vec<LilyToken* (&)>*?>*
    Vec *blocks = NEW(Vec);        // Vec<LilyPreparserFunBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        // 2. Preparse pattern
        Vec *pattern = NEW(Vec);  // Vec<LilyToken* (&)>*
        Vec *pattern_cond = NULL; // Vec<LilyToken* (&)>*?

        PREPARSE_UNTIL(pattern,
                       self->current->kind != LILY_TOKEN_KIND_FAT_ARROW &&
                         self->current->kind != LILY_TOKEN_KIND_INTERROGATION &&
                         self->current->kind != LILY_TOKEN_KIND_EOF);

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_INTERROGATION:
                next_token__LilyPreparser(self);

                pattern_cond = NEW(Vec);

                PREPARSE_UNTIL(pattern_cond,
                               self->current->kind !=
                                   LILY_TOKEN_KIND_FAT_ARROW &&
                                 self->current->kind != LILY_TOKEN_KIND_EOF);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_FAT_ARROW:
                        next_token__LilyPreparser(self);
                        break;
                    case LILY_TOKEN_KIND_EOF:
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                            NULL,
                            NULL,
                            from__String(
                              "expected `=>` after pattern condition")),
                          &self->count_error);

                        // Clean up allocations

                    clean_up_pattern: {
                        FREE(Vec, expr);

                        FREE_BUFFER_ITEMS(patterns->buffer, patterns->len, Vec);
                        FREE(Vec, patterns);

                        for (Usize i = 0; i < pattern_conds->len; ++i) {
                            Vec *item = get__Vec(pattern_conds, i);

                            if (item) {
                                FREE(Vec, item);
                            }
                        }

                        FREE(Vec, pattern_conds);

                        FREE_BUFFER_ITEMS(blocks->buffer,
                                          blocks->len,
                                          LilyPreparserFunBodyItem);
                        FREE(Vec, blocks);

                        FREE(Vec, pattern);

                        if (pattern_cond) {
                            FREE(Vec, pattern_cond);
                        }

                        return NULL;
                    }
                    default:
                        UNREACHABLE("this way is impossible");
                }

                break;

            case LILY_TOKEN_KIND_FAT_ARROW:
                next_token__LilyPreparser(self);
                break;

            case LILY_TOKEN_KIND_EOF:
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                              NULL,
                              NULL,
                              from__String("expected `=>` after pattern")),
                  &self->count_error);

                // Clean up allocations

                goto clean_up_pattern;

            default:
                UNREACHABLE("this way is impossible");
        }

        // 3. Preparse block
        LilyPreparserFunBodyItem *block = preparse_block__LilyPreparser(
          self, &must_close_match_case__LilyPreparser, true);

        if (block) {
            if (block->kind == LILY_PREPARSER_FUN_BODY_ITEM_KIND_EXPRS) {
                switch (
                  CAST(LilyToken *, get__Vec(self->tokens, self->position - 1))
                    ->kind) {
                    case LILY_TOKEN_KIND_SEMICOLON:
                        break;
                    default:
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                            NULL,
                            NULL,
                            from__String("expected `;`")),
                          &self->count_error);
                }
            } else {
                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_SEMICOLON:
                        emit__Diagnostic(
                          NEW_VARIANT(Diagnostic,
                                      simple_lily_warning,
                                      self->file,
                                      &self->current->location,
                                      NEW(LilyWarning,
                                          LILY_WARNING_KIND_UNUSED_SEMICOLON),
                                      NULL,
                                      NULL,
                                      NULL),
                          &self->count_warning);

                        next_token__LilyPreparser(self);
                    default:
                        break;
                }
            }

            push__Vec(blocks, block);
        }

        push__Vec(patterns, pattern);
        push__Vec(pattern_conds, pattern_cond);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location_match, self->current->location);
            next_token__LilyPreparser(self);

            break;

        default:
            break;
    }

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_match,
                       NEW(LilyPreparserFunBodyItemStmtMatch,
                           expr,
                           patterns,
                           pattern_conds,
                           blocks),
                       location_match);
}

LilyPreparserFunBodyItem *
preparse_next_block__LilyPreparser(LilyPreparser *self)
{
    // next [ <next_name> ] ;
    // <next_name> = <identifier_normal>
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `next` keyword

    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON: {
        finish_to_preparse_break_stmt: {
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               stmt_next,
                               NEW(LilyPreparserFunBodyItemStmtNext, name),
                               location);
        }
        }
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
            name = clone__String(self->current->identifier_normal);

            next_token__LilyPreparser(self);

            goto finish_to_preparse_break_stmt;
        }
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);

            if (name) {
                FREE(String, name);
            }

            return NULL;
        }
    }
}

LilyPreparserFunBodyItem *
preparse_basic_block__LilyPreparser(LilyPreparser *self)
{
    // begin <basic_block_body> end
    // <basic_block_body> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `begin` keyword

    Vec *basic_block_body = preparse_body__LilyPreparser(
      self, &must_close_basic_block__LilyPreparser);

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `end` keyword

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_block,
                       NEW(LilyPreparserFunBodyItemStmtBlock, basic_block_body),
                       location);
}

LilyPreparserFunBodyItem *
preparse_basic_brace_block__LilyPreparser(LilyPreparser *self)
{
    // @{ <basic_brace_block_body> }
    // <basic_brace_block_body> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;
    Vec *body = NEW(Vec); // Vec<LilyPreparserFunBodyItem*>*

    while (!must_close_basic_brace_block__LilyPreparser(self)) {
        LilyPreparserFunBodyItem *item = preparse_block__LilyPreparser(
          self, &must_close_basic_brace_block__LilyPreparser, true);

        if (item) {
            push__Vec(body, item);
        }
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `}`

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_block,
                       NEW(LilyPreparserFunBodyItemStmtBlock, body),
                       location);
}

LilyPreparserFunBodyItem *
preparse_unsafe_block__LilyPreparser(LilyPreparser *self)
{
    // unsafe <unsafe_block_body> end
    // <unsafe_block_body> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = location_fun_body_item;
    Vec *body = NEW(Vec); // Vec<LilyPreparserFunBodyItem*>*

    // 1. Preparse unsafe block body
    while (!must_close_unsafe_block__LilyPreparser(self)) {
        LilyPreparserFunBodyItem *item = preparse_block__LilyPreparser(
          self, &must_close_unsafe_block__LilyPreparser, true);

        if (item) {
            push__Vec(body, item);
        }
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `end` keyword

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_unsafe,
                       NEW(LilyPreparserFunBodyItemStmtUnsafe, body),
                       location);
}

LilyPreparserFunBodyItem *
preparse_variable_block__LilyPreparser(LilyPreparser *self,
                                       bool is_mut,
                                       bool is_trace,
                                       bool is_ref,
                                       bool is_drop)
{
    // val <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // mut <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // drop: val <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // drop: mut <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // ref: val <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // ref: mut <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // trace: val <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // trace: mut <variable_name> [ <variable_data_type> ] := <variable_expr> ;
    // <variable_name> = <identifier_normal>
    // <variable_data_type> = <tokens>
    // <variable_expr> = <tokens>
    // <tokens> = <token> [ <token> ]*
    Location location = clone__Location(&self->current->location);
    Vec *data_type = NULL; // Vec<LilyToken* (&)>*?

    // 1. Get the name of the variable.
    GET_NAME(self, from__String("expected name of variable"));

    // 2. Get the data type of the variable (optional).
    if (self->current->kind != LILY_TOKEN_KIND_COLON_EQ) {
        data_type = NEW(Vec);

        PREPARSE_UNTIL(data_type,
                       self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
                         self->current->kind != LILY_TOKEN_KIND_EOF);

        if (self->current->kind == LILY_TOKEN_KIND_EOF) {
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                NULL,
                NULL,
                from__String(
                  "expected `:=` after the data type of the variable")),
              &self->count_error);

            FREE(String, name);
            FREE(Vec, data_type);

            return NULL;
        }
    }

    // 3. Get if `:=` token is present before expression.
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                NULL,
                NULL,
                from__String(
                  "expected `:=` before the expression of the variable")),
              &self->count_error);
    }

    // 4. Preparse expression.
    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;

        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                NULL,
                NULL,
                from__String("expected `;` to close the variable declaration")),
              &self->count_error);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(LilyPreparserFunBodyItem,
                       stmt_var,
                       NEW(LilyPreparserFunBodyItemStmtVariable,
                           name,
                           data_type,
                           expr,
                           is_mut,
                           is_trace,
                           is_ref,
                           is_drop),
                       location);
}

bool
must_preparse_exprs(LilyPreparser *self)
{
    LilyToken *peeked = peek_token__LilyPreparser(self, 1);

    if (peeked) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_AT:
                return peeked->kind != LILY_TOKEN_KIND_L_BRACE;
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            case LILY_TOKEN_KIND_KEYWORD_BREAK:
            case LILY_TOKEN_KIND_KEYWORD_FOR:
            case LILY_TOKEN_KIND_KEYWORD_FUN:
            case LILY_TOKEN_KIND_KEYWORD_IF:
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
            case LILY_TOKEN_KIND_KEYWORD_NEXT:
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
            case LILY_TOKEN_KIND_KEYWORD_MUT:
            case LILY_TOKEN_KIND_KEYWORD_VAL:
            case LILY_TOKEN_KIND_KEYWORD_RETURN:
            case LILY_TOKEN_KIND_EOF:
            case LILY_TOKEN_KIND_KEYWORD_END:
            case LILY_TOKEN_KIND_R_BRACE:
            case LILY_TOKEN_KIND_SEMICOLON:
                return false;
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                return peeked->kind != LILY_TOKEN_KIND_BANG;
            case LILY_TOKEN_KIND_KEYWORD_TRY:
                return peeked->kind != LILY_TOKEN_KIND_KEYWORD_DO;
            case LILY_TOKEN_KIND_KEYWORD_REF:
            case LILY_TOKEN_KIND_KEYWORD_TRACE:
            case LILY_TOKEN_KIND_KEYWORD_DROP:
                return peeked->kind != LILY_TOKEN_KIND_COLON;
            default:
                return true;
        }
    } else {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            case LILY_TOKEN_KIND_KEYWORD_BREAK:
            case LILY_TOKEN_KIND_KEYWORD_FOR:
            case LILY_TOKEN_KIND_KEYWORD_FUN:
            case LILY_TOKEN_KIND_KEYWORD_IF:
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
            case LILY_TOKEN_KIND_KEYWORD_TRY:
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
            case LILY_TOKEN_KIND_KEYWORD_MUT:
            case LILY_TOKEN_KIND_KEYWORD_NEXT:
            case LILY_TOKEN_KIND_KEYWORD_VAL:
            case LILY_TOKEN_KIND_KEYWORD_RETURN:
            case LILY_TOKEN_KIND_EOF:
            case LILY_TOKEN_KIND_KEYWORD_END:
            case LILY_TOKEN_KIND_R_BRACE:
            case LILY_TOKEN_KIND_SEMICOLON:
                return false;
            default:
                return true;
        }
    }
}

LilyPreparserFunBodyItem *
preparse_block__LilyPreparser(LilyPreparser *self,
                              bool (*must_close)(LilyPreparser *),
                              bool parse_semicolon)
{
    location_fun_body_item = clone__Location(&self->current->location);

    switch (self->current->kind) {
        /*
            asm(<expr>,...);
        */
        case LILY_TOKEN_KIND_KEYWORD_ASM:
            return preparse_asm_block__LilyPreparser(self);
        /*
            await <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_AWAIT:
            return preparse_await_block__LilyPreparser(self);
        /*
            @{
                <block>
            }
        */
        case LILY_TOKEN_KIND_AT: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_L_BRACE:
                        jump__LilyPreparser(self, 2);

                        return preparse_basic_brace_block__LilyPreparser(self);

                    default:
                        goto preparse_exprs;
                }
            } else {
                goto preparse_exprs;
            }

            break;
        }

        /*
            begin
                <block>
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            return preparse_basic_block__LilyPreparser(self);

        /*
            break <name>;
        */
        case LILY_TOKEN_KIND_KEYWORD_BREAK:
            return preparse_break_block__LilyPreparser(self);

        /*
           defer <expr|stmt>;
        */
        case LILY_TOKEN_KIND_KEYWORD_DEFER:
            return preparse_defer_block__LilyPreparser(self);

        /*
           drop: val <name> [data_type] := <expr>;
           drop: mut <name> [data_type] := <expr>;
           drop <expr|stmt>;
        */
        case LILY_TOKEN_KIND_KEYWORD_DROP: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_COLON:
                        jump__LilyPreparser(self, 2);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_VAL:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, false, false, true);
                            case LILY_TOKEN_KIND_KEYWORD_MUT:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, true, false, false, true);
                            default:
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String(
                                      "expected `val` or `mut` keyword")),
                                  &self->count_error);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, false, false, true);
                        }
                    default:
                        return preparse_drop_block__LilyPreparser(self);
                }
            }
        }

        /*
            for <expr> in <expr> do
                <block>
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_FOR:
            return preparse_for_block__LilyPreparser(self);

        /*
            fun [<name>][(<params>)] [<return_data_type>] -> <item> [;]
        */
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return preparse_lambda__LilyPreparser(self);

        /*
            if <expr> do
                <block>
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_IF:
            return preparse_if_block__LilyPreparser(self);

        /*
            match <expr> do
                <pattern> [? <expr>] => <stmt | expr;>
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_MATCH:
            return preparse_match_block__LilyPreparser(self);

        /*
           next <name>;
        */
        case LILY_TOKEN_KIND_KEYWORD_NEXT:
            return preparse_next_block__LilyPreparser(self);

        /*
          raise <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_RAISE:
            return preparse_raise_block__LilyPreparser(self);

        /*
            return <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_RETURN:
            return preparse_return_block__LilyPreparser(self);

        /*
            try do
                <block>
            [catch <expr> do <block>]
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_TRY: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_DO:
                        return preparse_try_block__LilyPreparser(self);
                    default:
                        goto preparse_exprs;
                }
            } else {
                goto preparse_exprs;
            }

            break;
        }

        /*
            while <expr> do
            end
        */
        case LILY_TOKEN_KIND_KEYWORD_WHILE:
            return preparse_while_block__LilyPreparser(self);

        /*
            ref: val <name> [data_type] := <expr>;
            ref: mut <name> [data_type] := <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_REF: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_COLON:
                        jump__LilyPreparser(self, 2);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_VAL:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, false, true, false);
                            case LILY_TOKEN_KIND_KEYWORD_MUT:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, true, false, true, false);
                            default:
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String(
                                      "expected `val` or `mut` keyword")),
                                  &self->count_error);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, false, true, false);
                        }
                    default:
                        goto preparse_exprs;
                }
            } else {
                goto preparse_exprs;
            }
        }

        /*
            mut <name> [data_type] := <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_MUT:
            next_token__LilyPreparser(self);

            return preparse_variable_block__LilyPreparser(
              self, true, false, false, false);

        /*
            trace: val <name> [data_type] := <expr>;
            trace: mut <name> [data_type] := <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_TRACE: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_COLON:
                        jump__LilyPreparser(self, 2);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_VAL:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, true, false, false);
                            case LILY_TOKEN_KIND_KEYWORD_MUT:
                                next_token__LilyPreparser(self);

                                return preparse_variable_block__LilyPreparser(
                                  self, true, true, false, false);
                            default:
                                emit__Diagnostic(
                                  NEW_VARIANT(
                                    Diagnostic,
                                    simple_lily_error,
                                    self->file,
                                    &self->current->location,
                                    NEW(LilyError,
                                        LILY_ERROR_KIND_EXPECTED_TOKEN),
                                    NULL,
                                    NULL,
                                    from__String(
                                      "expected `val` or `mut` keyword")),
                                  &self->count_error);

                                return preparse_variable_block__LilyPreparser(
                                  self, false, true, false, false);
                        }
                    default:
                        goto preparse_exprs;
                }
            } else {
                goto preparse_exprs;
            }
        }

        /*
          unsafe =
            <block>
          end
        */
        case LILY_TOKEN_KIND_KEYWORD_UNSAFE:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_EQ:
                    next_token__LilyPreparser(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `=`")),
                      &self->count_error);

                    break;
            }

            return preparse_unsafe_block__LilyPreparser(self);

        /*
            val <name> [data_type] := <expr>;
        */
        case LILY_TOKEN_KIND_KEYWORD_VAL:
            next_token__LilyPreparser(self);

            return preparse_variable_block__LilyPreparser(
              self, false, false, false, false);

        case LILY_TOKEN_KIND_SEMICOLON:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_warning,
                          self->file,
                          &self->current->location,
                          NEW(LilyWarning, LILY_WARNING_KIND_UNUSED_SEMICOLON),
                          NULL,
                          NULL,
                          NULL),
              &self->count_warning);

            next_token__LilyPreparser(self);

            return NULL;

        /*
            <name>!(<args>);
        */
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
        case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                if (peeked->kind == LILY_TOKEN_KIND_BANG) {
                    return preparse_macro_expand_for_fun__LilyPreparser(self);
                } else {
                    goto preparse_exprs;
                }
            } else {
                goto preparse_exprs;
            }
        }

        default:
        preparse_exprs: {
            Location location = clone__Location(&self->current->location);
            Vec *exprs = NEW(Vec); // Vec<LilyToken* (&)>*

            PREPARSE_UNTIL(exprs,
                           must_preparse_exprs(self) && !must_close(self));

            if (parse_semicolon) {
                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_SEMICOLON:
                        next_token__LilyPreparser(self);

                        break;
                    default:
                        break;
                }
            }

            LilyToken *previous = NULL;

            if (self->current->kind == LILY_TOKEN_KIND_EOF) {
                previous = self->tokens->buffer[self->position];
            } else {
                previous = self->tokens->buffer[self->position - 1];
            }

            END_LOCATION(&location, previous->location);

            return NEW_VARIANT(LilyPreparserFunBodyItem,
                               exprs,
                               NEW(LilyPreparserFunBodyItemExprs, exprs),
                               location);
        }
    }
}

Vec *
preparse_body__LilyPreparser(LilyPreparser *self,
                             bool (*must_close)(LilyPreparser *))
{
    Vec *body = NEW(Vec); // Vec<LilyPreparserFunBodyItem*>*

    while (!must_close(self)) {
        LilyPreparserFunBodyItem *item =
          preparse_block__LilyPreparser(self, must_close, true);

        if (item) {
            push__Vec(body, item);
        }
    }

    if (self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
        self->current->kind == LILY_TOKEN_KIND_KEYWORD_ELIF ||
        self->current->kind == LILY_TOKEN_KIND_KEYWORD_ELSE ||
        self->current->kind == LILY_TOKEN_KIND_KEYWORD_CATCH) {
        return body;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                init__Vec(
                  1, from__String("you may have forgotten the keyword `end`")),
                NULL,
                NULL),
              &self->count_error);

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserFunBodyItem);
            FREE(Vec, body);

            return NULL;

        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError,
                    LILY_ERROR_KIND_UNEXPECTED_TOKEN_IN_FUNCTION_BODY),
                init__Vec(
                  1,
                  from__String("you probably misclosed function body - use "
                               "`end` keyword to close the function body")),
                NULL,
                NULL),
              &self->count_error);

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserFunBodyItem);
            FREE(Vec, body);

            return NULL;
    }
}

LilyPreparserDecl *
preparse_fun__LilyPreparser(LilyPreparser *self)
{
    // fun [<object_impl>][<name>][<generic_params>][<params>][<when>][<req>]
    // [<return_data_type>] = <body> end
    // <object_impl> = @<identifier_normal> [ . <identifier_normal> ]*
    // <name> = <identifier_normal> | <identifier_operator>
    // <generic_params> = <hook_with_comma_sep>
    // <params> = <paren_with_comma_sep>
    // <when> = when <expr> [ , <expr> ]*
    // <req> = req <expr> [ , <expr> ]*
    // <return_data_type> = <tokens>
    // <body> = <block>
    // <tokens> = <token> [ <token> ]*
    // <block> = <token> [ <token> ]*
    // <paren_with_comma_sep> = ( <tokens> [ , <tokens> ]* )
    // <hook_with_comma_sep> = [ <tokens> [ , <tokens> ]* ]
    // <expr> = <tokens>
    next_token__LilyPreparser(self); // skip `fun` keyword

    Location location = clone__Location(&location_decl);
    String *object_impl = NULL;
    String *name = NULL;
    Vec *generic_params = NULL;   // Vec<Vec<LilyToken* (&)>*>*?
    Vec *params = NULL;           // Vec<Vec<LilyToken* (&)>*>*?
    Vec *when = NULL;             // Vec<Vec<LilyToken* (&)>*>*?
    Vec *req = NULL;              // Vec<Vec<LilyToken* (&)>*>*?
    Vec *body = NULL;             // Vec<LilyPreparserFunBodyItem*>*
    Vec *return_data_type = NULL; // Vec<LilyToken* (&)>*?
    bool is_operator = false;
    bool req_is_comptime = false;
    bool when_is_comptime = false;

    // 1. Get name's object implementation
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_AT:
            next_token__LilyPreparser(self);

        parse_object_impl_name: {
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    if (object_impl) {
                        push__String(object_impl, '.');
                        push_str__String(
                          object_impl,
                          self->current->identifier_normal->buffer);
                    } else {
                        object_impl =
                          clone__String(self->current->identifier_normal);
                    }

                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_DOT:
                            next_token__LilyPreparser(self);
                            goto parse_object_impl_name;
                        default:
                            break;
                    }

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                        NULL,
                        NULL,
                        from__String("expected identifier after `@` or `.`")),
                      &self->count_error);
                    break;
            }
        }

        break;
        default:
            break;
    }

    // 2. Get fun Name
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_OPERATOR:
            is_operator = true;
            name = clone__String(self->current->identifier_operator);

            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_FUN_IDENTIFIER),
                NULL,
                NULL,
                NULL),
              &self->count_error);

            name = from__String("__error__");

            break;
    }

    // 3. Get generic param(s)
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);
            break;
        default:
            break;
    }

    // 4. Get fun param(s)
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            params = preparse_paren_with_comma_sep__LilyPreparser(self);
            break;
        default:
            break;
    }

    // 5. Get when and req condition

    // when [<expr>] + ..., req [<expr>] + ...

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_WHEN:
            when = NEW(Vec);

        preparse_when_expr: {
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_HOOK: {
                    next_token__LilyPreparser(self);

                    Vec *expr = NEW(Vec);

                    PREPARSE_UNTIL(
                      expr, self->current->kind != LILY_TOKEN_KIND_R_HOOK);

                    push__Vec(when, expr);
                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_PLUS:
                            next_token__LilyPreparser(self);

                            goto preparse_when_expr;
                        case LILY_TOKEN_KIND_COMMA:
                            next_token__LilyPreparser(self);

                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_KEYWORD_REQ:
                                    goto preparse_req_expr;
                                case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
                                    goto preparse_comptime_expr;
                                default:
                                    break;
                            }

                            break;
                        default:
                            break;
                    }

                    break;
                }

                default:
                    break;
            }

            break;
        }
        case LILY_TOKEN_KIND_KEYWORD_REQ:
            req = NEW(Vec);

        preparse_req_expr: {
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_HOOK: {
                    next_token__LilyPreparser(self);

                    Vec *expr = NEW(Vec);

                    PREPARSE_UNTIL(
                      expr, self->current->kind != LILY_TOKEN_KIND_R_HOOK);

                    push__Vec(req, expr);
                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_PLUS:
                            next_token__LilyPreparser(self);

                            goto preparse_req_expr;
                        case LILY_TOKEN_KIND_COMMA:
                            next_token__LilyPreparser(self);

                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_KEYWORD_REQ:
                                    goto preparse_req_expr;
                                case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
                                    goto preparse_comptime_expr;
                                default:
                                    break;
                            }

                            break;
                        default:
                            break;
                    }

                    break;
                }
                default:
                    break;
            }

            break;
        }
        case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
        preparse_comptime_expr: {
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_REQ:
                    // TODO: check if req expr was declared
                    req_is_comptime = true;
                    goto preparse_req_expr;
                case LILY_TOKEN_KIND_KEYWORD_WHEN:
                    // TODO: check if when expr was declared
                    when_is_comptime = true;
                    goto preparse_when_expr;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected `req` or `when` keyword after "
                                     "`comptime` keyword")),
                      &self->count_error);

                    break;
            }
            break;
        }
        default:
            break;
    }

    // 6. Get return data type
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            break;
        default:
            return_data_type = NEW(Vec);

            PREPARSE_UNTIL(return_data_type,
                           self->current->kind != LILY_TOKEN_KIND_EQ &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            if (self->current->kind == LILY_TOKEN_KIND_EOF) {
                return NULL;
            }

            break;
    }

    // 6. Get body of fun.
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);

            body = preparse_body__LilyPreparser(
              self, &must_close_fun_block__LilyPreparser);

            break;

        default:
            UNREACHABLE("this way is not possible");
    }

    END_LOCATION(&location, self->current->location);
    next_token__LilyPreparser(self); // skip `end` keyword

    // TODO: Check if the function is async.

    return NEW_VARIANT(LilyPreparserDecl,
                       fun,
                       location,
                       NEW(LilyPreparserFun,
                           name,
                           object_impl,
                           generic_params,
                           params,
                           return_data_type,
                           body,
                           req,
                           when,
                           visibility_decl,
                           false,
                           is_operator,
                           when_is_comptime,
                           req_is_comptime));
}

LilyPreparserDecl *
preparse_constant__LilyPreparser(LilyPreparser *self)
{
    // val <name> [data_type] := <expr>;
    // val (<name> [data_type], ...) := (<expr>,...);
    next_token__LilyPreparser(self); // skip `val` keyword

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            return preparse_constant_multiple__LilyPreparser(self);
        default:
            break;
    }

    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    // 1. Get name of the constant.
    GET_NAME(self, from__String("expected name of the constant"));

    // 2. Get data type of the constant.
    Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*

    if (self->current->kind != LILY_TOKEN_KIND_COLON_EQ) {
        PREPARSE_UNTIL(data_type,
                       self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
                         self->current->kind != LILY_TOKEN_KIND_SEMICOLON);
    } else {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      self->file,
                      &self->current->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                      NULL,
                      NULL,
                      from__String("expected data type of the constant")),
          &self->count_error);
    }

    // 3. Get expression of the constant.
    Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `:=`")),
              &self->count_error);

            FREE(String, current_s);

            break;
        }
    }

    PREPARSE_UNTIL(expr,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;

        case LILY_TOKEN_KIND_EOF: {
            // Clean up allocations

            FREE(String, name);
            FREE(Vec, expr);
            FREE(Vec, data_type);

            return NULL;
        }

        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      constant,
      location,
      NEW_VARIANT(
        LilyPreparserConstant,
        simple,
        NEW(LilyPreparserConstantInfo, name, expr, data_type, visibility)));
}

LilyPreparserDecl *
preparse_constant_multiple__LilyPreparser(LilyPreparser *self)
{
    // val (<name> [data_type], ...) := (<expr>,...);
    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    next_token__LilyPreparser(self); // skip `(`

    // 1. Get name of the constants.
    Vec *names = NEW(Vec);      // Vec<String*>*
    Vec *data_types = NEW(Vec); // Vec<Vec<LilyToken* (&)>*?>*

    while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                push__Vec(names,
                          clone__String(self->current->identifier_normal));
                next_token__LilyPreparser(self);

                break;
            default:
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                    NULL,
                    NULL,
                    from__String("expected name of the constant")),
                  &self->count_error);

                push__Vec(names, from__String("__error__"));

                break;
        }

        if (self->current->kind != LILY_TOKEN_KIND_COMMA &&
            self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
            Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*

            PREPARSE_UNTIL(data_type,
                           self->current->kind != LILY_TOKEN_KIND_COMMA &&
                             self->current->kind != LILY_TOKEN_KIND_R_PAREN);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COMMA:
                    next_token__LilyPreparser(self);
                    break;

                case LILY_TOKEN_KIND_R_PAREN:
                    break;

                default:
                    UNREACHABLE("this way is impossible");
            }

            push__Vec(data_types, data_type);
        } else {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
                          NULL,
                          NULL,
                          from__String("expected data type of the constant")),
              &self->count_error);

            push__Vec(data_types, NEW(Vec));

            if (self->current->kind == LILY_TOKEN_KIND_COMMA) {
                next_token__LilyPreparser(self); // skip `,`
            }
        }
    }

    next_token__LilyPreparser(self); // skip `)`

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `:=`")),
              &self->count_error);

            FREE(String, current_s);

            break;
        }
    }

    // 2. Get expression of constants.
    Vec *exprs = NEW(Vec); // Vec<Vec<LilyToken* (&)>*>*

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            next_token__LilyPreparser(self);

            while (self->current->kind != LILY_TOKEN_KIND_R_PAREN) {
                Vec *expr = NEW(Vec); // Vec<LilyToken* (&)>*

                PREPARSE_UNTIL(expr,
                               self->current->kind != LILY_TOKEN_KIND_COMMA &&
                                 self->current->kind !=
                                   LILY_TOKEN_KIND_R_PAREN);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_COMMA:
                        next_token__LilyPreparser(self);
                        break;

                    case LILY_TOKEN_KIND_R_PAREN:
                        break;

                    default:
                        UNREACHABLE("this way is impossible")
                }

                push__Vec(exprs, expr);
            }

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_EXPRESSION),
                          NULL,
                          NULL,
                          from__String("expected `(`")),
              &self->count_error);

            break;
    }

    next_token__LilyPreparser(self); // skip `)`

    if (exprs->len < names->len) {
        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->file,
            &self->current->location,
            NEW(LilyError, LILY_ERROR_KIND_MISS_ONE_OR_MANY_EXPRESSIONS),
            NULL,
            NULL,
            NULL),
          &self->count_error);

        // Fill exprs while exprs and names aren't the same length.
        for (int i = 0; i < names->len - exprs->len; ++i) {
            push__Vec(exprs, NEW(Vec));
        }
    } else if (exprs->len > names->len) {
        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->file,
            &self->current->location,
            NEW(LilyError, LILY_ERROR_KIND_MISS_ONE_OR_MANY_IDENTIFIERS),
            NULL,
            NULL,
            NULL),
          &self->count_error);

        // Fill names while exprs and names aren't the same length.
        for (int i = 0; i < exprs->len - names->len; ++i) {
            push__Vec(exprs, NEW(Vec));
        }
    }

    Vec *multiple = NEW(Vec); // Vec<LilyPreparserConstantInfo*>*

    for (Usize i = 0; i < names->len; ++i) {
        push__Vec(multiple,
                  NEW(LilyPreparserConstantInfo,
                      get__Vec(names, i),
                      get__Vec(exprs, i),
                      get__Vec(data_types, i),
                      visibility));
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;

        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);

            break;
    }

    // Clean up allocations

    FREE(Vec, names);
    FREE(Vec, data_types);
    FREE(Vec, exprs);

    return NEW_VARIANT(LilyPreparserDecl,
                       constant,
                       location,
                       NEW_VARIANT(LilyPreparserConstant, multiple, multiple));
}

LilyPreparserClassBodyItem *
preparse_attribute_for_class__LilyPreparser(LilyPreparser *self,
                                            Location location)
{
    // val <name> [data_type] [:= <expr>] [:: get | [,] set] ;
    next_token__LilyPreparser(self); // skip `val` keyword

    // 1. Get attribute's name
    GET_NAME(self, from__String("expected name of the attribute"));

    // 2. Get attribute's data type
    Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(data_type,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_COLON &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    if (data_type->len == 0) {
        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->file,
            &self->current->location,
            NEW(LilyError, LILY_ERROR_KIND_EXPECTED_DATA_TYPE),
            NULL,
            init__Vec(
              1, from__String("data type of the attribute is not optional")),
            from__String("expected data type of the attribute")),
          &self->count_error);
    }

    // 3. Preparse default expression
    Vec *default_expr = NULL; // Vec<LilyToken* (&)>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);

            default_expr = NEW(Vec);

            PREPARSE_UNTIL(default_expr,
                           self->current->kind != LILY_TOKEN_KIND_COLON_COLON &&
                             self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String("expected `::` or `;`")),
                      &self->count_error);

                    FREE(String, name);
                    FREE(Vec, data_type);

                    if (default_expr) {
                        FREE(Vec, default_expr);
                    }

                    return NULL;
                default:
                    break;
            }

            break;
        default:
            break;
    }

    // 4. Check if encapsulation is defined
    bool is_get = false;
    bool is_set = false;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_COLON:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_GET:
                    is_get = true;
                    break;

                case LILY_TOKEN_KIND_KEYWORD_SET:
                    is_set = true;
                    break;

                default: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, current_s->buffer),
                        NULL,
                        NULL,
                        from__String("expected `get` or `set` keyword")),
                      &self->count_error);

                    FREE(String, current_s);
                    FREE(String, name);
                    FREE(Vec, data_type);

                    if (default_expr) {
                        FREE(Vec, default_expr);
                    }

                    return NULL;
                }
            }

            next_token__LilyPreparser(self); // skip `get` or `set` keyword

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_COMMA:
                    next_token__LilyPreparser(self);
                    break;

                default:
                    goto return_attribute;
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_GET:
                    if (is_get) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_GET_IS_DUPLICATE),
                            NULL,
                            init__Vec(
                              1, from__String("remove one of the two `get`")),
                            NULL),
                          &self->count_error);
                    } else {
                        is_get = true;
                    }

                    break;

                case LILY_TOKEN_KIND_KEYWORD_SET:
                    if (is_set) {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_SET_IS_DUPLICATE),
                            NULL,
                            init__Vec(
                              1, from__String("remove one of the two `set`")),
                            NULL),
                          &self->count_error);
                    } else {
                        is_set = true;
                    }

                    break;

                default:
                    break;
            }

            next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

return_attribute: {
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);
        }
    }

    END_LOCATION(&location, self->current->location);

    return NEW_VARIANT(LilyPreparserClassBodyItem,
                       attribute,
                       NEW(LilyPreparserAttribute,
                           name,
                           data_type,
                           default_expr,
                           is_get,
                           is_set,
                           visibility_decl),
                       location);
}
}

LilyPreparserTraitBodyItem *
preparse_attribute_for_trait__LilyPreparser(LilyPreparser *self,
                                            Location location)
{
    LilyPreparserClassBodyItem *item =
      preparse_attribute_for_class__LilyPreparser(self, location);

    if (item) {
        LilyPreparserTraitBodyItem *res = NEW_VARIANT(
          LilyPreparserTraitBodyItem, attribute, location, item->attribute);

        lily_free(item);

        return res;
    }

    return NULL;
}

LilyPreparserTraitBodyItem *
preparse_macro_expand_for_trait__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserTraitBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserTraitBodyItem,
                      macro_expand,
                      decl->location,
                      decl->macro_expand);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

LilyPreparserClassBodyItem *
preparse_macro_expand_for_class__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserClassBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserClassBodyItem,
                      macro_expand,
                      decl->macro_expand,
                      decl->location);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

LilyPreparserClassBodyItem *
preparse_method_for_class__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_fun__LilyPreparser(self);

    if (decl) {
        LilyPreparserClassBodyItem *method = NEW_VARIANT(
          LilyPreparserClassBodyItem, method, decl->fun, decl->location);

        lily_free(decl);

        return method;
    }

    return NULL;
}

Vec *
preparse_class_body__LilyPreparser(LilyPreparser *self)
{
    Vec *body = NEW(Vec); // Vec<LilyPreparserClassBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_PUB: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_VAL: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserClassBodyItem *attribute =
                              preparse_attribute_for_class__LilyPreparser(
                                self,
                                clone__Location(&self->current->location));

                            if (attribute) {
                                push__Vec(body, attribute);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        case LILY_TOKEN_KIND_KEYWORD_FUN: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserClassBodyItem *method =
                              preparse_method_for_class__LilyPreparser(self);

                            if (method) {
                                push__Vec(body, method);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        default: {
                            String *current_s =
                              to_string__LilyToken(self->current);

                            emit__Diagnostic(
                              NEW_VARIANT(Diagnostic,
                                          simple_lily_error,
                                          self->file,
                                          &self->current->location,
                                          NEW_VARIANT(LilyError,
                                                      unexpected_token,
                                                      current_s->buffer),
                                          NULL,
                                          NULL,
                                          from__String(
                                            "expected `fun` or `val` keyword")),
                              &self->count_error);

                            FREE(String, current_s);

                            goto clean_up;
                        }
                    }
                } else {
                    String *current_s = from__String("pub");

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, current_s->buffer),
                        NULL,
                        init__Vec(
                          1, from__String("expected `fun` or `val` keyword")),
                        NULL),
                      &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_GLOBAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_VAL: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_STATIC;

                            LilyPreparserClassBodyItem *attribute =
                              preparse_attribute_for_class__LilyPreparser(
                                self,
                                clone__Location(&self->current->location));

                            if (attribute) {
                                push__Vec(body, attribute);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        case LILY_TOKEN_KIND_KEYWORD_FUN: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_STATIC;

                            LilyPreparserClassBodyItem *method =
                              preparse_method_for_class__LilyPreparser(self);

                            if (method) {
                                push__Vec(body, method);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default: {
                            String *current_s = from__String("pub");

                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW_VARIANT(LilyError,
                                            unexpected_token,
                                            current_s->buffer),
                                NULL,
                                init__Vec(1,
                                          from__String(
                                            "expected `fun` or `val` keyword")),
                                NULL),
                              &self->count_error);

                            FREE(String, current_s);

                            goto clean_up;
                        }
                    }
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                LilyPreparserClassBodyItem *attribute =
                  preparse_attribute_for_class__LilyPreparser(
                    self, clone__Location(&self->current->location));

                if (attribute) {
                    push__Vec(body, attribute);
                } else {
                    goto clean_up;
                }

                break;
            }
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserClassBodyItem *macro_expand =
                              preparse_macro_expand_for_class__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default:
                            goto unexpected_token_macro_expand;
                    }
                } else {
                unexpected_token_macro_expand: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                                 simple_lily_error,
                                                 self->file,
                                                 &self->current->location,
                                                 NEW_VARIANT(LilyError,
                                                             unexpected_token,
                                                             current_s->buffer),
                                                 NULL,
                                                 NULL,
                                                 from__String("expected `!`")),
                                     &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserClassBodyItem *method =
                  preparse_method_for_class__LilyPreparser(self);

                if (method) {
                    push__Vec(body, method);
                } else {
                    goto clean_up;
                }

                break;
            }
            default: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `fun`, `val` or identifier")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);
            }
        }
    }

    return body;

clean_up: {
    // Clean up allocations

    FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserTraitBodyItem);
    FREE(Vec, body);

    visibility_decl = LILY_VISIBILITY_PRIVATE;

    return NULL;
}
}

LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *impls,
                              Vec *inherits,
                              Vec *generic_params,
                              bool is_close)
{
    Location location = clone__Location(&location_decl);
    enum LilyVisibility visibility = visibility_decl;

    // 1. Preparse class body
    Vec *body = preparse_class_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;

        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                NULL,
                NULL,
                from__String("expected `end` keyword to close class")),
              &self->count_error);

            // Clean up allocations

        clean_up: {
            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);
                FREE(Vec, impls);
            }

            if (inherits) {
                FREE_BUFFER_ITEMS(inherits->buffer, inherits->len, Vec);
                FREE(Vec, inherits);
            }

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserClassBodyItem);
            FREE(Vec, body);

            visibility_decl = LILY_VISIBILITY_PRIVATE;

            return NULL;
        }

        default:
            UNREACHABLE("this way is possible");
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       object,
                       location,
                       NEW_VARIANT(LilyPreparserObject,
                                   class,
                                   NEW(LilyPreparserClass,
                                       name,
                                       generic_params,
                                       inherits,
                                       impls,
                                       body,
                                       visibility,
                                       is_close)));
}

LilyPreparserTraitBodyItem *
preparse_prototype__LilyPreparser(LilyPreparser *self)
{
    // fun <name> [generic_params] (params) [return_data_type] ;
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self); // skip `fun` keyword

    // 1. Get name of prototype
    GET_NAME(self, from__String("expected name of the prototype here"));

    // 2. Preparse generic params.
    Vec *generic_params = NULL; // Vec<Vec<LilyToken* (&)>*>*

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);

            break;
        default:
            break;
    }

    // 3. Preparse params.
    Vec *params = NULL; // Vec<Vec<LilyToken* (&)>*>*

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            params = preparse_paren_with_comma_sep__LilyPreparser(self);
            break;

        default:
            break;
    }

    // 4. Preparse return data type.
    Vec *return_data_type = NULL; // Vec<LilyToken* (&)>*

    if (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
        self->current->kind != LILY_TOKEN_KIND_EOF &&
        self->current->kind != LILY_TOKEN_KIND_KEYWORD_END) {
        return_data_type = NEW(Vec);

        PREPARSE_UNTIL(return_data_type,
                       self->current->kind != LILY_TOKEN_KIND_SEMICOLON);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);
            break;

        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);
    }

    END_LOCATION(&location, self->current->location);

    return NEW_VARIANT(LilyPreparserTraitBodyItem,
                       prototype,
                       location,
                       NEW(LilyPreparserPrototype,
                           name,
                           generic_params,
                           params,
                           return_data_type));
}

Vec *
preparse_trait_body__LilyPreparser(LilyPreparser *self)
{
    Vec *body = NEW(Vec); // Vec<LilyPreparserTraitBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                LilyPreparserTraitBodyItem *attribute =
                  preparse_attribute_for_trait__LilyPreparser(
                    self, clone__Location(&self->current->location));

                if (attribute) {
                    push__Vec(body, attribute);
                } else {
                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_GLOBAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_FUN: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_STATIC;

                            LilyPreparserTraitBodyItem *prototype =
                              preparse_prototype__LilyPreparser(self);

                            if (prototype) {
                                push__Vec(body, prototype);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        case LILY_TOKEN_KIND_KEYWORD_VAL: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_STATIC;

                            LilyPreparserTraitBodyItem *attribute =
                              preparse_attribute_for_trait__LilyPreparser(
                                self,
                                clone__Location(&self->current->location));

                            if (attribute) {
                                push__Vec(body, attribute);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        default: {
                            String *current_s = from__String("global");

                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW_VARIANT(LilyError,
                                            unexpected_token,
                                            current_s->buffer),
                                NULL,
                                init__Vec(1,
                                          from__String(
                                            "expected `fun` or `val` keyword")),
                                NULL),
                              &self->count_error);

                            FREE(String, current_s);

                            goto clean_up;
                        }
                    }
                } else {
                    String *current_s = from__String("global");

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, current_s->buffer),
                        NULL,
                        init__Vec(
                          1, from__String("expected `fun` or `val` keyword")),
                        NULL),
                      &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }

                visibility_decl = LILY_VISIBILITY_PRIVATE;

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_PUB: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_FUN: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserTraitBodyItem *prototype =
                              preparse_prototype__LilyPreparser(self);

                            if (prototype) {
                                push__Vec(body, prototype);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        case LILY_TOKEN_KIND_KEYWORD_VAL: {
                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserTraitBodyItem *attribute =
                              preparse_attribute_for_trait__LilyPreparser(
                                self,
                                clone__Location(&self->current->location));

                            if (attribute) {
                                push__Vec(body, attribute);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }

                        default: {
                            String *current_s = from__String("pub");

                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW_VARIANT(LilyError,
                                            unexpected_token,
                                            current_s->buffer),
                                NULL,
                                init__Vec(1,
                                          from__String(
                                            "expected `fun` or `val` keyword")),
                                NULL),
                              &self->count_error);

                            FREE(String, current_s);

                            goto clean_up;
                        }
                    }
                } else {
                    String *current_s = from__String("pub");

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, current_s->buffer),
                        NULL,
                        init__Vec(
                          1, from__String("expected `fun` or `val` keyword")),
                        NULL),
                      &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }

                visibility_decl = LILY_VISIBILITY_PRIVATE;

                break;
            }

            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserTraitBodyItem *macro_expand =
                              preparse_macro_expand_for_trait__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default:
                            goto unexpected_token_macro_expand;
                    }
                } else {
                unexpected_token_macro_expand: {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(NEW_VARIANT(Diagnostic,
                                                 simple_lily_error,
                                                 self->file,
                                                 &self->current->location,
                                                 NEW_VARIANT(LilyError,
                                                             unexpected_token,
                                                             current_s->buffer),
                                                 NULL,
                                                 NULL,
                                                 from__String("expected `!`")),
                                     &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserTraitBodyItem *prototype =
                  preparse_prototype__LilyPreparser(self);

                if (prototype) {
                    push__Vec(body, prototype);
                } else {
                    goto clean_up;
                }

                break;
            }
            default: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `fun`, `val` or identifier")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
        }
    }

    return body;

clean_up: {
    // Clean up allocations

    FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserTraitBodyItem);
    FREE(Vec, body);

    visibility_decl = LILY_VISIBILITY_PRIVATE;

    return NULL;
}
}

LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *inherits,
                              Vec *generic_params,
                              bool is_close)
{
    // trait <name> [generic_params] [inherits] [body] end
    Location location = clone__Location(&location_decl);
    enum LilyVisibility visibility = visibility_decl;

    // 1. Preparse body.
    Vec *body = preparse_trait_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                NULL,
                NULL,
                from__String("expected `end` keyword to close trait")),
              &self->count_error);

        clean_up: {
            FREE(String, name);

            if (inherits) {
                FREE_BUFFER_ITEMS(inherits->buffer, inherits->len, Vec);
                FREE(Vec, inherits);
            }

            if (generic_params) {
                FREE_BUFFER_ITEMS(
                  generic_params->buffer, generic_params->len, Vec);
                FREE(Vec, generic_params);
            }

            if (body) {
                FREE_BUFFER_ITEMS(
                  body->buffer, body->len, LilyPreparserTraitBodyItem);
                FREE(Vec, body);
            }

            return NULL;
        }

        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       object,
                       location,
                       NEW_VARIANT(LilyPreparserObject,
                                   trait,
                                   NEW(LilyPreparserTrait,
                                       name,
                                       generic_params,
                                       inherits,
                                       body,
                                       visibility,
                                       is_close)));
}

LilyPreparserRecordObjectBodyItem *
preparse_macro_expand_for_record_object__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserRecordObjectBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserRecordObjectBodyItem,
                      macro_expand,
                      decl->location,
                      decl->macro_expand);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

LilyPreparserRecordObjectBodyItem *
preparse_method_for_record__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_fun__LilyPreparser(self);

    if (decl) {
        LilyPreparserRecordObjectBodyItem *method = NEW_VARIANT(
          LilyPreparserRecordObjectBodyItem, method, decl->location, decl->fun);

        lily_free(decl);

        return method;
    }

    return NULL;
}

LilyPreparserRecordObjectBodyItem *
preparse_constant_for_record__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_constant__LilyPreparser(self);

    if (decl) {
        LilyPreparserRecordObjectBodyItem *constant =
          NEW_VARIANT(LilyPreparserRecordObjectBodyItem,
                      constant,
                      decl->location,
                      decl->constant);

        lily_free(decl);

        return constant;
    }

    return NULL;
}

Vec *
preparse_record_object_body__LilyPreparser(LilyPreparser *self)
{
    // <field> = <name> <data_type> ;
    // <fun> = fun ...
    // <const> = val ...
    // <macro_expand> = <name> ! [ <params> ] ;
    // <item> = <field> | <fun> | <const> | <macro_expand>
    // <body> = <item> [ <item> ]*
    Vec *body = NEW(Vec); // Vec<LilyPreparserRecordObjectBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                LilyPreparserRecordObjectBodyItem *constant =
                  preparse_constant_for_record__LilyPreparser(self);

                if (constant) {
                    push__Vec(body, constant);
                } else {
                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserRecordObjectBodyItem *method =
                  preparse_method_for_record__LilyPreparser(self);

                if (method) {
                    push__Vec(body, method);
                } else {
                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserRecordObjectBodyItem *macro_expand =
                              preparse_macro_expand_for_record_object__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default: {
                            Location location_field =
                              clone__Location(&self->current->location);
                            LilyPreparserRecordField *field =
                              preparse_record_field__LilyPreparser(self, false);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_field,
                                             previous->location);
                            }

                            if (field) {
                                push__Vec(
                                  body,
                                  NEW_VARIANT(LilyPreparserRecordObjectBodyItem,
                                              field,
                                              location_field,
                                              *field));
                                lily_free(field);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                    }
                } else {
                    String *current_s = to_string__LilyToken(self->current);

                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW_VARIANT(
                          LilyError, unexpected_token, current_s->buffer),
                        NULL,
                        NULL,
                        from__String("expected `fun`, `val`, identifier + `!` "
                                     "or identifier")),
                      &self->count_error);

                    FREE(String, current_s);

                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_PUB: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    visibility_decl = LILY_VISIBILITY_PUBLIC;

                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_FUN: {
                            next_token__LilyPreparser(self); // skip `pub`

                            LilyPreparserRecordObjectBodyItem *method =
                              preparse_method_for_record__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            if (method) {
                                push__Vec(body, method);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        case LILY_TOKEN_KIND_KEYWORD_VAL: {
                            next_token__LilyPreparser(self); // skip `pub`

                            LilyPreparserRecordObjectBodyItem *constant =
                              preparse_constant_for_record__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            if (constant) {
                                push__Vec(body, constant);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                            Location location_field =
                              clone__Location(&self->current->location);

                            next_token__LilyPreparser(self); // skip `pub`

                            LilyPreparserRecordField *field =
                              preparse_record_field__LilyPreparser(self, false);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_field,
                                             previous->location);
                            }

                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            if (field) {
                                push__Vec(
                                  body,
                                  NEW_VARIANT(LilyPreparserRecordObjectBodyItem,
                                              field,
                                              location_field,
                                              *field));
                                lily_free(field);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default:
                            visibility_decl = LILY_VISIBILITY_PRIVATE;
                            goto unexpected_token;
                    }
                }

                break;
            }

            default: {
            unexpected_token: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected identifier, `val`, `pub` or `fun`")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
            }
        }
    }

    return body;

clean_up: {
    FREE_BUFFER_ITEMS(
      body->buffer, body->len, LilyPreparserRecordObjectBodyItem);
    FREE(Vec, body);

    return NULL;
}
}

LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self,
                                      String *name,
                                      Vec *impls,
                                      Vec *generic_params)
{
    // object <name> [generic_params] record [impls] [body] end
    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    Vec *body = preparse_record_object_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                NULL,
                NULL,
                from__String("expected `end` keyword to close record object")),
              &self->count_error);

        clean_up: {
            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);
                FREE(Vec, impls);
            }

            if (generic_params) {
                FREE_BUFFER_ITEMS(
                  generic_params->buffer, generic_params->len, Vec);
                FREE(Vec, generic_params);
            }

            if (body) {
                FREE_BUFFER_ITEMS(
                  body->buffer, body->len, LilyPreparserRecordObjectBodyItem);
                FREE(Vec, body);
            }

            return NULL;
        }

        case LILY_TOKEN_KIND_KEYWORD_END:
            next_token__LilyPreparser(self);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    END_LOCATION(&location, self->current->location);

    return NEW_VARIANT(LilyPreparserDecl,
                       object,
                       location,
                       NEW_VARIANT(LilyPreparserObject,
                                   record,
                                   NEW(LilyPreparserRecordObject,
                                       name,
                                       generic_params,
                                       impls,
                                       body,
                                       visibility)));
}

LilyPreparserEnumObjectBodyItem *
preparse_constant_for_enum__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_constant__LilyPreparser(self);

    if (decl) {
        LilyPreparserEnumObjectBodyItem *constant =
          NEW_VARIANT(LilyPreparserEnumObjectBodyItem,
                      constant,
                      decl->location,
                      decl->constant);

        lily_free(decl);

        return constant;
    }

    return NULL;
}

LilyPreparserEnumObjectBodyItem *
preparse_macro_expand_for_enum_object__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserEnumObjectBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserEnumObjectBodyItem,
                      macro_expand,
                      decl->location,
                      decl->macro_expand);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

LilyPreparserEnumObjectBodyItem *
preparse_method_for_enum__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_fun__LilyPreparser(self);

    if (decl) {
        LilyPreparserEnumObjectBodyItem *method = NEW_VARIANT(
          LilyPreparserEnumObjectBodyItem, method, decl->location, decl->fun);

        lily_free(decl);

        return method;
    }

    return NULL;
}

Vec *
preparse_enum_object_body__LilyPreparser(LilyPreparser *self)
{
    // <variant> = <name> [ : <data_type> ] ;
    // <fun> = fun ...
    // <const> = val ...
    // <macro_expand> = <name> ! [ <params> ] ;
    // <item> = <variant> | <fun> | <const> | <macro_expand>
    // <body> = <item> [ <item> ]*
    Vec *body = NEW(Vec); // Vec<LilyPreparserEnumObjectBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                LilyPreparserEnumObjectBodyItem *constant =
                  preparse_constant_for_enum__LilyPreparser(self);

                if (constant) {
                    push__Vec(body, constant);
                } else {
                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserEnumObjectBodyItem *method =
                  preparse_method_for_enum__LilyPreparser(self);

                if (method) {
                    push__Vec(body, method);
                } else {
                    goto clean_up;
                }

                break;
            }

            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserEnumObjectBodyItem *macro_expand =
                              preparse_macro_expand_for_enum_object__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default: {

                            Location location_variant =
                              clone__Location(&self->current->location);
                            LilyPreparserEnumVariant *variant =
                              preparse_enum_variant__LilyPreparser(self);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_variant,
                                             previous->location);
                            }

                            if (variant) {
                                push__Vec(
                                  body,
                                  NEW_VARIANT(LilyPreparserEnumObjectBodyItem,
                                              variant,
                                              location_variant,
                                              *variant));
                                lily_free(variant);
                            } else {
                                goto clean_up;
                            }
                            break;
                        }
                    }
                } else {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        NULL,
                        NULL,
                        from__String("expected `!` or data type")),
                      &self->count_error);

                    goto clean_up;
                }

                break;
            }

            default: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected identifier, `val` or `fun`")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
        }
    }

    return body;

clean_up: {
    // Clean up allocations

    FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserEnumObjectBodyItem);
    FREE(Vec, body);

    return NULL;
}
}

LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self,
                                    String *name,
                                    Vec *impls,
                                    Vec *generic_params)
{
    // object <name> [generic_params] enum [impls] [body] end
    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    Vec *body = preparse_enum_object_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                NULL,
                NULL,
                from__String("expected `end` keyword to close enum object")),
              &self->count_error);

        clean_up: {
            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);
                FREE(Vec, impls);
            }

            if (generic_params) {
                FREE_BUFFER_ITEMS(
                  generic_params->buffer, generic_params->len, Vec);
                FREE(Vec, generic_params);
            }

            if (body) {
                FREE_BUFFER_ITEMS(
                  body->buffer, body->len, LilyPreparserEnumObjectBodyItem);
                FREE(Vec, body);
            }

            return NULL;
        }

        case LILY_TOKEN_KIND_KEYWORD_END:
            next_token__LilyPreparser(self);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    END_LOCATION(&location, self->current->location);

    return NEW_VARIANT(LilyPreparserDecl,
                       object,
                       location,
                       NEW_VARIANT(LilyPreparserObject,
                                   enum_,
                                   NEW(LilyPreparserEnumObject,
                                       name,
                                       generic_params,
                                       impls,
                                       body,
                                       visibility)));
}

LilyPreparserDecl *
preparse_object__LilyPreparser(LilyPreparser *self, bool is_close)
{
#define PREPARSE_INHERIT_OR_IMPL(v, label, token_kind)                         \
    next_token__LilyPreparser(self);                                           \
                                                                               \
    switch (self->current->kind) {                                             \
        case LILY_TOKEN_KIND_L_HOOK:                                           \
            if (v) {                                                           \
                switch (token_kind) {                                          \
                    case LILY_TOKEN_KIND_KEYWORD_IMPL:                         \
                        emit__Diagnostic(                                      \
                          NEW_VARIANT(                                         \
                            Diagnostic,                                        \
                            simple_lily_error,                                 \
                            self->file,                                        \
                            &self->current->location,                          \
                            NEW(LilyError,                                     \
                                LILY_ERROR_KIND_IMPL_IS_ALREADY_DEFINED),      \
                            init__Vec(1,                                       \
                                      from__String(                            \
                                        "remove this re-definition of impl")), \
                            NULL,                                              \
                            NULL),                                             \
                          &self->count_error);                                 \
                                                                               \
                        /* Clean up allocations */                             \
                        FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);     \
                        FREE(Vec, impls);                                      \
                                                                               \
                        if (inherits) {                                        \
                            FREE_BUFFER_ITEMS(                                 \
                              inherits->buffer, inherits->len, Vec);           \
                            FREE(Vec, inherits);                               \
                        }                                                      \
                                                                               \
                        return NULL;                                           \
                    case LILY_TOKEN_KIND_KEYWORD_INHERIT:                      \
                        emit__Diagnostic(                                      \
                          NEW_VARIANT(                                         \
                            Diagnostic,                                        \
                            simple_lily_error,                                 \
                            self->file,                                        \
                            &self->current->location,                          \
                            NEW(LilyError,                                     \
                                LILY_ERROR_KIND_INHERIT_IS_ALREADY_DEFINED),   \
                            init__Vec(                                         \
                              1,                                               \
                              from__String(                                    \
                                "remove this re-definition of inherit")),      \
                            NULL,                                              \
                            NULL),                                             \
                          &self->count_error);                                 \
                                                                               \
                        /* Clean up allocations */                             \
                        FREE_BUFFER_ITEMS(                                     \
                          inherits->buffer, inherits->len, Vec);               \
                        FREE(Vec, inherits);                                   \
                                                                               \
                        if (impls) {                                           \
                            FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec); \
                            FREE(Vec, impls);                                  \
                        }                                                      \
                                                                               \
                        return NULL;                                           \
                    default:                                                   \
                        UNREACHABLE("this way is impossible");                 \
                }                                                              \
            }                                                                  \
                                                                               \
            v = preparse_hook_with_comma_sep__LilyPreparser(self);             \
                                                                               \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_KEYWORD_IN:                               \
                    next_token__LilyPreparser(self);                           \
                    break;                                                     \
                                                                               \
                case LILY_TOKEN_KIND_PLUS:                                     \
                    next_token__LilyPreparser(self);                           \
                    goto label;                                                \
                                                                               \
                default: {                                                     \
                    String *current_s = to_string__LilyToken(self->current);   \
                                                                               \
                    emit__Diagnostic(                                          \
                      NEW_VARIANT(                                             \
                        Diagnostic,                                            \
                        simple_lily_error,                                     \
                        self->file,                                            \
                        &self->current->location,                              \
                        NEW_VARIANT(                                           \
                          LilyError, unexpected_token, current_s->buffer),     \
                        NULL,                                                  \
                        NULL,                                                  \
                        from__String("expected `in` keyword or `+`")),         \
                      &self->count_error);                                     \
                                                                               \
                    FREE(String, current_s);                                   \
                                                                               \
                    break;                                                     \
                }                                                              \
            }                                                                  \
                                                                               \
            break;                                                             \
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {                              \
            v = NEW(Vec);                                                      \
                                                                               \
            Vec *item = NEW(Vec); /* Vec<LilyToken* (&)>* */                   \
                                                                               \
            PREPARSE_UNTIL(item,                                               \
                           self->current->kind != LILY_TOKEN_KIND_PLUS &&      \
                             self->current->kind !=                            \
                               LILY_TOKEN_KIND_KEYWORD_IN &&                   \
                             self->current->kind != LILY_TOKEN_KIND_EOF);      \
                                                                               \
            switch (self->current->kind) {                                     \
                case LILY_TOKEN_KIND_PLUS:                                     \
                    switch (self->current->kind) {                             \
                        case token_kind:                                       \
                            push__Vec(v, item);                                \
                                                                               \
                            goto label;                                        \
                        default: {                                             \
                            String *current_s =                                \
                              to_string__LilyToken(self->current);             \
                                                                               \
                            emit__Diagnostic(                                  \
                              NEW_VARIANT(                                     \
                                Diagnostic,                                    \
                                simple_lily_error,                             \
                                self->file,                                    \
                                &self->current->location,                      \
                                NEW_VARIANT(LilyError,                         \
                                            unexpected_token,                  \
                                            current_s->buffer),                \
                                NULL,                                          \
                                NULL,                                          \
                                from__String(                                  \
                                  "expected `impl` or `inherit` keyword")),    \
                              &self->count_error);                             \
                                                                               \
                            FREE(String, current_s);                           \
                                                                               \
                            goto label;                                        \
                        }                                                      \
                    }                                                          \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_KEYWORD_IN:                               \
                    next_token__LilyPreparser(self);                           \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_EOF:                                      \
                    emit__Diagnostic(                                          \
                      NEW_VARIANT(                                             \
                        Diagnostic,                                            \
                        simple_lily_error,                                     \
                        self->file,                                            \
                        &self->current->location,                              \
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),      \
                        NULL,                                                  \
                        NULL,                                                  \
                        from__String("expected `in` keyword or `+`")),         \
                      &self->count_error);                                     \
                                                                               \
                    break;                                                     \
                default:                                                       \
                    UNREACHABLE("this way is impossible");                     \
            }                                                                  \
                                                                               \
            push__Vec(v, item);                                                \
                                                                               \
            break;                                                             \
        }                                                                      \
        default: {                                                             \
            String *current_s = to_string__LilyToken(self->current);           \
                                                                               \
            emit__Diagnostic(                                                  \
              NEW_VARIANT(                                                     \
                Diagnostic,                                                    \
                simple_lily_error,                                             \
                self->file,                                                    \
                &self->current->location,                                      \
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),   \
                NULL,                                                          \
                NULL,                                                          \
                from__String("expected identifier or `[`")),                   \
              &self->count_error);                                             \
                                                                               \
            FREE(String, current_s);                                           \
                                                                               \
            return NULL;                                                       \
        }                                                                      \
    }                                                                          \
    break;

    next_token__LilyPreparser(self); // skip `object` keyword

    // 1. Preparse impl and inherit
    Location *location_inherit = NULL;
    Location *location_impl = NULL;
    Vec *impls = NULL;    // Vec<Vec<LilyToken* (&)>*>*?
    Vec *inherits = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_IMPL: {
        preparse_impl: {
            location_impl = &self->current->location;
            PREPARSE_INHERIT_OR_IMPL(
              impls, preparse_inherit, LILY_TOKEN_KIND_KEYWORD_INHERIT);
        }
        }
        case LILY_TOKEN_KIND_KEYWORD_INHERIT: {
        preparse_inherit: {
            location_inherit = &self->current->location;
            PREPARSE_INHERIT_OR_IMPL(
              inherits, preparse_impl, LILY_TOKEN_KIND_KEYWORD_IMPL);
        }
        }
        default:
            break;
    }

    // 2. Get object's name
    GET_NAME(self, from__String("expected name of the object"));

    // 3. Preparse generic params
    Vec *generic_params = NULL; // Vec<Vec<LilyToken* (&)>*>*

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);

            break;
        default:
            break;
    }

    // 4. Check object kind (class, enum, record or trait)
    enum LilyTokenKind object_kind = self->current->kind;

    next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);

            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `=`")),
              &self->count_error);

            FREE(String, current_s);

            break;
        }
    }

    switch (object_kind) {
        case LILY_TOKEN_KIND_KEYWORD_CLASS:
            return preparse_class__LilyPreparser(
              self, name, impls, inherits, generic_params, is_close);

        case LILY_TOKEN_KIND_KEYWORD_ENUM:
            if (inherits) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location_inherit,
                    NEW(LilyError, LILY_ERROR_KIND_INHERIT_IS_NOT_EXPECTED),
                    NULL,
                    NULL,
                    from__String("inherit is not expected on enum object")),
                  &self->count_error);

                // Clean up allocations

                FREE_BUFFER_ITEMS(inherits->buffer, inherits->len, Vec);
                FREE(Vec, inherits);
            }

            if (is_close) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location_inherit,
                    NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_CLOSE),
                    NULL,
                    NULL,
                    from__String("close object is not expected for enum")),
                  &self->count_error);
            }

            return preparse_enum_object__LilyPreparser(
              self, name, impls, generic_params);

        case LILY_TOKEN_KIND_KEYWORD_RECORD:
            if (inherits) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location_inherit,
                    NEW(LilyError, LILY_ERROR_KIND_INHERIT_IS_NOT_EXPECTED),
                    NULL,
                    NULL,
                    from__String("inherit is not expected on record object")),
                  &self->count_error);

                // Clean up allocations

                FREE_BUFFER_ITEMS(inherits->buffer, inherits->len, Vec);
                FREE(Vec, inherits);
            }

            if (is_close) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location_inherit,
                    NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_CLOSE),
                    NULL,
                    NULL,
                    from__String("close object is not expected for record")),
                  &self->count_error);
            }

            return preparse_record_object__LilyPreparser(
              self, name, impls, generic_params);

        case LILY_TOKEN_KIND_KEYWORD_TRAIT:
            if (impls) {
                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    location_impl,
                    NEW(LilyError, LILY_ERROR_KIND_IMPL_IS_NOT_EXPECTED),
                    NULL,
                    NULL,
                    from__String("impl is not expected on trait")),
                  &self->count_error);

                // Clean up allocations

                FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);
                FREE(Vec, impls);
            }

            return preparse_trait__LilyPreparser(
              self, name, inherits, generic_params, is_close);

        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_BAD_KIND_OF_OBJECT),
                init__Vec(1,
                          from__String(
                            "expected `class`, `enum`, `record` or `trait`")),
                NULL,
                NULL),
              &self->count_error);

            // Clean up allocations

            if (impls) {
                FREE_BUFFER_ITEMS(impls->buffer, impls->len, Vec);
                FREE(Vec, impls);
            }

            if (inherits) {
                FREE_BUFFER_ITEMS(inherits->buffer, inherits->len, Vec);
                FREE(Vec, inherits);
            }

            FREE(String, name);

            return NULL;
    }
}

LilyPreparserDecl *
preparse_type__LilyPreparser(LilyPreparser *self)
{
    next_token__LilyPreparser(self); // skip `type` keyword

    // 1. Get type name
    GET_NAME(self, from__String("expected `type` identifier"));

    // 3. Parse generic params
    Vec *generic_params = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);

            break;
        default:
            break;
    }

    // 2. Check kind of type
    enum LilyTokenKind kind_of_type = self->current->kind;

    next_token__LilyPreparser(self); // skip `alias`, `enum` or `record` keyword

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `=`")),
              &self->count_error);

            FREE(String, name);

            return NULL;
    }

    switch (kind_of_type) {
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
            return preparse_alias__LilyPreparser(self, name, generic_params);
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
            return preparse_enum__LilyPreparser(self, name, generic_params);
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
            return preparse_record__LilyPreparser(self, name, generic_params);
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_BAD_KIND_OF_TYPE),
                init__Vec(1,
                          from__String("expected `alias`, `enum` or `record`")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, name);

            return NULL;
    }
}

LilyPreparserRecordField *
preparse_record_field__LilyPreparser(LilyPreparser *self, bool is_mut)
{
    // [ <pub> ] <name> <data_type> [ := <expr> ] ;
    String *name = NULL;
    Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*
    Vec *optional_expr = NULL; // Vec<LilyToken* (&)>*?
    enum LilyVisibility visibility_field = visibility_decl;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            visibility_field = LILY_VISIBILITY_PUBLIC;
            next_token__LilyPreparser(self);
            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);

            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          init__Vec(1, from__String("expected field name")),
                          NULL,
                          NULL),
              &self->count_error);

            name = from__String("__error__");
    }

    PREPARSE_UNTIL(data_type,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_COLON_EQ: {
            next_token__LilyPreparser(self);

            optional_expr = NEW(Vec);

            PREPARSE_UNTIL(optional_expr,
                           self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_SEMICOLON:
                    next_token__LilyPreparser(self);

                    break;
                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        init__Vec(1, from__String("expected `;`")),
                        NULL,
                        NULL),
                      &self->count_error);

                    FREE(Vec, optional_expr);
                    FREE(Vec, data_type);
                    FREE(String, name);

                    return NULL;
                default:
                    UNREACHABLE("this way is impossible");
            }

            break;
        }
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          init__Vec(1, from__String("expected `;`")),
                          NULL,
                          NULL),
              &self->count_error);

            FREE(Vec, data_type);
            FREE(String, name);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW(LilyPreparserRecordField,
               name,
               data_type,
               optional_expr,
               visibility_field,
               is_mut);
}

LilyPreparserRecordBodyItem *
preparse_macro_expand_for_record__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserRecordBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserRecordBodyItem,
                      macro_expand,
                      decl->location,
                      decl->macro_expand);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

Vec *
preparse_record_body__LilyPreparser(LilyPreparser *self)
{
    Vec *body = NEW(Vec); // Vec<LilyPreparserRecordBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserRecordBodyItem *macro_expand =
                              preparse_macro_expand_for_record__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default: {
                            Location location_field =
                              clone__Location(&self->current->location);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserRecordField *field =
                              preparse_record_field__LilyPreparser(self, false);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_field,
                                             previous->location);
                            }

                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            if (field) {
                                push__Vec(
                                  body,
                                  NEW_VARIANT(LilyPreparserRecordBodyItem,
                                              field,
                                              location_field,
                                              *field));
                                lily_free(field);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                    }
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_MUT: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                            Location location_field =
                              clone__Location(&self->current->location);

                            next_token__LilyPreparser(self);

                            visibility_decl = LILY_VISIBILITY_PUBLIC;

                            LilyPreparserRecordField *field =
                              preparse_record_field__LilyPreparser(self, true);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_field,
                                             previous->location);
                            }

                            visibility_decl = LILY_VISIBILITY_PRIVATE;

                            if (field) {
                                push__Vec(
                                  body,
                                  NEW_VARIANT(LilyPreparserRecordBodyItem,
                                              field,
                                              location_field,
                                              *field));

                                lily_free(field);
                            } else {
                                goto clean_up;
                            }
                        } break;
                        default:
                            goto unexpected_token;
                    }
                } else {
                    goto unexpected_token;
                }

                break;
            }
            default: {
            unexpected_token: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected `mut` keyword or identifier")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
            }
        }
    }

    return body;

clean_up: {
    FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserRecordBodyItem);
    FREE(Vec, body);

    return NULL;
}
}

LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self,
                               String *name,
                               Vec *generic_params)
{
    // type <name> [ <generic_params> ] record = <body> end

    // 1. Get visibility
    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    Vec *body = preparse_record_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location, self->current->location);

            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                init__Vec(
                  1, from__String("expected `end` keyword to close record")),
                NULL,
                NULL),
              &self->count_error);

        clean_up: {
            FREE(String, name);

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserRecordBodyItem);
            FREE(Vec, body);

            return NULL;
        }
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      type,
      location,
      NEW_VARIANT(
        LilyPreparserType,
        record,
        NEW(LilyPreparserRecord, name, generic_params, body, visibility)));
}

static LilyPreparserEnumVariant *
preparse_enum_variant__LilyPreparser(LilyPreparser *self)
{
    // <name> [ : <data_type> ] [ ; ]

    // 1. Get variant's name
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                init__Vec(1, from__String("expected variant identifier")),
                NULL,
                NULL),
              &self->count_error);

            name = from__String("__error__");
    }

    // 2. Preparse data type.
    Vec *data_type = NULL; // Vec<LilyToken* (&)>*?

    if (self->current->kind == LILY_TOKEN_KIND_COLON) {
        data_type = NEW(Vec);

        next_token__LilyPreparser(self);

        PREPARSE_UNTIL(data_type,
                       self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                         self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
                         self->current->kind != LILY_TOKEN_KIND_EOF);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            break;
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                init__Vec(
                  1, from__String("expected `;` to close variant declaration")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, name);
            FREE(Vec, data_type);

            return NULL;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `:` or `;`")),
              &self->count_error);

            FREE(String, name);
            FREE(String, current_s);

            next_token__LilyPreparser(self);

            return NULL;
        }
    }

    return NEW(LilyPreparserEnumVariant, name, data_type);
}

LilyPreparserEnumBodyItem *
preparse_macro_expand_for_enum__LilyPreparser(LilyPreparser *self)
{
    LilyPreparserDecl *decl = preparse_macro_expand__LilyPreparser(self);

    if (decl) {
        LilyPreparserEnumBodyItem *macro_expand =
          NEW_VARIANT(LilyPreparserEnumBodyItem,
                      macro_expand,
                      decl->location,
                      decl->macro_expand);

        lily_free(decl);

        return macro_expand;
    }

    return NULL;
}

Vec *
preparse_enum_body__LilyPreparser(LilyPreparser *self)
{
    Vec *body = NEW(Vec); // Vec<LilyPreparserEnumBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_BANG: {
                            LilyPreparserEnumBodyItem *macro_expand =
                              preparse_macro_expand_for_enum__LilyPreparser(
                                self);

                            if (macro_expand) {
                                push__Vec(body, macro_expand);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                        default: {
                            Location location_variant =
                              clone__Location(&self->current->location);
                            LilyPreparserEnumVariant *variant =
                              preparse_enum_variant__LilyPreparser(self);

                            {
                                LilyToken *previous =
                                  self->tokens->buffer[self->position - 1];

                                END_LOCATION(&location_variant,
                                             previous->location);
                            }

                            if (variant) {
                                push__Vec(body,
                                          NEW_VARIANT(LilyPreparserEnumBodyItem,
                                                      variant,
                                                      location_variant,
                                                      *variant));
                                lily_free(variant);
                            } else {
                                goto clean_up;
                            }

                            break;
                        }
                    }
                }

                break;
            }
            default: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    from__String("expected identifier")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
        }
    }

    return body;

clean_up: {
    FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserEnumBodyItem);
    FREE(Vec, body);

    return NULL;
}
}

LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self,
                             String *name,
                             Vec *generic_params)
{
    enum LilyVisibility visibility = visibility_decl;
    Location location = clone__Location(&location_decl);

    Vec *body = preparse_enum_body__LilyPreparser(self);

    if (!body) {
        goto clean_up;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location, self->current->location);

            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                init__Vec(1,
                          from__String("expected `end` keyword to close enum")),
                NULL,
                NULL),
              &self->count_error);

        clean_up: {
            FREE(String, name);

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserEnumBodyItem);
            FREE(Vec, body);

            return NULL;
        }
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      type,
      location,
      NEW_VARIANT(
        LilyPreparserType,
        enum_,
        NEW(LilyPreparserEnum, name, generic_params, body, visibility)));
}

LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *generic_params)
{
    // type <name> [ <generic_params> ] alias = <data_type> [ ; ]
    Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(data_type,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location_decl, self->current->location);

            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                init__Vec(1, from__String("expected `;` to close alias")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, name);
            FREE(Vec, data_type);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       type,
                       location_decl,
                       NEW_VARIANT(LilyPreparserType,
                                   alias,
                                   NEW(LilyPreparserAlias,
                                       name,
                                       generic_params,
                                       data_type,
                                       visibility_decl)));
}

LilyPreparserDecl *
preparse_error__LilyPreparser(LilyPreparser *self)
{
    // error <name> [ <generic_params> ] [ : <data_type> ] ;
    Location location = clone__Location(&location_decl);

    next_token__LilyPreparser(self); // skip `error`

    // 1. Get name of error
    GET_NAME(self, from__String("expected name of the error"));

    // 2. Preparse generic params.
    Vec *generic_params = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);
        default:
            break;
    }

    // 3. Preparse data type of error.
    Vec *data_type = NULL; // Vec<LilyToken* (&)>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_COLON:
            next_token__LilyPreparser(self);

            data_type = NEW(Vec);

            PREPARSE_UNTIL(data_type,
                           self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                             self->current->kind != LILY_TOKEN_KIND_EOF);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_EOF:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                        init__Vec(1,
                                  from__String("expected `;` to close error")),
                        NULL,
                        NULL),
                      &self->count_error);

                    break;
                default:
                    END_LOCATION(&location, self->current->location);
                    next_token__LilyPreparser(self);

                    break;
            }

            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                init__Vec(1, from__String("expected `;` or `:`")),
                NULL,
                NULL),
              &self->count_error);

            FREE(String, current_s);
            FREE(String, name);

            if (generic_params) {
                FREE_BUFFER_ITEMS(
                  generic_params->buffer, generic_params->len, Vec);
                FREE(Vec, generic_params);
            }

            return NULL;
        }
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      error,
      location,
      NEW(
        LilyPreparserError, name, data_type, generic_params, visibility_decl));
}

LilyPreparserDecl *
preparse_use__LilyPreparser(LilyPreparser *self)
{
    // use <path>|<identifier>;
    // <path> = [ <token> ]*
    // <identifier> = token
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self); // skip `use`

    Vec *path = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(path,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);

            break;
        default:
            UNREACHABLE("this situation is impossible");
    }

    if (path->len == 0) {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      self->file,
                      &self->current->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                      NULL,
                      NULL,
                      NULL),
          &self->count_error);
    }

    return NEW_VARIANT(
      LilyPreparserDecl, use, location, NEW(LilyPreparserUse, path));
}

LilyPreparserDecl *
preparse_include__LilyPreparser(LilyPreparser *self)
{
    // include <path>|<identifier>;
    // <path> = [ <token> ]*
    // <identifier> = token
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self); // skip `include`

    Vec *path = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(path,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);

            break;
        default:
            UNREACHABLE("this situation is impossible");
    }

    if (path->len == 0) {
        emit__Diagnostic(
          NEW_VARIANT(Diagnostic,
                      simple_lily_error,
                      self->file,
                      &self->current->location,
                      NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                      NULL,
                      NULL,
                      NULL),
          &self->count_error);
    }

    return NEW_VARIANT(
      LilyPreparserDecl, include, location, NEW(LilyPreparserInclude, path));
}

LilyPreparserDecl *
preparse_macro_expand__LilyPreparser(LilyPreparser *self)
{
    // <name> ! [ ( <params> ) ] ;
    ASSERT(self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_NORMAL ||
           self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_STRING);

    Location location = clone__Location(&self->current->location);

    // 1. Get name of the macro expand.
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            name = clone__String(self->current->identifier_string);

            break;
        default:
            UNREACHABLE("this situation is impossible");
    }

    next_token__LilyPreparser(self);
    next_token__LilyPreparser(self); // skip `!`

    // 2. Get params of the macro expand.
    Vec *params = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN: {
            LilyToken *peeked = peek_token__LilyPreparser(self, 1);

            if (peeked) {
                switch (peeked->kind) {
                    case LILY_TOKEN_KIND_R_PAREN:
                        jump__LilyPreparser(self, 2);

                        break;
                    default:
                        params =
                          preparse_paren_with_comma_sep__LilyPreparser(self);

                        break;
                }
            }

            break;
        }
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       macro_expand,
                       location,
                       NEW(LilyPreparserMacroExpand, name, params));
}

// val <name> <data_type>;
LilyPreparserLibBodyItem *
preparse_lib_constant_prototype__LilyPreparser(LilyPreparser *self)
{
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self);

    // 1. Get name
    GET_NAME(self, from__String("expected name of the constant prototype"));

    // 2. Preparse data type of the constant prototype
    Vec *data_type = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(data_type,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);

            FREE(String, name);

            return NULL;
    }

    return NEW_VARIANT(LilyPreparserLibBodyItem,
                       constant,
                       location,
                       NEW(LilyPreparserLibConstantPrototype, name, data_type));
}

// fun <name>(<params>) <return_data_type> := <new_name>;
LilyPreparserLibBodyItem *
preparse_lib_fun_prototype__LilyPreparser(LilyPreparser *self)
{
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self);

    // 1. Get name
    GET_NAME(self, from__String("expected name of the fun prototype"));

    // 2. Get params
    Vec *params = NULL; // Vec<Vec<LilyToken* (&)>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            params = preparse_paren_with_comma_sep__LilyPreparser(self);

            break;
        default:
            break;
    }

    // 3. Get return data type of fun prototype
    Vec *return_data_type = NEW(Vec); // Vec<LilyToken* (&)>*

    PREPARSE_UNTIL(return_data_type,
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                     self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
                     self->current->kind != LILY_TOKEN_KIND_EOF);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EOF:
            FREE(String, name);

            if (params) {
                FREE_BUFFER_ITEMS(params->buffer, params->len, Vec);
                FREE(Vec, params);
            }

            FREE(Vec, return_data_type);

            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `:=` or `;`")),
              &self->count_error);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    // 4. Get new name of fun prototype
    String *new_name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    new_name = clone__String(self->current->identifier_normal);
                    next_token__LilyPreparser(self);

                    break;
                case LILY_TOKEN_KIND_IDENTIFIER_STRING:
                    new_name = clone__String(self->current->identifier_string);
                    next_token__LilyPreparser(self);

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                        NULL,
                        NULL,
                        from__String("expected identifier after `:=`")),
                      &self->count_error);
            }

            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `;`")),
              &self->count_error);

            break;
    }

    return NEW_VARIANT(LilyPreparserLibBodyItem,
                       fun,
                       location,
                       NEW(LilyPreparserLibFunPrototype,
                           name,
                           new_name,
                           params,
                           return_data_type));
}

// lib("C") App =
// end
LilyPreparserDecl *
preparse_lib__LilyPreparser(LilyPreparser *self)
{
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self);

    // 1. Parse target language.
    enum LilyPreparserLibFrom from = -1;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_LITERAL_STR:
                    if (!strcmp(self->current->literal_str->buffer, "C") ||
                        !strcmp(self->current->literal_str->buffer, "CC")) {
                        from = LILY_PREPARSER_LIB_FROM_CC;
                    } else if (!strcmp(self->current->literal_str->buffer,
                                       "CPP")) {
                        from = LILY_PREPARSER_LIB_FROM_CPP;
                    } else {
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError,
                                LILY_ERROR_KIND_UNKNOWN_FROM_VALUE_IN_LIB),
                            NULL,
                            NULL,
                            NULL),
                          &self->count_error);

                        return NULL;
                    }

                    next_token__LilyPreparser(self);

                    switch (self->current->kind) {
                        case LILY_TOKEN_KIND_R_PAREN:
                            next_token__LilyPreparser(self);

                            break;
                        default:
                            emit__Diagnostic(
                              NEW_VARIANT(
                                Diagnostic,
                                simple_lily_error,
                                self->file,
                                &self->current->location,
                                NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                                NULL,
                                NULL,
                                from__String("expected `)`")),
                              &self->count_error);
                    }

                    break;
                default:
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                        NULL,
                        NULL,
                        from__String("expected string literal")),
                      &self->count_error);
            }

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `(`")),
              &self->count_error);
    }

    // 2. Get name of lib.
    String *name = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_IDENTIFIER_STRING:
            name = clone__String(self->current->identifier_string);
            next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `=`")),
              &self->count_error);
    }

    // 3. Preparse body
    Vec *body = NEW(Vec); // Vec<LilyPreparserLibBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        LilyPreparserLibBodyItem *item = NULL;

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL:
                item = preparse_lib_constant_prototype__LilyPreparser(self);

                break;
            case LILY_TOKEN_KIND_KEYWORD_FUN:
                item = preparse_lib_fun_prototype__LilyPreparser(self);

                break;
            case LILY_TOKEN_KIND_KEYWORD_TYPE:
                TODO("preparse type prototype");
            case LILY_TOKEN_KIND_KEYWORD_OBJECT:
                TODO("preparse object prototype");
            default:
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->file,
                              &self->current->location,
                              NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_TOKEN),
                              NULL,
                              NULL,
                              NULL),
                  &self->count_error);

                break;
        }

        if (item) {
            push__Vec(body, item);
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            END_LOCATION(&location, self->current->location);
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_EOF:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                          NULL,
                          NULL,
                          from__String("expected `end`")),
              &self->count_error);
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(LilyPreparserDecl,
                       lib,
                       location,
                       NEW(LilyPreparserLib, name, from, body));
}

void
preparse_preprocess__LilyPreparser(LilyPreparser *self)
{
    // TODO: Issue #12
}

void
preparse_when_condition__LilyPreparser(LilyPreparser *self)
{
    // TODO: Issue #13

    // when <cond>:
    while (self->current->kind != LILY_TOKEN_KIND_COLON &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        next_token__LilyPreparser(self);
    }

    if (self->current->kind == LILY_TOKEN_KIND_EOF) {
        return;
    }

    next_token__LilyPreparser(self);
}

CONSTRUCTOR(LilyPreparserInfo, LilyPreparserInfo, String *package_name)
{
    return (LilyPreparserInfo){
        .public_imports = NEW(Vec),
        .private_imports = NEW(Vec),
        .public_macros = NEW(Vec),
        .private_macros = NEW(Vec),
        .decls = NEW(Vec),
        .package = NEW(LilyPreparserPackage, package_name),
    };
}

DESTRUCTOR(LilyPreparserInfo, const LilyPreparserInfo *self)
{
    FREE_BUFFER_ITEMS(self->public_imports->buffer,
                      self->public_imports->len,
                      LilyPreparserImport);
    FREE(Vec, self->public_imports);

    FREE_BUFFER_ITEMS(self->private_imports->buffer,
                      self->private_imports->len,
                      LilyPreparserImport);
    FREE(Vec, self->private_imports);

    FREE_BUFFER_ITEMS(self->public_macros->buffer,
                      self->public_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, self->public_macros);

    FREE_BUFFER_ITEMS(self->private_macros->buffer,
                      self->private_macros->len,
                      LilyPreparserMacro);
    FREE(Vec, self->private_macros);

    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyPreparserDecl);
    FREE(Vec, self->decls);

    FREE(LilyPreparserPackage, self->package);
}

CONSTRUCTOR(LilyPreparser,
            LilyPreparser,
            const File *file,
            const Vec *tokens,
            const char *default_package_access,
            bool destroy_all)
{
    return (LilyPreparser){ .file = file,
                            .tokens = tokens,
                            .current = NULL,
                            .position = 0,
                            .count_error = 0,
                            .count_warning = 0,
                            .default_package_access = default_package_access,
                            .destroy_all = destroy_all };
}

void
run__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info)
{
    destroy_all = self->destroy_all;

    self->current = get__Vec(self->tokens, 0);

    bool package_is_preparse = false;

    while (self->current->kind != LILY_TOKEN_KIND_EOF) {
        location_decl = clone__Location(&self->current->location);

        switch (self->current->kind) {
            /*
                import <import_value> [as <as_value>]
            */
            case LILY_TOKEN_KIND_KEYWORD_IMPORT: {
                LilyPreparserImport *import =
                  preparse_import__LilyPreparser(self);

                if (import) {
                    push__Vec(info->private_imports, import);
                }

                break;
            }

            /*
                [pub] macro <name>(<params>) = {
                    <tokens>
                };
            */
            case LILY_TOKEN_KIND_KEYWORD_MACRO: {
                LilyPreparserMacro *macro = preparse_macro__LilyPreparser(self);

                if (macro) {
                    push__Vec(info->private_macros, macro);
                }

                break;
            }

            /*
                package [name] =
                    [pub] .<name>,
                    ...
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_PACKAGE: {
                if (package_is_preparse) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->file,
                        &self->current->location,
                        NEW(LilyError,
                            LILY_ERROR_KIND_DUPLICATE_PACKAGE_DECLARATION),
                        NULL,
                        NULL,
                        NULL),
                      &self->count_error);

                    goto exit_preparser;
                }

                if (!preparse_package__LilyPreparser(self, info)) {
                    goto exit_preparser;
                } else {
                    package_is_preparse = true;
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_PUB:
                visibility_decl = LILY_VISIBILITY_PUBLIC;

                next_token__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_IMPORT: {
                        LilyPreparserImport *import =
                          preparse_import__LilyPreparser(self);

                        if (import) {
                            push__Vec(info->public_imports, import);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_FUN: {
                        LilyPreparserDecl *fun =
                          preparse_fun__LilyPreparser(self);

                        if (fun) {
                            push__Vec(info->decls, fun);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_VAL: {
                        LilyPreparserDecl *constant =
                          preparse_constant__LilyPreparser(self);

                        if (constant) {
                            push__Vec(info->decls, constant);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_MODULE: {
                        LilyPreparserDecl *module =
                          preparse_module__LilyPreparser(self);

                        if (module) {
                            push__Vec(info->decls, module);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_TYPE: {
                        LilyPreparserDecl *type =
                          preparse_type__LilyPreparser(self);

                        if (type) {
                            push__Vec(info->decls, type);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_MACRO: {
                        LilyPreparserMacro *macro =
                          preparse_macro__LilyPreparser(self);

                        if (macro) {
                            push__Vec(info->public_macros, macro);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_CLOSE: {
                    preparse_close: {
                        next_token__LilyPreparser(self);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_object: {
                                LilyPreparserDecl *object =
                                  preparse_object__LilyPreparser(self, true);

                                if (object) {
                                    push__Vec(info->decls, object);
                                }

                                break;
                            }
                            default: {
                                String *current_s =
                                  to_string__LilyToken(self->current);

                                emit__Diagnostic(
                                  NEW_VARIANT(Diagnostic,
                                              simple_lily_error,
                                              self->file,
                                              &self->current->location,
                                              NEW_VARIANT(LilyError,
                                                          unexpected_token,
                                                          current_s->buffer),
                                              NULL,
                                              NULL,
                                              NULL),
                                  &self->count_error);

                                FREE(String, current_s);

                                next_token__LilyPreparser(self);
                            }
                        }

                        break;
                    }
                    }

                    case LILY_TOKEN_KIND_KEYWORD_object: {
                        LilyPreparserDecl *object =
                          preparse_object__LilyPreparser(self, false);

                        if (object) {
                            push__Vec(info->decls, object);
                        }

                        break;
                    }

                    default: {
                        String *current_s = to_string__LilyToken(self->current);

                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW_VARIANT(
                              LilyError, unexpected_token, current_s->buffer),
                            NULL,
                            NULL,
                            from__String("unexpected token after `pub`")),
                          &self->count_error);

                        goto exit_preparser;
                    }
                }

                visibility_decl = LILY_VISIBILITY_PRIVATE;

                break;

            /*
                [pub] module <path> =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_MODULE: {
                LilyPreparserDecl *module =
                  preparse_module__LilyPreparser(self);

                if (module) {
                    push__Vec(info->decls, module);
                }

                break;
            }

            /*
                test <test_name> =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_TEST:
                preparse_test__LilyPreparser(self);

                break;

            /*
                error <error_name>[[<generic_params>]] [:<dt>];
            */
            case LILY_TOKEN_KIND_KEYWORD_ERROR: {
                LilyPreparserDecl *error = preparse_error__LilyPreparser(self);

                if (error) {
                    push__Vec(info->decls, error);
                }

                break;
            }

            /*
                use <path>|<identifier>;
            */
            case LILY_TOKEN_KIND_KEYWORD_USE: {
                LilyPreparserDecl *use = preparse_use__LilyPreparser(self);

                if (use) {
                    push__Vec(info->decls, use);
                }

                break;
            }

            /*
                include <path>|<identifier>;
            */
            case LILY_TOKEN_KIND_KEYWORD_INCLUDE: {
                LilyPreparserDecl *include =
                  preparse_include__LilyPreparser(self);

                if (include) {
                    push__Vec(info->decls, include);
                }

                break;
            }

            /*
                fun[@<object_name>] <name>[[<generic_params>]][(<params>)] [when
               [<cond>] + ..., req
               [<cond>] + ...] <return_type> = <body> end
            */
            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserDecl *fun = preparse_fun__LilyPreparser(self);

                if (fun) {
                    push__Vec(info->decls, fun);
                }

                break;
            }

            /*
                val <name> [data_type] := <expr>;
                val (<name> [data_type], ...) := (<expr>, ...);
            */
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                LilyPreparserDecl *constant =
                  preparse_constant__LilyPreparser(self);

                if (constant) {
                    push__Vec(info->decls, constant);
                }

                break;
            }

            /*
                close object <name>[[<generic_params>]] class =
                    <body>
                end

                close object <name>[[<generic_params>]] trait =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_CLOSE:
                goto preparse_close;

            /*
                object <name>[[<generic_params>]] class =
                    <body>
                end

                object <name>[[<generic_params>]] record =
                    <body>
                end

                object <name>[[<generic_params>]] enum =
                    <body>
                end

                object <name>[[<generic_params>]] trait =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_object: {
                LilyPreparserDecl *object =
                  preparse_object__LilyPreparser(self, false);

                if (object) {
                    push__Vec(info->decls, object);
                }

                break;
            }

            /*
                type <name>[[<generic_params>]] alias = <dt>;

                type <name>[[<generic_params>]] enum =
                    <body>
                end

                type <name>[[<generic_params>]] record =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_TYPE: {
                LilyPreparserDecl *type = preparse_type__LilyPreparser(self);

                if (type) {
                    push__Vec(info->decls, type);
                }

                break;
            }

            case LILY_TOKEN_KIND_KEYWORD_LIB: {
                LilyPreparserDecl *lib = preparse_lib__LilyPreparser(self);

                if (lib) {
                    push__Vec(info->decls, lib);
                }

                break;
            }

            /*
                <name>!(<args>);
            */
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            case LILY_TOKEN_KIND_IDENTIFIER_STRING: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    if (peeked->kind == LILY_TOKEN_KIND_BANG) {
                        LilyPreparserDecl *macro_expand =
                          preparse_macro_expand__LilyPreparser(self);

                        if (macro_expand) {
                            push__Vec(info->decls, macro_expand);
                        }

                        break;
                    } else {
                        goto unexpected_token;
                    }
                } else {
                    goto unexpected_token;
                }

                break;
            }

            case LILY_TOKEN_KIND_COMMENT_DOC:
                next_token__LilyPreparser(self);

                break;

            /*
                #[<proc>]
            */
            case LILY_TOKEN_KIND_HASHTAG:
                preparse_preprocess__LilyPreparser(self);

                break;

            /*
                when <cond>:
                <fun>
            */
            case LILY_TOKEN_KIND_KEYWORD_WHEN:
                preparse_when_condition__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_FUN: {
                        LilyPreparserDecl *fun =
                          preparse_fun__LilyPreparser(self);

                        if (fun) {
                            push__Vec(info->decls, fun);
                        }

                        break;
                    }

                    case LILY_TOKEN_KIND_KEYWORD_PUB:
                        next_token__LilyPreparser(self);

                        switch (self->current->kind) {
                            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                                next_token__LilyPreparser(self);

                                LilyPreparserDecl *fun =
                                  preparse_fun__LilyPreparser(self);

                                if (fun) {
                                    push__Vec(info->decls, fun);
                                }

                                break;
                            }
                            default:
                                goto unexpected_token;
                        }

                        break;

                    case LILY_TOKEN_KIND_EOF:
                        emit__Diagnostic(
                          NEW_VARIANT(
                            Diagnostic,
                            simple_lily_error,
                            self->file,
                            &self->current->location,
                            NEW(LilyError, LILY_ERROR_KIND_EOF_NOT_EXPECTED),
                            NULL,
                            NULL,
                            from__String(
                              "expected `fun` or `pub` keyword after `:`")),
                          &self->count_error);

                        break;
                    default:
                        goto unexpected_token;
                }

                break;

            default: {
            unexpected_token: {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
            }
        }
    }

exit_preparser: {
}

#ifdef DEBUG_PREPARSER
    printf("\n====Preparser(%s)====\n", self->file->name);

    for (Usize i = 0; i < self->tokens->len; ++i) {
        CALL_DEBUG(LilyToken, get__Vec(self->tokens, i));
    }

    printf("\n====Preparser public imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->public_imports->len; ++i) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(info->public_imports, i));
    }

    printf("\n====Preparser private imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->private_imports->len; ++i) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(info->private_imports, i));
    }

    printf("\n====Preparser public macros(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->public_macros->len; ++i) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(info->public_macros, i));
    }

    printf("\n====Preparser private macros(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->private_macros->len; ++i) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(info->private_macros, i));
    }

    printf("\n====Preparser decls(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->decls->len; ++i) {
        CALL_DEBUG(LilyPreparserDecl, get__Vec(info->decls, i));
    }

    printf("\n====Package(%s)====\n", self->file->name);

    CALL_DEBUG(LilyPreparserPackage, info->package);
#endif

    if (self->count_error > 0) {
        exit(1);
    }
}
