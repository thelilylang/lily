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

#include <base/new.h>

#include <core/lily/error.h>
#include <core/lily/lily.h>
#include <core/lily/preparser.h>
#include <core/lily/token.h>
#include <core/shared/diagnostic.h>

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
                   Vec *tokens,
                   Location location);

// Construct LilyPreparserSubPackage type.
static CONSTRUCTOR(LilyPreparserSubPackage *,
                   LilyPreparserSubPackage,
                   enum LilyVisibility visibility,
                   String *name);

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

// Construct LilyPreparserFunBodyItemStmtBlock type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
                          LilyPreparserFunBodyItemStmtBlock,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtBlock type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtBlock,
                  const LilyPreparserFunBodyItemStmtBlock *self);

// Construct LilyPreparserFunBodyItemStmtFor type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtFor,
                          LilyPreparserFunBodyItemStmtFor,
                          Vec *expr,
                          Vec *block);

// Free LilyPreparserFunBodyItemStmtFor type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtFor,
                  const LilyPreparserFunBodyItemStmtFor *self);

// Construct LilyPreparserFunBodyItemStmtIf type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtIf,
                          LilyPreparserFunBodyItemStmtIf,
                          Vec *if_expr,
                          Vec *if_block,
                          Vec *elif_exprs,
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

// Construct LilyPreparserFunBodyItemStmtTry type.
static inline CONSTRUCTOR(LilyPreparserFunBodyItemStmtTry,
                          LilyPreparserFunBodyItemStmtTry,
                          Vec *block,
                          Vec *catch_expr,
                          Vec *catch_block);

// Free LilyPreparserFunBodyItemStmtTry type.
static DESTRUCTOR(LilyPreparserFunBodyItemStmtTry,
                  const LilyPreparserFunBodyItemStmtTry *self);

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
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_block,
                           LilyPreparserFunBodyItemStmtBlock stmt_block,
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
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY).
static VARIANT_CONSTRUCTOR(LilyPreparserFunBodyItem *,
                           LilyPreparserFunBodyItem,
                           stmt_try,
                           LilyPreparserFunBodyItemStmtTry stmt_try,
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
                          expr,
                          LilyPreparserFunBodyItem *self);

// Free LilyPreparserFunBodyItem type
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_block,
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
// (LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY).
static VARIANT_DESTRUCTOR(LilyPreparserFunBodyItem,
                          stmt_try,
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

// Freee LilyPreparserFunBodyItem type.
static DESTRUCTOR(LilyPreparserFunBodyItem, LilyPreparserFunBodyItem *self);

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
// (LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserClassBodyItem,
                          method,
                          LilyPreparserClassBodyItem *self);

// Free LilyPreparserClassBodyItem type.
static DESTRUCTOR(LilyPreparserClassBodyItem, LilyPreparserClassBodyItem *self);

// Construct LilyPreparserClass type.
static inline CONSTRUCTOR(LilyPreparserClass,
                          LilyPreparserClass,
                          String *name,
                          Vec *generic_params,
                          Vec *inherits,
                          Vec *implements,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserClass type.
static DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self);

// Construct LilyPreparserPrototype type.
static inline CONSTRUCTOR(LilyPreparserPrototype,
                          LilyPreparserPrototype,
                          String *name,
                          Vec *generic_params,
                          Vec *params);

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
// (LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE).
static VARIANT_DESTRUCTOR(LilyPreparserTraitBodyItem,
                          prototype,
                          LilyPreparserTraitBodyItem *self);

// Free LilyPreparserTraitBodyItem type.
static DESTRUCTOR(LilyPreparserTraitBodyItem, LilyPreparserTraitBodyItem *self);

// Construct LilyPreparserTrait type.
static inline CONSTRUCTOR(LilyPreparserTrait,
                          LilyPreparserTrait,
                          String *name,
                          Vec *generic_params,
                          Vec *inherits,
                          Vec *body,
                          enum LilyVisibility visibility);

// Free LilyPreparserTrait type.
static DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self);

// Construct LilyPreparserRecordField type.
static CONSTRUCTOR(LilyPreparserRecordField *,
                   LilyPreparserRecordField,
                   String *name,
                   Vec *data_type,
                   Vec *optional_expr,
                   Location location,
                   enum LilyVisibility visibility);

// Free LilyPreparserRecordField type.
static DESTRUCTOR(LilyPreparserRecordField, LilyPreparserRecordField *self);

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
// (LILY_PREPARSER_RECORD_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
                          method,
                          LilyPreparserRecordObjectBodyItem *self);

// Free LilyPreparserRecordObjectBodyItem type.
static DESTRUCTOR(LilyPreparserRecordObjectBodyItem,
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
                          Vec *data_type,
                          Location location);

// Free LilyPreparserEnumVariant type.
static DESTRUCTOR(LilyPreparserEnumVariant, LilyPreparserEnumVariant *self);

// Construct LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_CONSTANT).
static VARIANT_CONSTRUCTOR(LilyPreparserEnumObjectBodyItem *,
                           LilyPreparserEnumObjectBodyItem,
                           constant,
                           Location location,
                           LilyPreparserConstant constant);

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
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_METHOD).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          method,
                          LilyPreparserEnumObjectBodyItem *self);

// Free LilyPreparserEnumObjectBodyItem type
// (LILY_PREPARSER_ENUM_OBJECT_ITEM_KIND_VARIANT).
static VARIANT_DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
                          variant,
                          LilyPreparserEnumObjectBodyItem *self);

// Free LilyPreparserEnumObjectBodyItem type.
static DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
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
                          Vec *data_type,
                          enum LilyVisibility visibility);

// Free LilyPreparserAlias type.
static DESTRUCTOR(LilyPreparserAlias, const LilyPreparserAlias *self);

// Construct LilyPreparserEnum type.
static inline CONSTRUCTOR(LilyPreparserEnum,
                          LilyPreparserEnum,
                          String *name,
                          Vec *variants,
                          enum LilyVisibility visibility);

// Free LilyPreparserEnum type.
static DESTRUCTOR(LilyPreparserEnum, const LilyPreparserEnum *self);

// Construct LilyPreparserRecord type.
static inline CONSTRUCTOR(LilyPreparserRecord,
                          LilyPreparserRecord,
                          String *name,
                          Vec *fields,
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

// Construct LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_FUN).
static VARIANT_CONSTRUCTOR(LilyPreparserDecl *,
                           LilyPreparserDecl,
                           fun,
                           Location location,
                           LilyPreparserFun fun);

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

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_CONSTANT).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, constant, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_FUN).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, fun, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_MODULE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, module, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_OBJECT).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, object, LilyPreparserDecl *self);

// Free LilyPreparserDecl type (LILY_PREPARSER_DECL_KIND_TYPE).
static VARIANT_DESTRUCTOR(LilyPreparserDecl, type, LilyPreparserDecl *self);

// Free LilyPreparserDecl type.
static DESTRUCTOR(LilyPreparserDecl, LilyPreparserDecl *self);

// Advance to the one position and update the current.
static void
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
/// @return Vec<Vec<LilyToken*>*>*
static Vec *
preparse_paren_with_comma_sep__LilyPreparser(LilyPreparser *self);

/// @skip Push all tokens between left and right hook.
/// @note Works only with comma separator
/// @return Vec<Vec<LilyToken*>*>*
static Vec *
preparse_hook_with_comma_sep__LilyPreparser(LilyPreparser *self);

/// @skip Push all tokens between left and right brace.
/// @note Works only with comma separator
/// @return Vec<Vec<LilyToken*>*>*
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

/// @brief Check if the basic block could be closed.
static inline bool
must_close_basic_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the basic brace block could be closed.
static inline bool
must_close_basic_brace_block__LilyPreparser(LilyPreparser *self);

/// @brief Check if the `fun` block could be closed.
static inline bool
must_close_fun_block__LilyPreparser(LilyPreparser *self);

static void
preparse_if_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_for_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_while_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_try_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_match_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_basic_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_basic_brace_block__LilyPreparser(LilyPreparser *self, Vec *body);

static void
preparse_variable_block__LilyPreparser(LilyPreparser *self,
                                       Vec *body,
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
static Vec *
preparse_body__LilyPreparser(LilyPreparser *self,
                             bool (*must_close)(LilyPreparser *));

static LilyPreparserDecl *
preparse_fun__LilyPreparser(LilyPreparser *self);

static LilyPreparserClassBodyItem *
preparse_attribute_for_class__LilyPreparser(LilyPreparser *self,
                                            Location location);

static LilyPreparserTraitBodyItem *
preparse_attribute_for_trait__LilyPreparser(LilyPreparser *self,
                                            Location location);

static LilyPreparserClassBodyItem *
preparse_method_for_class__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *impls,
                              Vec *inherits,
                              Vec *generic_params);

static LilyPreparserTraitBodyItem *
preparse_prototype__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *inherits,
                              Vec *generic_params);

static LilyPreparserRecordObjectBodyItem *
preparse_method_for_record__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self,
                                      String *name,
                                      Vec *impls,
                                      Vec *generic_params);

static LilyPreparserEnumObjectBodyItem *
preparse_method_for_enum__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self,
                                    String *name,
                                    Vec *impls,
                                    Vec *generic_params);

static LilyPreparserDecl *
preparse_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_type__LilyPreparser(LilyPreparser *self);

static LilyPreparserRecordField *
preparse_record_field__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self, String *name);

static LilyPreparserEnumVariant *
preparse_enum_variant__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self, String *name);

static LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self, String *name);

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
                        push__Vec(item, clone__LilyToken(self->current));      \
                        next_token__LilyPreparser(self);                       \
                    } while (self->current->kind != LILY_TOKEN_KIND_R_BRACE);  \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_L_HOOK:                                   \
                    do {                                                       \
                        push__Vec(item, clone__LilyToken(self->current));      \
                        next_token__LilyPreparser(self);                       \
                    } while (self->current->kind != LILY_TOKEN_KIND_R_HOOK);   \
                                                                               \
                    break;                                                     \
                case LILY_TOKEN_KIND_L_PAREN:                                  \
                    do {                                                       \
                        push__Vec(item, clone__LilyToken(self->current));      \
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
                push__Vec(item, clone__LilyToken(self->current));              \
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

static enum LilyVisibility visibility_decl = LILY_VISIBILITY_PRIVATE;
static Location location_decl;
static Location location_fun_body_item;

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

#ifdef RUN_UNTIL_PREPARSER
    if (self->as) {
        FREE(String, self->as);
    }
#endif

    lily_free(self);
}

CONSTRUCTOR(LilyPreparserMacro *,
            LilyPreparserMacro,
            String *name,
            Vec *tokens,
            Location location)
{
    LilyPreparserMacro *self = lily_malloc(sizeof(LilyPreparserMacro));

    self->name = name;
    self->tokens = tokens;
    self->location = location;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyPreparserMacro, const LilyPreparserMacro *self)
{
    String *res = format__String(
      "LilyPreparserMacro{{ name = {S}, tokens = {{ ", self->name);

    DEBUG_VEC_STR(self->tokens, res, LilyToken);

    push_str__String(res, ", location = ");

    char *location_s = to_string__Debug__Location(&self->location);

    push_str__String(res, location_s);
    push_str__String(res, " }");

    lily_free(location_s);

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
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, LilyToken);
    FREE(Vec, self->tokens);
    lily_free(self);
}

CONSTRUCTOR(LilyPreparserSubPackage *,
            LilyPreparserSubPackage,
            enum LilyVisibility visibility,
            String *name)
{
    LilyPreparserSubPackage *self =
      lily_malloc(sizeof(LilyPreparserSubPackage));

    self->name = name;
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
            return format("LilyPreparserSubPackage{{ name = {S}, visibility = "
                          "LILY_VISIBILITY_PRIVATE }",
                          self->name);
        case LILY_VISIBILITY_PUBLIC:
            return format("LilyPreparserSubPackage{{ name = {S}, visibility = "
                          "LILY_VISIBILITY_PUBLIC }",
                          self->name);
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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

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
#ifdef RUN_UNTIL_PREPARSER
    if (self->name) {
        FREE(String, self->name);
    }
#endif

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
    String *res = format__String("LilyPreparserModule{{ name = {S}, body = {{ ",
                                 self->name);

    for (Usize i = 0; i < self->body->len; i++) {
        // String *s =
        //   to_string__Debug__LilyPreparserDecl(get__Vec(self->body, i));

        // APPEND_AND_FREE(res, s);

        // if (i != self->body->len - 1) {
        //     push_str__String(res, ", ");
        // }
    }

    push_str__String(res, " }, visibility = ");
    push_str__String(res, to_string__Debug__LilyVisibility(self->visibility));
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserModule, const LilyPreparserModule *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyToken);
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
    FREE_BUFFER_ITEMS(self->tokens->buffer, self->tokens->len, LilyToken);
    FREE(Vec, self->tokens);
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

    DEBUG_VEC_STR(self->block, res, LilyToken);

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

CONSTRUCTOR(LilyPreparserFunBodyItemStmtFor,
            LilyPreparserFunBodyItemStmtFor,
            Vec *expr,
            Vec *block)
{
    return (LilyPreparserFunBodyItemStmtFor){ .expr = expr, .block = block };
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyPreparserFunBodyItemStmtFor,
               const LilyPreparserFunBodyItemStmtFor *self)
{
    String *res = from__String("LilyPreparserFunBodyItemStmtFor{ expr =");

    DEBUG_VEC_STR(self->expr, res, LilyToken);
    push_str__String(res, ", block =");

    DEBUG_VEC_STR(self->block, res, LilyToken);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserFunBodyItemStmtFor,
           const LilyPreparserFunBodyItemStmtFor *self)
{
    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
    FREE(Vec, self->expr);
    FREE_BUFFER_ITEMS(
      self->block->buffer, self->block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->block);
}

CONSTRUCTOR(LilyPreparserFunBodyItemStmtIf,
            LilyPreparserFunBodyItemStmtIf,
            Vec *if_expr,
            Vec *if_block,
            Vec *elif_exprs,
            Vec *elif_blocks,
            Vec *else_block)
{
    return (LilyPreparserFunBodyItemStmtIf){ .if_expr = if_expr,
                                             .if_block = if_block,
                                             .elif_exprs = elif_exprs,
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
    FREE_BUFFER_ITEMS(self->if_expr->buffer, self->if_expr->len, LilyToken);
    FREE(Vec, self->if_expr);

    FREE_BUFFER_ITEMS(
      self->if_block->buffer, self->if_block->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->if_block);

    if (self->elif_exprs) {
        FREE_BUFFER_ITEMS_2(
          self->elif_exprs->buffer, self->elif_exprs->len, LilyToken);
        FREE(Vec, self->elif_exprs);
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

    for (Usize i = 0; i < self->pattern_conds->len; i++) {
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
    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
    FREE(Vec, self->expr);

    FREE_BUFFER_ITEMS_2(self->patterns->buffer, self->patterns->len, LilyToken);
    FREE(Vec, self->patterns);

    for (Usize i = 0; i < self->pattern_conds->len; i++) {
        Vec *item = get__Vec(self->pattern_conds, i);

        if (item) {
            FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
            FREE(Vec, item);
        }
    }

    FREE(Vec, self->pattern_conds);

    FREE_BUFFER_ITEMS(
      self->blocks->buffer, self->blocks->len, LilyPreparserFunBodyItem);
    FREE(Vec, self->blocks);
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

    if (self->catch_expr) {
        FREE_BUFFER_ITEMS(
          self->catch_expr->buffer, self->catch_expr->len, LilyToken);
        FREE(Vec, self->catch_expr);
    }

    if (self->catch_block) {
        FREE_BUFFER_ITEMS(self->catch_block->buffer,
                          self->catch_block->len,
                          LilyPreparserFunBodyItem);
        FREE(Vec, self->catch_block);
    }
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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->data_type) {
        FREE_BUFFER_ITEMS(
          self->data_type->buffer, self->data_type->len, LilyToken);
        FREE(Vec, self->data_type);
    }

    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
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
    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
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
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_FOR";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_IF";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_MATCH";
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            return "LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY";
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
      format__String("LilyPreparserFunBodyItem{ kind = {s}, location = {sa}",
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
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtBlock(
              &self->stmt_block);

            push_str__String(res, ", stmt_block = ");
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
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY: {
            String *s = to_string__Debug__LilyPreparserFunBodyItemStmtTry(
              &self->stmt_try);

            push_str__String(res, ", stmt_try = ");
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
                   expr,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemExprs, &self->exprs);
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
                   stmt_try,
                   LilyPreparserFunBodyItem *self)
{
    FREE(LilyPreparserFunBodyItemStmtTry, &self->stmt_try);
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
            FREE_VARIANT(LilyPreparserFunBodyItem, expr, self);
            break;
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_BLOCK:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_block, self);
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
        case LILY_PREPARSER_FUN_BODY_ITEM_KIND_STMT_TRY:
            FREE_VARIANT(LilyPreparserFunBodyItem, stmt_try, self);
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
    DEBUG_VEC_STR(self->return_data_type, res, LilyToken);

    push_str__String(res, ", body =");
    DEBUG_VEC_STRING(self->body, res, LilyPreparserFunBodyItem);

    push_str__String(res, ", req =");
    DEBUG_VEC_STR_2(self->req, res, LilyToken);

    push_str__String(res, ", when =");
    DEBUG_VEC_STR_2(self->when, res, LilyToken);

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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);

    if (self->object_impl) {
        FREE(String, self->object_impl);
    }
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken)
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS_2(self->params->buffer, self->params->len, LilyToken);
        FREE(Vec, self->params);
    }

    FREE_BUFFER_ITEMS(
      self->return_data_type->buffer, self->return_data_type->len, LilyToken);
    FREE(Vec, self->return_data_type);

    FREE_BUFFER_ITEMS(self->req->buffer, self->req->len, LilyToken);
    FREE(Vec, self->req);

    FREE_BUFFER_ITEMS(self->when->buffer, self->when->len, LilyToken);
    FREE(Vec, self->when);

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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
    FREE(Vec, self->expr);
    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
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
        case LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD:
            return "LILY_PREPARSER_CLASS_BODY_ITEM_KIND_METHOD";
        default:
            UNREACHABLE("unknown variant");
    }
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
      "LilyPreparserAttribute{ name = {S}, data_type =", self->name);

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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
    FREE(Vec, self->data_type);

    if (self->default_expr) {
        FREE_BUFFER_ITEMS(
          self->default_expr->buffer, self->default_expr->len, LilyToken);
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
            enum LilyVisibility visibility)
{
    return (LilyPreparserClass){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .implements = implements,
                                 .body = body };
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

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken);
        FREE(Vec, self->generic_params);
    }

    if (self->inherits) {
        FREE_BUFFER_ITEMS_2(
          self->inherits->buffer, self->inherits->len, LilyToken);
        FREE(Vec, self->inherits);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS_2(
          self->implements->buffer, self->implements->len, LilyToken);
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
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE:
            return "LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_PROTOTYPE";
        case LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE:
            return "LILY_PREPARSER_TRAIT_BODY_ITEM_KIND_ATTRIBUTE";
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

CONSTRUCTOR(LilyPreparserPrototype,
            LilyPreparserPrototype,
            String *name,
            Vec *generic_params,
            Vec *params)
{
    return (LilyPreparserPrototype){ .name = name,
                                     .generic_params = generic_params,
                                     .params = params };
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

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserPrototype, const LilyPreparserPrototype *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken);
        FREE(Vec, self->generic_params);
    }

    if (self->params) {
        FREE_BUFFER_ITEMS_2(self->params->buffer, self->params->len, LilyToken);
        FREE(Vec, self->params);
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
      "LilyPreparserTraitBodyItem{ kind = {s}, location = {sa}",
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
            enum LilyVisibility visibility)
{
    return (LilyPreparserTrait){ .name = name,
                                 .generic_params = generic_params,
                                 .inherits = inherits,
                                 .body = body,
                                 .visibility = visibility };
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

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken);
        FREE(Vec, self->generic_params);
    }

    if (self->inherits) {
        FREE_BUFFER_ITEMS_2(
          self->inherits->buffer, self->inherits->len, LilyToken);
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
            Location location,
            enum LilyVisibility visibility)
{
    LilyPreparserRecordField *self =
      lily_malloc(sizeof(LilyPreparserRecordField));

    self->name = name;
    self->data_type = data_type;
    self->optional_expr = optional_expr;
    self->visibility = visibility;
    self->location = location;

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

    push_str__String(res, ", visibility = ");
    push_str__String(res, to_string__Debug__LilyVisibility(self->visibility));

    {
        char *s = format(", location = {sa} }",
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserRecordField, LilyPreparserRecordField *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
    FREE(Vec, self->data_type);

    if (self->optional_expr) {
        FREE_BUFFER_ITEMS(
          self->optional_expr->buffer, self->optional_expr->len, LilyToken);
        FREE(Vec, self->optional_expr);
    }

    lily_free(self);
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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->field.name);
#endif

    FREE_BUFFER_ITEMS(
      self->field.data_type->buffer, self->field.data_type->len, LilyToken);
    FREE(Vec, self->field.data_type);

    if (self->field.optional_expr) {
        FREE_BUFFER_ITEMS(self->field.optional_expr->buffer,
                          self->field.optional_expr->len,
                          LilyToken);
        FREE(Vec, self->field.optional_expr);
    }

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
        char *s = format(", visibility = {s} }", self->visibility);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserRecordObject, const LilyPreparserRecordObject *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken);
        FREE(Vec, self->generic_params);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS_2(
          self->implements->buffer, self->implements->len, LilyToken);
        FREE(Vec, self->implements);
    }

    FREE_BUFFER_ITEMS(
      self->body->buffer, self->body->len, LilyPreparserRecordObjectBodyItem);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserEnumVariant *,
            LilyPreparserEnumVariant,
            String *name,
            Vec *data_type,
            Location location)
{
    LilyPreparserEnumVariant *self =
      lily_malloc(sizeof(LilyPreparserEnumVariant));

    self->name = name;
    self->data_type = data_type;
    self->location = location;

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

    DEBUG_VEC_STR(self->data_type, res, LilyToken);

    {
        char *s = format(", location = {sa} }",
                         to_string__Debug__Location(&self->location));

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserEnumVariant, LilyPreparserEnumVariant *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
    FREE(Vec, self->data_type);
    lily_free(self);
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
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->variant.name);
#endif

    FREE_BUFFER_ITEMS(
      self->variant.data_type->buffer, self->variant.data_type->len, LilyToken);
    FREE(Vec, self->variant.data_type);

    lily_free(self);
}

DESTRUCTOR(LilyPreparserEnumObjectBodyItem,
           LilyPreparserEnumObjectBodyItem *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_ENUM_OBJECT_BODY_ITEM_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserEnumObjectBodyItem, constant, self);
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
        char *s = format(", visibility = {s} }", self->visibility);

        PUSH_STR_AND_FREE(res, s);
    }

    return res;
}
#endif

DESTRUCTOR(LilyPreparserEnumObject, const LilyPreparserEnumObject *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    if (self->generic_params) {
        FREE_BUFFER_ITEMS_2(
          self->generic_params->buffer, self->generic_params->len, LilyToken);
        FREE(Vec, self->generic_params);
    }

    if (self->implements) {
        FREE_BUFFER_ITEMS_2(
          self->implements->buffer, self->implements->len, LilyToken);
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
            FREE(LilyPreparserClass, &self->class);
            break;
        case LILY_PREPARSER_OBJECT_KIND_ENUM:
            FREE(LilyPreparserEnumObject, &self->enum_);
            break;
        case LILY_PREPARSER_OBJECT_KIND_RECORD:
            FREE(LilyPreparserRecordObject, &self->record);
            break;
        case LILY_PREPARSER_OBJECT_KIND_TRAIT:
            FREE(LilyPreparserTrait, &self->trait);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyPreparserAlias,
            LilyPreparserAlias,
            String *name,
            Vec *data_type,
            enum LilyVisibility visibility)
{
    return (LilyPreparserAlias){ .name = name,
                                 .data_type = data_type,
                                 .visibility = visibility };
}

DESTRUCTOR(LilyPreparserAlias, const LilyPreparserAlias *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
    FREE(Vec, self->data_type);
}

CONSTRUCTOR(LilyPreparserEnum,
            LilyPreparserEnum,
            String *name,
            Vec *variants,
            enum LilyVisibility visibility)
{
    return (LilyPreparserEnum){ .name = name,
                                .variants = variants,
                                .visibility = visibility };
}

DESTRUCTOR(LilyPreparserEnum, const LilyPreparserEnum *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(
      self->variants->buffer, self->variants->len, LilyPreparserEnumVariant);
    FREE(Vec, self->variants);
}

CONSTRUCTOR(LilyPreparserRecord,
            LilyPreparserRecord,
            String *name,
            Vec *fields,
            enum LilyVisibility visibility)
{
    return (LilyPreparserRecord){ .name = name,
                                  .fields = fields,
                                  .visibility = visibility };
}

DESTRUCTOR(LilyPreparserRecord, const LilyPreparserRecord *self)
{
#ifdef RUN_UNTIL_PREPARSER
    FREE(String, self->name);
#endif

    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyPreparserRecordField);
    FREE(Vec, self->fields);
}

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
            FREE(LilyPreparserAlias, &self->alias);
            break;
        case LILY_PREPARSER_TYPE_KIND_ENUM:
            FREE(LilyPreparserEnum, &self->enum_);
            break;
        case LILY_PREPARSER_TYPE_KIND_RECORD:
            FREE(LilyPreparserRecord, &self->record);
            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

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

VARIANT_DESTRUCTOR(LilyPreparserDecl, constant, LilyPreparserDecl *self)
{
    FREE(LilyPreparserConstant, &self->constant);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyPreparserDecl, fun, LilyPreparserDecl *self)
{
    FREE(LilyPreparserFun, &self->fun);
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

DESTRUCTOR(LilyPreparserDecl, LilyPreparserDecl *self)
{
    switch (self->kind) {
        case LILY_PREPARSER_DECL_KIND_CONSTANT:
            FREE_VARIANT(LilyPreparserDecl, constant, self);
            break;

        case LILY_PREPARSER_DECL_KIND_FUN:
            FREE_VARIANT(LilyPreparserDecl, fun, self);
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

        default:
            UNREACHABLE("unknown variant");
    }
}

void
next_token__LilyPreparser(LilyPreparser *self)
{
    if (self->position + 1 < self->tokens->len) {
        ++self->position;
        self->current = get__Vec(self->tokens, self->position);
    }
}

void
jump__LilyPreparser(LilyPreparser *self, Usize n)
{
    while (n--) {
        next_token__LilyPreparser(self);
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
    String *import_value = NULL;
    String *as_value = NULL;

    next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_LITERAL_STRING:
            import_value = clone__String(self->current->literal_string);
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

    end__Location(&location_decl,
                  self->current->location.end_line,
                  self->current->location.end_column);

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
    String *name = NULL;
    Vec *tokens = NEW(Vec);

    next_token__LilyPreparser(self);

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
                          NULL,
                          NULL,
                          NULL),
              &self->count_error);

            FREE(Vec, tokens);

            return NULL;
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

get_tokens : {
    while (self->current->kind != LILY_TOKEN_KIND_R_BRACE &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(tokens, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

    next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);
            break;
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

    end__Location(&location_decl,
                  self->current->location.end_line,
                  self->current->location.end_column);

    return NEW(LilyPreparserMacro, name, tokens, location_decl);
}

int
preparse_package__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info)
{
    next_token__LilyPreparser(self);

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

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {

        String *sub_pkg_name = NULL;
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
            case LILY_TOKEN_KIND_DOT: {
            get_pkg_name : {
                next_token__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                        sub_pkg_name =
                          clone__String(self->current->identifier_normal);
                        next_token__LilyPreparser(self);

                        while (self->current->kind == LILY_TOKEN_KIND_DOT) {
                            push_str__String(sub_pkg_name, ".");

                            next_token__LilyPreparser(self);

                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                                    push_str__String(
                                      sub_pkg_name,
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

                        break;
                    default:
                    expected_sub_pkg_name : {
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
            }
            default: {
            expected_dot : {
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
        }

        push__Vec(info->package->sub_packages,
                  NEW(LilyPreparserSubPackage, visibility, sub_pkg_name));
    }

    next_token__LilyPreparser(self);

    return 1;
}

LilyPreparserDecl *
preparse_module_body__LilyPreparser(LilyPreparser *self)
{
    location_decl = clone__Location(&self->current->location);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            visibility_decl = LILY_VISIBILITY_PUBLIC;

            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_KEYWORD_TYPE:
                    return preparse_type__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_MODULE:
                    return preparse_module__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_FUN:
                    return preparse_fun__LilyPreparser(self);
                case LILY_TOKEN_KIND_KEYWORD_object:
                    return preparse_object__LilyPreparser(self);
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

                    FREE(String, current_s);

                    return NULL;
                }
            }
            break;
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
            return preparse_type__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
            return preparse_module__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            return preparse_fun__LilyPreparser(self);
        case LILY_TOKEN_KIND_KEYWORD_object:
            return preparse_object__LilyPreparser(self);
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_BANG:
                    break;
                default:
                    return NULL;
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
                NULL),
              &self->count_error);

            FREE(String, current_s);

            next_token__LilyPreparser(self);

            return NULL;
        }
    }
}

LilyPreparserDecl *
preparse_module__LilyPreparser(LilyPreparser *self)
{
    next_token__LilyPreparser(self);

    String *name = NULL;
    Vec *body = NEW(Vec);

    // 1. Get the name
parse_module_name : {
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

    end__Location(&module_location,
                  self->current->location.end_line,
                  self->current->location.end_column);

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
must_close_basic_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_KEYWORD_END ||
           self->current->kind == LILY_TOKEN_KIND_EOF;
}

bool
must_close_basic_brace_block__LilyPreparser(LilyPreparser *self)
{
    return self->current->kind == LILY_TOKEN_KIND_R_HOOK;
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

void
preparse_if_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;
    Vec *if_expr = NEW(Vec);
    Vec *if_block = NULL;

    next_token__LilyPreparser(self); // skip `if` keyword

    // 1. Preparse `if` expression
    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(if_expr, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

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
              "expected `do` keyword after `if` statement expression")),
          &self->count_error);

        FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
        FREE(Vec, if_expr);

        return;
    }

    next_token__LilyPreparser(self);

    // 2. Preparse `if` block
    if_block =
      preparse_body__LilyPreparser(self, &must_close_if_block__LilyPreparser);

    if (!if_block) {
        FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
        FREE(Vec, if_expr);

        return;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_ELIF: {
            // 3(A). Preparse `elif` statement. Similar process than `if`
            // statement preparsing.

            Vec *elif_exprs = NEW(Vec);
            Vec *elif_blocks = NEW(Vec);

            while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_ELSE &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                Vec *elif_expr = NEW(Vec);
                Vec *elif_block = NULL;

                next_token__LilyPreparser(self); // skip `elif` keyword

                // 3(1A). Preparse `elif` expression.
                while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                       self->current->kind != LILY_TOKEN_KIND_EOF) {
                    push__Vec(elif_expr, clone__LilyToken(self->current));
                    next_token__LilyPreparser(self);
                }

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
                        from__String("expected `do` keyword after `elif` "
                                     "statement expression")),
                      &self->count_error);

                    goto clean_up_elif;
                }

                next_token__LilyPreparser(self);

                // 3(2A). Preparse `elif` block.
                elif_block = preparse_body__LilyPreparser(
                  self, &must_close_if_block__LilyPreparser);

                if (!elif_block) {
                    // Clean up allocations

                clean_up_elif : {
                    FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
                    FREE(Vec, if_expr);

                    FREE_BUFFER_ITEMS(if_block->buffer,
                                      if_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, if_block);

                    FREE_BUFFER_ITEMS_2(
                      elif_exprs->buffer, elif_exprs->len, LilyToken);
                    FREE(Vec, elif_exprs);

                    FREE_BUFFER_ITEMS_2(elif_blocks->buffer,
                                        elif_blocks->len,
                                        LilyPreparserFunBodyItem);
                    FREE(Vec, elif_blocks);

                    return;
                }
                }

                push__Vec(elif_exprs, elif_expr);
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
                            end__Location(&location,
                                          self->current->location.end_line,
                                          self->current->location.end_column);

                            LilyPreparserFunBodyItem *item =
                              NEW_VARIANT(LilyPreparserFunBodyItem,
                                          stmt_if,
                                          NEW(LilyPreparserFunBodyItemStmtIf,
                                              if_expr,
                                              if_block,
                                              elif_exprs,
                                              elif_blocks,
                                              else_block),
                                          location);

                            push__Vec(body, item);
                        }

                        default:
                            UNREACHABLE("this way is impossible");
                    }

                    break;
                }

                case LILY_TOKEN_KIND_KEYWORD_END: {
                    end__Location(&location,
                                  self->current->location.end_line,
                                  self->current->location.end_column);
                    next_token__LilyPreparser(self); // skip `end` keyword

                    LilyPreparserFunBodyItem *item =
                      NEW_VARIANT(LilyPreparserFunBodyItem,
                                  stmt_if,
                                  NEW(LilyPreparserFunBodyItemStmtIf,
                                      if_expr,
                                      if_block,
                                      elif_exprs,
                                      elif_blocks,
                                      NULL),
                                  location);

                    push__Vec(body, item);

                    break;
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

                    FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
                    FREE(Vec, if_expr);

                    FREE_BUFFER_ITEMS(if_block->buffer,
                                      if_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, if_block);

                    FREE_BUFFER_ITEMS_2(
                      elif_exprs->buffer, elif_exprs->len, LilyToken);
                    FREE(Vec, elif_exprs);

                    FREE_BUFFER_ITEMS_2(elif_blocks->buffer,
                                        elif_blocks->len,
                                        LilyPreparserFunBodyItem);
                    FREE(Vec, elif_blocks);

                    return;

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

                FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
                FREE(Vec, if_expr);

                FREE_BUFFER_ITEMS(
                  if_block->buffer, if_block->len, LilyPreparserFunBodyItem);
                FREE(Vec, if_block);

                return;
            }

            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyPreparser(self); // skip `end` keyword

            LilyPreparserFunBodyItem *item =
              NEW_VARIANT(LilyPreparserFunBodyItem,
                          stmt_if,
                          NEW(LilyPreparserFunBodyItemStmtIf,
                              if_expr,
                              if_block,
                              NULL,
                              NULL,
                              else_block),
                          location);

            push__Vec(body, item);

            break;
        }

        case LILY_TOKEN_KIND_KEYWORD_END: {
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyPreparser(self); // skip `end` keyword

            LilyPreparserFunBodyItem *item =
              NEW_VARIANT(LilyPreparserFunBodyItem,
                          stmt_if,
                          NEW(LilyPreparserFunBodyItemStmtIf,
                              if_expr,
                              if_block,
                              NULL,
                              NULL,
                              NULL),
                          location);

            push__Vec(body, item);

            break;
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

            FREE_BUFFER_ITEMS(if_expr->buffer, if_expr->len, LilyToken);
            FREE(Vec, if_expr);

            FREE_BUFFER_ITEMS(
              if_block->buffer, if_block->len, LilyPreparserFunBodyItem);
            FREE(Vec, if_block);

            return;

        default:
            UNREACHABLE("this way is impossible");
    }
}

void
preparse_for_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `for` keyword

    // 1. Preparse for expression.
    Vec *expr = NEW(Vec);

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(expr, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

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
                          from__String("expected `do` keyword after `for` "
                                       "statement expression")),
              &self->count_error);

            // Clean up allocations

            FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
            FREE(Vec, expr);

            return;

        default:
            next_token__LilyPreparser(self);
    }

    // 2. Preparse `for` block
    Vec *block =
      preparse_body__LilyPreparser(self, &must_close_for_block__LilyPreparser);

    if (!block) {
        // Clean up allocations

        FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
        FREE(Vec, expr);

        return;
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self); // skip `end` keyword

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
                  stmt_for,
                  NEW(LilyPreparserFunBodyItemStmtFor, expr, block),
                  location);

    push__Vec(body, item);
}

void
preparse_while_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `while` keyword

    // 1. Preparse while expression
    Vec *expr = NEW(Vec);

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(expr, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

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

            // Clean up allocations

            FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
            FREE(Vec, expr);

            return;

        default:
            next_token__LilyPreparser(self);
    }

    // 2. Preparse `while` block
    Vec *block = preparse_body__LilyPreparser(
      self, &must_close_while_block__LilyPreparser);

    if (!block) {
        // Clean up allocations

        FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
        FREE(Vec, expr);

        return;
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self); // skip `end` keyword

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
                  stmt_while,
                  NEW(LilyPreparserFunBodyItemStmtWhile, expr, block),
                  location);

    push__Vec(body, item);
}

void
preparse_try_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;

    jump__LilyPreparser(self, 2); // skip `try` and `do` keyword

    // 1. Preparse try block
    Vec *try_block =
      preparse_body__LilyPreparser(self, &must_close_try_block__LilyPreparser);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END: {
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyPreparser(self); // skip `end` keyword

            LilyPreparserFunBodyItem *item = NEW_VARIANT(
              LilyPreparserFunBodyItem,
              stmt_try,
              NEW(LilyPreparserFunBodyItemStmtTry, try_block, NULL, NULL),
              location);

            push__Vec(body, item);

            break;
        }

        case LILY_TOKEN_KIND_KEYWORD_CATCH: {
            next_token__LilyPreparser(self);

            // 2. Preparse catch expression
            Vec *catch_expr = NEW(Vec);

            while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                push__Vec(catch_expr, clone__LilyToken(self->current));
                next_token__LilyPreparser(self);
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

                    FREE_BUFFER_ITEMS(
                      catch_expr->buffer, catch_expr->len, LilyToken);
                    FREE(Vec, catch_expr);

                    FREE_BUFFER_ITEMS(try_block->buffer,
                                      try_block->len,
                                      LilyPreparserFunBodyItem);
                    FREE(Vec, try_block);

                    return;
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

                FREE_BUFFER_ITEMS(
                  catch_expr->buffer, catch_expr->len, LilyToken);
                FREE(Vec, catch_expr);

                return;
            }

            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyPreparser(self); // skip `end` keyword

            LilyPreparserFunBodyItem *item =
              NEW_VARIANT(LilyPreparserFunBodyItem,
                          stmt_try,
                          NEW(LilyPreparserFunBodyItemStmtTry,
                              try_block,
                              catch_expr,
                              catch_block),
                          location);

            push__Vec(body, item);
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

            return;

        default:
            UNREACHABLE("this way is impossible");
    }
}

void
preparse_match_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location_match = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `match` keyword

    // 1. Preparse `match` expression
    Vec *expr = NEW(Vec); // Vec<LilyToken*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_DO &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(expr, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

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

            FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
            FREE(Vec, expr);

            return;

        default:
            UNREACHABLE("this way is impossible");
    }

    Vec *patterns = NEW(Vec);      // Vec<Vec<LilyToken*>*>*
    Vec *pattern_conds = NEW(Vec); // Vec<Vec<LilyToken*>*?>*
    Vec *blocks = NEW(Vec);        // Vec<LilyPreparserFunBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        // 2. Preparse pattern
        Vec *pattern = NEW(Vec);
        Vec *pattern_cond = NULL;

        while (self->current->kind != LILY_TOKEN_KIND_FAT_ARROW &&
               self->current->kind != LILY_TOKEN_KIND_INTERROGATION &&
               self->current->kind != LILY_TOKEN_KIND_EOF) {
            push__Vec(pattern, clone__LilyToken(self->current));
            next_token__LilyPreparser(self);
        }

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_INTERROGATION:
                next_token__LilyPreparser(self);

                pattern_cond = NEW(Vec);

                while (self->current->kind != LILY_TOKEN_KIND_FAT_ARROW &&
                       self->current->kind != LILY_TOKEN_KIND_EOF) {
                    push__Vec(pattern_cond, clone__LilyToken(self->current));
                    next_token__LilyPreparser(self);
                }

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

                    clean_up_pattern : {
                        FREE_BUFFER_ITEMS(expr->buffer, expr->len, LilyToken);
                        FREE(Vec, expr);

                        FREE_BUFFER_ITEMS_2(
                          patterns->buffer, patterns->len, LilyToken);
                        FREE(Vec, patterns);

                        for (Usize i = 0; i < pattern_conds->len; i++) {
                            Vec *item = get__Vec(pattern_conds, i);

                            if (item) {
                                FREE_BUFFER_ITEMS(
                                  item->buffer, item->len, LilyToken);
                                FREE(Vec, item);
                            }
                        }

                        FREE(Vec, pattern_conds);

                        FREE_BUFFER_ITEMS(blocks->buffer,
                                          blocks->len,
                                          LilyPreparserFunBodyItem);
                        FREE(Vec, blocks);

                        FREE_BUFFER_ITEMS(
                          pattern->buffer, pattern->len, LilyToken);
                        FREE(Vec, pattern);

                        FREE_BUFFER_ITEMS(
                          pattern_cond->buffer, pattern_cond->len, LilyToken);
                        FREE(Vec, pattern_cond);
                        return;
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
        switch (self->current->kind) {
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
                            preparse_basic_brace_block__LilyPreparser(self,
                                                                      blocks);

                        default:
                            goto preparse_expr;
                    }
                } else {
                    goto preparse_expr;
                }
            }

            /*
                begin
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
                preparse_basic_block__LilyPreparser(self, blocks);
                break;

            /*
                for <expr> in <expr> do
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_FOR:
                preparse_for_block__LilyPreparser(self, blocks);
                break;

            /*
                if <expr> do
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_IF:
                preparse_if_block__LilyPreparser(self, blocks);
                break;

            /*
                match <expr> do
                    <pattern> [? <expr>] => <stmt | expr;>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
                preparse_match_block__LilyPreparser(self, blocks);
                break;

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
                            preparse_try_block__LilyPreparser(self, blocks);
                            break;
                        default:
                            goto preparse_expr;
                    }
                } else {
                    goto preparse_expr;
                }

                break;
            }

            /*
                while <expr> do
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
                preparse_while_block__LilyPreparser(self, blocks);
                break;

            // Expression
            default: {
            preparse_expr : {
                Location location_expr =
                  clone__Location(&self->current->location);
                Vec *exprs = NEW(Vec);

                while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                       self->current->kind != LILY_TOKEN_KIND_EOF) {
                    push__Vec(exprs, clone__LilyToken(self->current));
                    next_token__LilyPreparser(self);
                }

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_SEMICOLON: {
                        LilyToken *previous =
                          get__Vec(self->tokens, self->position - 1);

                        end__Location(&location_expr,
                                      previous->location.end_line,
                                      previous->location.end_column);
                        next_token__LilyPreparser(self);
                        break;
                    }
                    case LILY_TOKEN_KIND_EOF:
                        // TODO: clean up allocations

                        return;
                    default:
                        UNREACHABLE("this way is impossible");
                }

                push__Vec(blocks,
                          NEW_VARIANT(LilyPreparserFunBodyItem,
                                      exprs,
                                      NEW(LilyPreparserFunBodyItemExprs, exprs),
                                      location_expr));
            }
            }
        }

        push__Vec(patterns, pattern);
        push__Vec(pattern_conds, pattern_cond);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            end__Location(&location_match,
                          self->current->location.end_line,
                          self->current->location.end_column);
            next_token__LilyPreparser(self);

            break;

        default:
            break;
    }

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
                  stmt_match,
                  NEW(LilyPreparserFunBodyItemStmtMatch,
                      expr,
                      patterns,
                      pattern_conds,
                      blocks),
                  location_match);

    push__Vec(body, item);
}

void
preparse_basic_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;

    next_token__LilyPreparser(self); // skip `begin` keyword

    Vec *basic_block_body = preparse_body__LilyPreparser(
      self, &must_close_basic_block__LilyPreparser);

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self); // skip `end` keyword

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
                  stmt_block,
                  NEW(LilyPreparserFunBodyItemStmtBlock, basic_block_body),
                  location);

    push__Vec(body, item);
}

void
preparse_basic_brace_block__LilyPreparser(LilyPreparser *self, Vec *body)
{
    Location location = location_fun_body_item;
    Vec *brace_block_body = preparse_body__LilyPreparser(
      self, &must_close_basic_brace_block__LilyPreparser);

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self); // skip `}` keyword

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
                  stmt_block,
                  NEW(LilyPreparserFunBodyItemStmtBlock, brace_block_body),
                  location);

    push__Vec(body, item);
}

void
preparse_variable_block__LilyPreparser(LilyPreparser *self,
                                       Vec *body,
                                       bool is_mut,
                                       bool is_trace,
                                       bool is_ref,
                                       bool is_drop)
{
    Location location = clone__Location(&self->current->location);
    String *name = NULL;
    Vec *data_type = NULL;

    // 1. Get the name of the variable.
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            next_token__LilyPreparser(self);

            break;
        default: {
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_EXPECTED_IDENTIFIER),
                          NULL,
                          NULL,
                          from__String("expected name of variable")),
              &self->count_error);

            name = from__String("__error__");
        }
    }

    // 2. Get the data type of the variable (optional).
    if (self->current->kind != LILY_TOKEN_KIND_COLON_EQ) {
        data_type = NEW(Vec);

        while (self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
               self->current->kind != LILY_TOKEN_KIND_EOF) {
            push__Vec(data_type, clone__LilyToken(self->current));
            next_token__LilyPreparser(self);
        }

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

            FREE_BUFFER_ITEMS(data_type->buffer, data_type->len, LilyToken);
            FREE(Vec, data_type);

            return;
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
                NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_TOKEN),
                NULL,
                NULL,
                from__String(
                  "expected `:=` before the expresion of the variable")),
              &self->count_error);
    }

    // 4. Preparse expression.
    Vec *expr = NEW(Vec);

    while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(expr, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
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

    LilyPreparserFunBodyItem *item =
      NEW_VARIANT(LilyPreparserFunBodyItem,
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

    push__Vec(body, item);
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
            case LILY_TOKEN_KIND_KEYWORD_FOR:
            case LILY_TOKEN_KIND_KEYWORD_IF:
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
            case LILY_TOKEN_KIND_KEYWORD_TRY:
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
            case LILY_TOKEN_KIND_KEYWORD_MUT:
            case LILY_TOKEN_KIND_KEYWORD_VAL:
            case LILY_TOKEN_KIND_EOF:
            case LILY_TOKEN_KIND_KEYWORD_END:
                return false;
            case LILY_TOKEN_KIND_KEYWORD_REF:
            case LILY_TOKEN_KIND_KEYWORD_TRACE:
                return peeked->kind != LILY_TOKEN_KIND_KEYWORD_VAL &&
                       peeked->kind != LILY_TOKEN_KIND_KEYWORD_MUT;
            default:
                return true;
        }
    } else {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_BEGIN:
            case LILY_TOKEN_KIND_KEYWORD_FOR:
            case LILY_TOKEN_KIND_KEYWORD_IF:
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
            case LILY_TOKEN_KIND_KEYWORD_TRY:
            case LILY_TOKEN_KIND_KEYWORD_WHILE:
            case LILY_TOKEN_KIND_KEYWORD_MUT:
            case LILY_TOKEN_KIND_KEYWORD_VAL:
            case LILY_TOKEN_KIND_EOF:
            case LILY_TOKEN_KIND_KEYWORD_END:
                return false;
            default:
                return true;
        }
    }
}

Vec *
preparse_body__LilyPreparser(LilyPreparser *self,
                             bool (*must_close)(LilyPreparser *))
{
    Vec *body = NEW(Vec);
    bool is_brace_block = false;

    while (!must_close(self)) {
        location_fun_body_item = clone__Location(&self->current->location);

        switch (self->current->kind) {
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
                            is_brace_block = true;
                            jump__LilyPreparser(self, 2);
                            preparse_basic_brace_block__LilyPreparser(self,
                                                                      body);

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
                preparse_basic_block__LilyPreparser(self, body);
                break;

            /*
                for <expr> in <expr> do
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_FOR:
                preparse_for_block__LilyPreparser(self, body);
                break;

            /*
                if <expr> do
                    <block>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_IF:
                preparse_if_block__LilyPreparser(self, body);
                break;

            /*
                match <expr> do
                    <pattern> [? <expr>] => <stmt | expr;>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_MATCH:
                preparse_match_block__LilyPreparser(self, body);
                break;

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
                            preparse_try_block__LilyPreparser(self, body);
                            break;
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
                preparse_while_block__LilyPreparser(self, body);
                break;

            /*
                ref val <name> [data_type] := <expr>;
                ref mut <name> [data_type] := <expr>;
            */
            case LILY_TOKEN_KIND_KEYWORD_REF: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_VAL:
                            jump__LilyPreparser(self, 2);

                            preparse_variable_block__LilyPreparser(
                              self, body, false, false, true, false);

                            break;

                        case LILY_TOKEN_KIND_KEYWORD_MUT:
                            jump__LilyPreparser(self, 2);

                            preparse_variable_block__LilyPreparser(
                              self, body, true, false, true, false);

                            break;

                        default:
                            goto preparse_exprs;
                    }
                } else {
                    goto preparse_exprs;
                }

                break;
            }

            /*
                mut <name> [data_type] := <expr>;
            */
            case LILY_TOKEN_KIND_KEYWORD_MUT:
                next_token__LilyPreparser(self);

                preparse_variable_block__LilyPreparser(
                  self, body, true, false, false, false);
                break;

            /*
                trace val <name> [data_type] := <expr>;
                trace mut <name> [data_type] := <expr>;
            */
            case LILY_TOKEN_KIND_KEYWORD_TRACE: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    switch (peeked->kind) {
                        case LILY_TOKEN_KIND_KEYWORD_VAL:
                            jump__LilyPreparser(self, 2);

                            preparse_variable_block__LilyPreparser(
                              self, body, false, true, false, false);

                            break;

                        case LILY_TOKEN_KIND_KEYWORD_MUT:
                            jump__LilyPreparser(self, 2);

                            preparse_variable_block__LilyPreparser(
                              self, body, true, true, false, false);

                            break;

                        default:
                            goto preparse_exprs;
                    }
                } else {
                    goto preparse_exprs;
                }

                break;
            }

            /*
                val <name> [data_type] := <expr>;
            */
            case LILY_TOKEN_KIND_KEYWORD_VAL:
                next_token__LilyPreparser(self);

                preparse_variable_block__LilyPreparser(
                  self, body, false, false, false, false);

                break;

            default:
            preparse_exprs : {
                Location location = clone__Location(&self->current->location);
                Vec *exprs = NEW(Vec);

                while (must_preparse_exprs(self)) {
                    push__Vec(exprs, clone__LilyToken(self->current));
                    next_token__LilyPreparser(self);
                }

                LilyToken *previous = self->tokens->buffer[self->position - 1];

                end__Location(&location,
                              previous->location.end_line,
                              previous->location.end_column);
                push__Vec(body,
                          NEW_VARIANT(LilyPreparserFunBodyItem,
                                      exprs,
                                      NEW(LilyPreparserFunBodyItemExprs, exprs),
                                      location));

                break;
            }
        }
    }

    if (is_brace_block || self->current->kind == LILY_TOKEN_KIND_KEYWORD_END) {
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
    next_token__LilyPreparser(self);

    Location location = location_decl;
    String *object_impl = NULL;
    String *name = NULL;
    Vec *generic_params = NULL;       // Vec<Vec<LilyToken*>*>*?
    Vec *params = NULL;               // Vec<Vec<LilyToken*>*>*?
    Vec *when = NEW(Vec);             // Vec<Vec<LilyToken*>*>*
    Vec *req = NEW(Vec);              // Vec<Vec<LilyToken*>*>*
    Vec *body = NULL;                 // Vec<LilyToken*>*
    Vec *return_data_type = NEW(Vec); // Vec<LilyToken*>*
    bool req_is_comptime = false;
    bool when_is_comptime = false;

    // 1. Get name's object implementation
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_AT:
            next_token__LilyPreparser(self);

        parse_object_impl_name : {
            switch (self->current->kind) {
                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                    if (object_impl) {
                        push__String(object_impl, '.');
                        push_str__String(
                          object_impl,
                          self->current->identifier_normal->buffer);
                        next_token__LilyPreparser(self);
                    } else {
                        object_impl = self->current->identifier_normal;
                        next_token__LilyPreparser(self);
                    }

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
        preparse_when_expr : {
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_HOOK: {
                    next_token__LilyPreparser(self);

                    Vec *expr = NEW(Vec);

                    while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                        push__Vec(expr, clone__LilyToken(self->current));
                        next_token__LilyPreparser(self);
                    }

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
        preparse_req_expr : {
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_L_HOOK: {

                    Vec *expr = NEW(Vec);

                    while (self->current->kind != LILY_TOKEN_KIND_R_HOOK) {
                        push__Vec(expr, clone__LilyToken(self->current));
                        next_token__LilyPreparser(self);
                    }

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
        }
        case LILY_TOKEN_KIND_KEYWORD_COMPTIME:
        preparse_comptime_expr : {
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
                        NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_TOKEN),
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
            while (self->current->kind != LILY_TOKEN_KIND_EQ &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                push__Vec(return_data_type, clone__LilyToken(self->current));
                next_token__LilyPreparser(self);
            }

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

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self); // skip `end` keyword

    // TODO: Check if the function is an operator.
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
                           false,
                           when_is_comptime,
                           req_is_comptime));
}

LilyPreparserClassBodyItem *
preparse_attribute_for_class__LilyPreparser(LilyPreparser *self,
                                            Location location)
{
    next_token__LilyPreparser(self); // skip `val` keyword

    // 1. Get attribute's name
    String *name = NULL;

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
                          NULL,
                          NULL,
                          from__String("expected name of the attribute")),
              &self->count_error);

            name = from__String("__error__");
    }

    // 2. Get attribute's data type
    Vec *data_type = NEW(Vec); // Vec<LilyToken*>*

    while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
           self->current->kind != LILY_TOKEN_KIND_COLON_COLON &&
           self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(data_type, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

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
    Vec *default_expr = NULL; // Vec<LilyToken*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_COLON_EQ:
            next_token__LilyPreparser(self);

            default_expr = NEW(Vec);

            while (self->current->kind != LILY_TOKEN_KIND_COLON_COLON &&
                   self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                push__Vec(default_expr, clone__LilyToken(self->current));
                next_token__LilyPreparser(self);
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
                        NULL,
                        NULL,
                        from__String("expected `::` or `;`")),
                      &self->count_error);

                    FREE(String, name);

                    FREE_BUFFER_ITEMS(
                      data_type->buffer, data_type->len, LilyToken);
                    FREE(Vec, data_type);

                    if (default_expr) {
                        FREE_BUFFER_ITEMS(
                          default_expr->buffer, default_expr->len, LilyToken);
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

                    FREE_BUFFER_ITEMS(
                      data_type->buffer, data_type->len, LilyToken);
                    FREE(Vec, data_type);

                    if (default_expr) {
                        FREE_BUFFER_ITEMS(
                          default_expr->buffer, default_expr->len, LilyToken);
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

return_attribute : {
    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            end__Location(&location,
                          self->current->location.end_line,
                          self->current->location.end_column);
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

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

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

LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *impls,
                              Vec *inherits,
                              Vec *generic_params)
{
    Location location = location_decl;
    enum LilyVisibility visibility = visibility_decl;

    // 1. Preparse class body
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
            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserClassBodyItem *method =
                  preparse_method_for_class__LilyPreparser(self);

                if (method) {
                    push__Vec(body, method);
                } else {
                clean_up : {
                    FREE(String, name);

                    if (impls) {
                        FREE_BUFFER_ITEMS_2(
                          impls->buffer, impls->len, LilyToken);
                        FREE(Vec, impls);
                    }

                    if (inherits) {
                        FREE_BUFFER_ITEMS_2(
                          inherits->buffer, inherits->len, LilyToken);
                        FREE(Vec, inherits);
                    }

                    FREE_BUFFER_ITEMS(
                      body->buffer, body->len, LilyPreparserClassBodyItem);
                    FREE(Vec, body);

                    return NULL;
                }
                }
            }
            default:
                break;
        }

        visibility_decl = LILY_VISIBILITY_PRIVATE;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
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

            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS_2(impls->buffer, impls->len, LilyToken);
                FREE(Vec, impls);
            }

            if (inherits) {
                FREE_BUFFER_ITEMS_2(inherits->buffer, inherits->len, LilyToken);
                FREE(Vec, inherits);
            }

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserClassBodyItem);
            FREE(Vec, body);

            visibility_decl = LILY_VISIBILITY_PRIVATE;

            return NULL;

        default:
            UNREACHABLE("this way is possible");
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

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
                                       visibility)));
}

LilyPreparserTraitBodyItem *
preparse_prototype__LilyPreparser(LilyPreparser *self)
{
    Location location = clone__Location(&self->current->location);

    next_token__LilyPreparser(self); // skip `fun` keyword

    // 1. Get name of prototype
    String *name = NULL;

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
                          NULL,
                          NULL,
                          from__String("expected name of protoype here")),
              &self->count_error);

            name = from__String("__error__");

            break;
    }

    // 2. Preparse generic params.
    Vec *generic_params = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_HOOK:
            generic_params = preparse_hook_with_comma_sep__LilyPreparser(self);
            break;

        default:
            break;
    }

    // 3. Preparse params.
    Vec *params = NULL;

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_L_PAREN:
            params = preparse_paren_with_comma_sep__LilyPreparser(self);
            break;

        default:
            break;
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

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

    return NEW_VARIANT(
      LilyPreparserTraitBodyItem,
      prototype,
      location,
      NEW(LilyPreparserPrototype, name, generic_params, params));
}

LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self,
                              String *name,
                              Vec *inherits,
                              Vec *generic_params)
{
    Location location = location_decl;
    enum LilyVisibility visibility = visibility_decl;

    // 1. Preparse body.
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

            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserTraitBodyItem *prototype =
                  preparse_prototype__LilyPreparser(self);

                if (prototype) {
                    push__Vec(body, prototype);
                } else {
                clean_up : {
                    // Clean up allocations

                    FREE(String, name);

                    if (inherits) {
                        FREE_BUFFER_ITEMS_2(
                          inherits->buffer, inherits->len, LilyToken);
                        FREE(Vec, inherits);
                    }

                    if (generic_params) {
                        FREE_BUFFER_ITEMS_2(generic_params->buffer,
                                            generic_params->len,
                                            LilyToken);
                        FREE(Vec, generic_params);
                    }

                    FREE_BUFFER_ITEMS(
                      body->buffer, body->len, LilyPreparserTraitBodyItem);
                    FREE(Vec, body);

                    visibility_decl = LILY_VISIBILITY_PRIVATE;

                    return NULL;
                }
                }

                break;
            }
            default:
                break;
        }
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);
    next_token__LilyPreparser(self);

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
                                       visibility)));
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

LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self,
                                      String *name,
                                      Vec *impls,
                                      Vec *generic_params)
{
    enum LilyVisibility visibility = visibility_decl;
    Location location = location_decl;
    Vec *body = NEW(Vec); // Vec<LilyPreparserRecordObjectBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                // TODO: push constant to body.
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

            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyPreparserRecordField *field =
                  preparse_record_field__LilyPreparser(self);

                if (field) {
                    push__Vec(body,
                              NEW_VARIANT(LilyPreparserRecordObjectBodyItem,
                                          field,
                                          field->location,
                                          *field));
                    lily_free(field);
                } else {
                clean_up : {
                    // Clean up allocations

                    FREE(String, name);

                    if (impls) {
                        FREE_BUFFER_ITEMS_2(
                          impls->buffer, impls->len, LilyToken);
                        FREE(Vec, impls);
                    }

                    if (generic_params) {
                        FREE_BUFFER_ITEMS_2(generic_params->buffer,
                                            generic_params->len,
                                            LilyToken);
                        FREE(Vec, generic_params);
                    }

                    FREE_BUFFER_ITEMS(body->buffer,
                                      body->len,
                                      LilyPreparserRecordObjectBodyItem);
                    FREE(Vec, body);

                    return NULL;
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
                    from__String("expected identifier, `val` or `fun`")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
        }
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

            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS_2(impls->buffer, impls->len, LilyToken);
                FREE(Vec, impls);
            }

            if (generic_params) {
                FREE_BUFFER_ITEMS_2(
                  generic_params->buffer, generic_params->len, LilyToken);
                FREE(Vec, generic_params);
            }

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserRecordObjectBodyItem);
            FREE(Vec, body);

            return NULL;

        case LILY_TOKEN_KIND_KEYWORD_END:
            next_token__LilyPreparser(self);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

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

LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self,
                                    String *name,
                                    Vec *impls,
                                    Vec *generic_params)
{
    enum LilyVisibility visibility = visibility_decl;
    Location location = location_decl;
    Vec *body = NEW(Vec); // Vec<LilyPreparserEnumObjectBodyItem*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_VAL: {
                // TODO: push constant to body.
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

            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyPreparserEnumVariant *variant =
                  preparse_enum_variant__LilyPreparser(self);

                if (variant) {
                    push__Vec(body,
                              NEW_VARIANT(LilyPreparserEnumObjectBodyItem,
                                          variant,
                                          variant->location,
                                          *variant));
                    lily_free(variant);
                } else {
                clean_up : {
                    // Clean up allocations

                    FREE(String, name);

                    if (impls) {
                        FREE_BUFFER_ITEMS_2(
                          impls->buffer, impls->len, LilyToken);
                        FREE(Vec, impls);
                    }

                    if (generic_params) {
                        FREE_BUFFER_ITEMS_2(generic_params->buffer,
                                            generic_params->len,
                                            LilyToken);
                        FREE(Vec, generic_params);
                    }

                    FREE_BUFFER_ITEMS(
                      body->buffer, body->len, LilyPreparserEnumObjectBodyItem);
                    FREE(Vec, body);

                    return NULL;
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
                    from__String("expected identifier, `val` or `fun`")),
                  &self->count_error);

                FREE(String, current_s);

                next_token__LilyPreparser(self);

                break;
            }
        }
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

            FREE(String, name);

            if (impls) {
                FREE_BUFFER_ITEMS_2(impls->buffer, impls->len, LilyToken);
                FREE(Vec, impls);
            }

            if (generic_params) {
                FREE_BUFFER_ITEMS_2(
                  generic_params->buffer, generic_params->len, LilyToken);
                FREE(Vec, generic_params);
            }

            FREE_BUFFER_ITEMS(
              body->buffer, body->len, LilyPreparserEnumObjectBodyItem);
            FREE(Vec, body);

            return NULL;

        case LILY_TOKEN_KIND_KEYWORD_END:
            next_token__LilyPreparser(self);

            break;

        default:
            UNREACHABLE("this way is impossible");
    }

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

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
preparse_object__LilyPreparser(LilyPreparser *self)
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
                        FREE_BUFFER_ITEMS_2(                                   \
                          impls->buffer, impls->len, LilyToken);               \
                        FREE(Vec, impls);                                      \
                                                                               \
                        if (inherits) {                                        \
                            for (Usize i = 0; i < inherits->len; i++) {        \
                                Vec *item = get__Vec(inherits, i);             \
                                                                               \
                                FREE_BUFFER_ITEMS(                             \
                                  item->buffer, item->len, LilyToken);         \
                                FREE(Vec, item);                               \
                            }                                                  \
                                                                               \
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
                        FREE_BUFFER_ITEMS_2(                                   \
                          inherits->buffer, inherits->len, LilyToken);         \
                        FREE(Vec, inherits);                                   \
                                                                               \
                        if (impls) {                                           \
                            FREE_BUFFER_ITEMS_2(                               \
                              impls->buffer, impls->len, LilyToken);           \
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
            Vec *item = NEW(Vec); /* Vec<LilyToken*>* */                       \
                                                                               \
            while (self->current->kind != LILY_TOKEN_KIND_PLUS &&              \
                   self->current->kind != LILY_TOKEN_KIND_KEYWORD_IN &&        \
                   self->current->kind != LILY_TOKEN_KIND_EOF) {               \
                push__Vec(item, clone__LilyToken(self->current));              \
                next_token__LilyPreparser(self);                               \
            }                                                                  \
                                                                               \
            switch (self->current->kind) {                                     \
                next_token__LilyPreparser(self);                               \
                                                                               \
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
    Vec *impls = NULL;    // Vec<Vec<LilyToken*>*>*?
    Vec *inherits = NULL; // Vec<Vec<LilyToken*>*>*?

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_IMPL: {
        preparse_impl : {
            location_impl = &self->current->location;
            PREPARSE_INHERIT_OR_IMPL(
              impls, preparse_inherit, LILY_TOKEN_KIND_KEYWORD_INHERIT);
        }
        }
        case LILY_TOKEN_KIND_KEYWORD_INHERIT: {
        preparse_inherit : {
            location_inherit = &self->current->location;
            PREPARSE_INHERIT_OR_IMPL(
              inherits, preparse_impl, LILY_TOKEN_KIND_KEYWORD_IMPL);
        }
        }
        default:
            break;
    }

    // 2. Get object's name
    String *name = NULL;

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
                          NULL,
                          NULL,
                          from__String("expected name of object")),
              &self->count_error);

            name = from__String("__error__");
    }

    // 3. Preparse generic params
    Vec *generic_params = NULL;

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
              self, name, impls, inherits, generic_params);

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

                FREE_BUFFER_ITEMS_2(inherits->buffer, inherits->len, LilyToken);
                FREE(Vec, inherits);
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

                FREE_BUFFER_ITEMS_2(inherits->buffer, inherits->len, LilyToken);
                FREE(Vec, inherits);
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

                FREE_BUFFER_ITEMS_2(impls->buffer, impls->len, LilyToken);
                FREE(Vec, impls);
            }

            return preparse_trait__LilyPreparser(
              self, name, inherits, generic_params);

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
                FREE_BUFFER_ITEMS_2(impls->buffer, impls->len, LilyToken);
                FREE(Vec, impls);
            }

            if (inherits) {
                FREE_BUFFER_ITEMS_2(inherits->buffer, inherits->len, LilyToken);
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
    String *name = NULL;

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
                          NULL,
                          NULL,
                          from__String("expected `type` identifier")),
              &self->count_error);

            name = from__String("__error__");
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
                          NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_TOKEN),
                          NULL,
                          NULL,
                          from__String("expected `=`")),
              &self->count_error);

            FREE(String, name);

            return NULL;
    }

    switch (kind_of_type) {
        case LILY_TOKEN_KIND_KEYWORD_ALIAS:
            return preparse_alias__LilyPreparser(self, name);
        case LILY_TOKEN_KIND_KEYWORD_ENUM:
            return preparse_enum__LilyPreparser(self, name);
        case LILY_TOKEN_KIND_KEYWORD_RECORD:
            return preparse_record__LilyPreparser(self, name);
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
preparse_record_field__LilyPreparser(LilyPreparser *self)
{
    String *name = NULL;
    Vec *data_type = NEW(Vec); // Vec<LilyToken*>*
    Vec *optional_expr = NULL;
    Location location_field = clone__Location(&self->current->location);
    enum LilyVisibility visibility_field = LILY_VISIBILITY_PRIVATE;

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

    while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
           self->current->kind != LILY_TOKEN_KIND_COLON_EQ &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(data_type, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            next_token__LilyPreparser(self);

            break;
        case LILY_TOKEN_KIND_COLON_EQ: {
            next_token__LilyPreparser(self);

            optional_expr = NEW(Vec);

            while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
                   self->current->kind != LILY_TOKEN_KIND_EOF) {
                push__Vec(optional_expr, clone__LilyToken(self->current));
                next_token__LilyPreparser(self);
            }

            switch (self->current->kind) {
                case LILY_TOKEN_KIND_SEMICOLON:
                    end__Location(&location_field,
                                  self->current->location.end_line,
                                  self->current->location.end_column);
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

                    FREE_BUFFER_ITEMS(
                      optional_expr->buffer, optional_expr->len, LilyToken);
                    FREE(Vec, optional_expr);

                    FREE_BUFFER_ITEMS(
                      data_type->buffer, data_type->len, LilyToken);
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

            FREE_BUFFER_ITEMS(data_type->buffer, data_type->len, LilyToken);
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
               location_field,
               visibility_field);
}

LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self, String *name)
{
    Vec *fields = NEW(Vec); // Vec<LilyPreparserRecordField*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        LilyPreparserRecordField *field =
          preparse_record_field__LilyPreparser(self);

        if (field) {
            push__Vec(fields, field);
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            end__Location(&location_decl,
                          self->current->location.end_line,
                          self->current->location.end_column);

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

            FREE(String, name);

            FREE_BUFFER_ITEMS(
              fields->buffer, fields->len, LilyPreparserRecordField);
            FREE(Vec, fields);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      type,
      location_decl,
      NEW_VARIANT(LilyPreparserType,
                  record,
                  NEW(LilyPreparserRecord, name, fields, visibility_decl)));
}

static LilyPreparserEnumVariant *
preparse_enum_variant__LilyPreparser(LilyPreparser *self)
{
    Location location_variant = clone__Location(&self->current->location);

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
    Vec *data_type = NEW(Vec); // Vec<LilyToken*>*

    while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(data_type, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            end__Location(&location_variant,
                          self->current->location.end_line,
                          self->current->location.end_column);

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

            FREE_BUFFER_ITEMS(data_type->buffer, data_type->len, LilyToken);
            FREE(Vec, data_type);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW(LilyPreparserEnumVariant, name, data_type, location_variant);
}

LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self, String *name)
{
    Vec *variants = NEW(Vec); // Vec<LilyPreparserEnumVariant*>*

    while (self->current->kind != LILY_TOKEN_KIND_KEYWORD_END &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        LilyPreparserEnumVariant *variant =
          preparse_enum_variant__LilyPreparser(self);

        if (variant) {
            push__Vec(variants, variant);
        }
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_END:
            end__Location(&location_decl,
                          self->current->location.end_column,
                          self->current->location.end_column);

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

            FREE(String, name);

            FREE_BUFFER_ITEMS(
              variants->buffer, variants->len, LilyPreparserEnumVariant);
            FREE(Vec, variants);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      type,
      location_decl,
      NEW_VARIANT(LilyPreparserType,
                  enum_,
                  NEW(LilyPreparserEnum, name, variants, visibility_decl)));
}

LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self, String *name)
{
    Vec *data_type = NEW(Vec); // Vec<LilyToken*>*

    while (self->current->kind != LILY_TOKEN_KIND_SEMICOLON &&
           self->current->kind != LILY_TOKEN_KIND_EOF) {
        push__Vec(data_type, clone__LilyToken(self->current));
        next_token__LilyPreparser(self);
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            end__Location(&location_decl,
                          self->current->location.end_line,
                          self->current->location.end_column);

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

            FREE_BUFFER_ITEMS(data_type->buffer, data_type->len, LilyToken);
            FREE(Vec, data_type);

            return NULL;
        default:
            UNREACHABLE("this way is impossible");
    }

    return NEW_VARIANT(
      LilyPreparserDecl,
      type,
      location_decl,
      NEW_VARIANT(LilyPreparserType,
                  alias,
                  NEW(LilyPreparserAlias, name, data_type, visibility_decl)));
}

void
preparse_preprocess__LilyPreparser(LilyPreparser *self)
{
}

void
preparse_when_condition__LilyPreparser(LilyPreparser *self)
{
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

    FREE(Vec, self->public_macros);

#ifdef RUN_UNTIL_PREPARSER
    FREE(Vec, self->private_macros);
#endif

    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyPreparserDecl);
    FREE(Vec, self->decls);

    FREE(LilyPreparserPackage, self->package);
}

CONSTRUCTOR(LilyPreparser, LilyPreparser, const File *file, const Vec *tokens)
{
    return (LilyPreparser){ .file = file,
                            .tokens = tokens,
                            .current = NULL,
                            .position = 0,
                            .count_error = 0 };
}

void
run__LilyPreparser(LilyPreparser *self, LilyPreparserInfo *info)
{
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

                    case LILY_TOKEN_KIND_KEYWORD_object: {
                        LilyPreparserDecl *object =
                          preparse_object__LilyPreparser(self);

                        if (object) {
                            push__Vec(info->decls, object);
                        }

                        break;
                    }

                    default:
                        // ERROR: unexpected keyword after `pub`
                        break;
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
                fun[@<object_name>] <name>(<params>) [when [<cond>] + ...,
               req
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
                object <name>[[<generic_params>]] class =
                    <body>
                end

                object <name>[[<generic_params>]] record =
                    <body>
                end

                object <name>[[<generic_params>]] enum =
                    <body>
                end
            */
            case LILY_TOKEN_KIND_KEYWORD_object: {
                LilyPreparserDecl *object =
                  preparse_object__LilyPreparser(self);

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

            /*
                <name>!(<args>)
            */
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    if (peeked->kind == LILY_TOKEN_KIND_BANG) {
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
            unexpected_token : {
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

exit_preparser : {
}

#ifdef DEBUG_PREPARSER
    printf("\n====Preparser(%s)====\n", self->file->name);

    for (Usize i = 0; i < self->tokens->len; i++) {
        CALL_DEBUG(LilyToken, get__Vec(self->tokens, i));
    }

    printf("\n====Preparser public imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->public_imports->len; i++) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(info->public_imports, i));
    }

    printf("\n====Preparser private imports(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->private_imports->len; i++) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(info->private_imports, i));
    }

    printf("\n====Preparser public macros(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->public_macros->len; i++) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(info->public_macros, i));
    }

    printf("\n====Preparser private macros(%s)====\n", self->file->name);

    for (Usize i = 0; i < info->private_macros->len; i++) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(info->private_macros, i));
    }

    printf("\n====Package(%s)====\n", self->file->name);

    CALL_DEBUG(LilyPreparserPackage, info->package);
#endif

    if (self->count_error > 0) {
        exit(1);
    }
}
