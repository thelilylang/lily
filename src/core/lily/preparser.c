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

// Construct LilyPreparserFun type.
static inline CONSTRUCTOR(LilyPreparserFun,
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
                          bool when_is_comptime);

// Free LilyPreparserFun type.
static DESTRUCTOR(LilyPreparserFun, const LilyPreparserFun *self);

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

// Remove an item at the current position and update the current.
static void
eat_token__LilyPreparser(LilyPreparser *self);

// Remove an item at the current position and update the current (without free
// the token).
static void
eat_token_w_free__LilyPreparser(LilyPreparser *self);

// Peek token at position + n.
static LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n);

// Combine next_token and eat_token function.
static void
eat_and_next_token__LilyPreparser(LilyPreparser *self);

static void
eat_w_free_and_next_token__LilyPreparser(LilyPreparser *self);

static bool
is_start_a_new_block__LilyPreparser(const LilyPreparser *self);

static void
go_to_next_block__LilyPreparser(LilyPreparser *self);

static LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self);

static LilyPreparserMacro *
preparse_macro__LilyPreparser(LilyPreparser *self);

// NOTE: return 1 for success, 0 for failed.
static int
preparse_package__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_module_body__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_module__LilyPreparser(LilyPreparser *self);

static void
preparse_test__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_fun__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_object__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_type__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self);

static LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self);

static void
preparse_preprocess__LilyPreparser(LilyPreparser *self);

static void
preparse_when_condition__LilyPreparser(LilyPreparser *self);

static enum LilyVisibility visibility_decl = LILY_VISIBILITY_PRIVATE;
static Location location;

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

    for (Usize i = 0; i < self->tokens->len; i++) {
        char *s = to_string__Debug__LilyToken(get__Vec(self->tokens, i));

        push_str__String(res, s);

        lily_free(s);

        if (i != self->tokens->len - 1) {
            push_str__String(res, ", ");
        }
    }

    push_str__String(res, " }, location = ");

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
        push_str__String(res, ", sub_packages = { ");
    } else {
        push_str__String(
          res, "LilyPreparserPackage{ name = NULL, sub_packages = { ");
    }

    for (Usize i = 0; i < self->sub_packages->len; i++) {
        char *s = to_string__Debug__LilyPreparserSubPackage(
          get__Vec(self->sub_packages, i));

        push_str__String(res, s);

        lily_free(s);

        if (i != self->sub_packages->len - 1) {
            push_str__String(res, ", ");
        }
    }

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

DESTRUCTOR(LilyPreparserModule, const LilyPreparserModule *self)
{
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
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyToken);
    FREE(Vec, self->body);
}

CONSTRUCTOR(LilyPreparserFun,
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

DESTRUCTOR(LilyPreparserFun, const LilyPreparserFun *self)
{
    for (Usize i = 0; i < self->generic_params->len; i++) {
        Vec *item = get__Vec(self->generic_params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->generic_params);

    for (Usize i = 0; i < self->params->len; i++) {
        Vec *item = get__Vec(self->params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->params);

    FREE_BUFFER_ITEMS(
      self->return_data_type->buffer, self->return_data_type->len, LilyToken);
    FREE(Vec, self->return_data_type);
    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyToken);
    FREE(Vec, self->body);
    FREE_BUFFER_ITEMS(self->req->buffer, self->req->len, LilyToken);
    FREE(Vec, self->req);
    FREE_BUFFER_ITEMS(self->when->buffer, self->when->len, LilyToken);
    FREE(Vec, self->when);
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

DESTRUCTOR(LilyPreparserClass, const LilyPreparserClass *self)
{
    for (Usize i = 0; i < self->generic_params->len; i++) {
        Vec *item = get__Vec(self->generic_params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->generic_params);

    for (Usize i = 0; i < self->inherits->len; i++) {
        Vec *item = get__Vec(self->inherits, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->inherits);

    for (Usize i = 0; i < self->implements->len; i++) {
        Vec *item = get__Vec(self->implements, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->implements);

    for (Usize i = 0; i < self->body->len; i++) {
        Vec *item = get__Vec(self->body, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->body);
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

DESTRUCTOR(LilyPreparserTrait, const LilyPreparserTrait *self)
{
    for (Usize i = 0; i < self->generic_params->len; i++) {
        Vec *item = get__Vec(self->generic_params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->generic_params);

    for (Usize i = 0; i < self->inherits->len; i++) {
        Vec *item = get__Vec(self->inherits, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->inherits);

    FREE_BUFFER_ITEMS(self->body->buffer, self->body->len, LilyToken);
    FREE(Vec, self->body);
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

DESTRUCTOR(LilyPreparserRecordObject, const LilyPreparserRecordObject *self)
{
    for (Usize i = 0; i < self->generic_params->len; i++) {
        Vec *item = get__Vec(self->generic_params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->generic_params);

    for (Usize i = 0; i < self->implements->len; i++) {
        Vec *item = get__Vec(self->implements, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->implements);

    for (Usize i = 0; i < self->body->len; i++) {
        Vec *item = get__Vec(self->body, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->body);
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

DESTRUCTOR(LilyPreparserEnumObject, const LilyPreparserEnumObject *self)
{
    for (Usize i = 0; i < self->generic_params->len; i++) {
        Vec *item = get__Vec(self->generic_params, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->generic_params);

    for (Usize i = 0; i < self->implements->len; i++) {
        Vec *item = get__Vec(self->implements, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->implements);

    for (Usize i = 0; i < self->body->len; i++) {
        Vec *item = get__Vec(self->body, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }
    FREE(Vec, self->body);
}

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
    for (Usize i = 0; i < self->variants->len; i++) {
        Vec *item = get__Vec(self->variants, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }

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
    for (Usize i = 0; i < self->fields->len; i++) {
        Vec *item = get__Vec(self->fields, i);

        FREE_BUFFER_ITEMS(item->buffer, item->len, LilyToken);
        FREE(Vec, item);
    }

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

DESTRUCTOR(LilyPreparserConstantInfo, LilyPreparserConstantInfo *self)
{
    FREE_BUFFER_ITEMS(self->expr->buffer, self->expr->len, LilyToken);
    FREE(Vec, self->expr);
    FREE_BUFFER_ITEMS(self->data_type->buffer, self->data_type->len, LilyToken);
    FREE(Vec, self->data_type);
    lily_free(self);
}

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
        default:
            UNREACHABLE("unknown variant");
    }
}

void
next_token__LilyPreparser(LilyPreparser *self)
{
    if (self->position + 1 < self->scanner->tokens->len) {
        ++self->position;
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
eat_token__LilyPreparser(LilyPreparser *self)
{
    if (self->current->kind != LILY_TOKEN_KIND_EOF) {
        FREE(LilyToken, remove__Vec(self->scanner->tokens, self->position));
    }
}

void
eat_token_w_free__LilyPreparser(LilyPreparser *self)
{
    if (self->current->kind != LILY_TOKEN_KIND_EOF) {
        remove__Vec(self->scanner->tokens, self->position);
    }
}

LilyToken *
peek_token__LilyPreparser(const LilyPreparser *self, Usize n)
{
    if (self->position + n < self->scanner->tokens->len) {
        return get__Vec(self->scanner->tokens, self->position + n);
    }

    return NULL;
}

void
eat_and_next_token__LilyPreparser(LilyPreparser *self)
{
    eat_token__LilyPreparser(self);

    if (self->position < self->scanner->tokens->len) {
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
}

void
eat_w_free_and_next_token__LilyPreparser(LilyPreparser *self)
{
    eat_token_w_free__LilyPreparser(self);

    if (self->position < self->scanner->tokens->len) {
        self->current = get__Vec(self->scanner->tokens, self->position);
    }
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
            } else {
                return false;
            }
        }
        default:
            return false;
    }
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

LilyPreparserImport *
preparse_import__LilyPreparser(LilyPreparser *self)
{
    String *import_value = NULL;
    String *as_value = NULL;
    Location location = clone__Location(&self->current->location);

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_LITERAL_STRING:
            import_value = clone__String(self->current->literal_string);
            eat_and_next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->scanner->source.file,
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
            eat_and_next_token__LilyPreparser(self);

            if (self->current->kind == LILY_TOKEN_KIND_IDENTIFIER_NORMAL) {
                as_value = clone__String(self->current->identifier_normal);
                eat_and_next_token__LilyPreparser(self);
            } else {
                String *current_s = to_string__LilyToken(self->current);

                emit__Diagnostic(
                  NEW_VARIANT(
                    Diagnostic,
                    simple_lily_error,
                    self->scanner->source.file,
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

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
                &self->current->location,
                NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                NULL,
                NULL,
                from__String("expected `;`")),
              &self->count_error);

            FREE(String, current_s);
        }
    }

    return NEW(LilyPreparserImport, import_value, as_value, location);
}

LilyPreparserMacro *
preparse_macro__LilyPreparser(LilyPreparser *self)
{
    String *name = NULL;
    Vec *tokens = NEW(Vec);
    Location location = clone__Location(&self->current->location);

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            name = clone__String(self->current->identifier_normal);
            eat_and_next_token__LilyPreparser(self);
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->scanner->source.file,
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
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
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
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
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
        push__Vec(tokens, self->current);
        eat_w_free_and_next_token__LilyPreparser(self);
    }

    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_SEMICOLON:
            eat_and_next_token__LilyPreparser(self);
            break;
        case LILY_TOKEN_KIND_EOF: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
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

    end__Location(&location,
                  self->current->location.end_line,
                  self->current->location.end_column);

    return NEW(LilyPreparserMacro, name, tokens, location);
}

int
preparse_package__LilyPreparser(LilyPreparser *self)
{
    eat_and_next_token__LilyPreparser(self);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            if (self->package->name) {
                emit__Diagnostic(
                  NEW_VARIANT(Diagnostic,
                              simple_lily_error,
                              self->scanner->source.file,
                              &self->current->location,
                              NEW(LilyError,
                                  LILY_ERROR_KIND_PACKAGE_NAME_ALREADY_DEFINED),
                              NULL,
                              NULL,
                              NULL),
                  &self->count_error);

                return 0;
            }

            self->package->name =
              clone__String(self->current->identifier_normal);
            eat_and_next_token__LilyPreparser(self);

            break;
        default:
            break;
    }

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_EQ:
            eat_and_next_token__LilyPreparser(self);
            break;
        default: {
            String *current_s = to_string__LilyToken(self->current);

            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
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
                eat_and_next_token__LilyPreparser(self);
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
                eat_and_next_token__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                        sub_pkg_name =
                          clone__String(self->current->identifier_normal);
                        eat_and_next_token__LilyPreparser(self);

                        while (self->current->kind == LILY_TOKEN_KIND_DOT) {
                            push_str__String(sub_pkg_name, ".");

                            eat_and_next_token__LilyPreparser(self);

                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
                                    push_str__String(
                                      sub_pkg_name,
                                      self->current->identifier_normal->buffer);
                                    eat_and_next_token__LilyPreparser(self);

                                    break;
                                default:
                                    goto expected_sub_pkg_name;
                            }
                        }

                        if (self->current->kind !=
                              LILY_TOKEN_KIND_KEYWORD_END &&
                            self->current->kind == LILY_TOKEN_KIND_COMMA) {
                            switch (self->current->kind) {
                                case LILY_TOKEN_KIND_COMMA:
                                    eat_and_next_token__LilyPreparser(self);
                                    break;
                                default: {
                                    String *current_s =
                                      to_string__LilyToken(self->current);

                                    emit__Diagnostic(
                                      NEW_VARIANT(
                                        Diagnostic,
                                        simple_lily_error,
                                        self->scanner->source.file,
                                        &self->current->location,
                                        NEW_VARIANT(LilyError,
                                                    unexpected_token,
                                                    current_s->buffer),
                                        NULL,
                                        NULL,
                                        from__String("expected `,`")),
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
                            self->scanner->source.file,
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
                    self->scanner->source.file,
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

        push__Vec(self->package->sub_packages,
                  NEW(LilyPreparserSubPackage, visibility, sub_pkg_name));
    }

    eat_and_next_token__LilyPreparser(self);

    return 1;
}

LilyPreparserDecl *
preparse_module_body__LilyPreparser(LilyPreparser *self)
{
    location = clone__Location(&self->current->location);

    switch (self->current->kind) {
        case LILY_TOKEN_KIND_KEYWORD_PUB:
            next_token__LilyPreparser(self);

            switch (self->current->kind) {
                default:
                    break;
            }
            break;
        case LILY_TOKEN_KIND_KEYWORD_TYPE:
            return preparse_type__LilyPreparser(self);
            break;
        case LILY_TOKEN_KIND_KEYWORD_MODULE:
            break;
        case LILY_TOKEN_KIND_KEYWORD_FUN:
            break;
        case LILY_TOKEN_KIND_KEYWORD_object:
            break;
        case LILY_TOKEN_KIND_IDENTIFIER_NORMAL:
            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(Diagnostic,
                          simple_lily_error,
                          self->scanner->source.file,
                          &self->current->location,
                          NEW(LilyError, LILY_ERROR_KIND_UNEXPECTED_TOKEN),
                          NULL,
                          NULL,
                          NULL),
              &self->count_error);

            next_token__LilyPreparser(self);

            return NULL;
    }
}

LilyPreparserDecl *
preparse_module__LilyPreparser(LilyPreparser *self)
{
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
                name = self->current->identifier_normal;
                eat_w_free_and_next_token__LilyPreparser(self);
            }

            if (self->current->kind == LILY_TOKEN_KIND_DOT) {
                next_token__LilyPreparser(self);
                goto parse_module_name;
            }

            break;
        default:
            emit__Diagnostic(
              NEW_VARIANT(
                Diagnostic,
                simple_lily_error,
                self->scanner->source.file,
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

    Location module_location = location;

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

    if (self->current->kind == LILY_TOKEN_KIND_EOF) {
        FREE_BUFFER_ITEMS(body->buffer, body->len, LilyPreparserDecl);
        FREE(Vec, body);
        FREE(String, name);

        end__Location(&module_location,
                      self->current->location.end_line,
                      self->current->location.end_column);

        emit__Diagnostic(
          NEW_VARIANT(
            Diagnostic,
            simple_lily_error,
            self->scanner->source.file,
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

    end__Location(&module_location,
                  self->current->location.end_line,
                  self->current->location.end_column);

    return NEW_VARIANT(LilyPreparserDecl,
                       module,
                       module_location,
                       NEW(LilyPreparserModule, name, body, visibility_decl));
}

void
preparse_test__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_fun__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_class__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_trait__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_record_object__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_enum_object__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_object__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_type__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_record__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_enum__LilyPreparser(LilyPreparser *self)
{
}

LilyPreparserDecl *
preparse_alias__LilyPreparser(LilyPreparser *self)
{
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

CONSTRUCTOR(LilyPreparser,
            LilyPreparser,
            LilyScanner *scanner,
            String *package_name)
{
    return (LilyPreparser){ .scanner = scanner,
                            .public_imports = NEW(Vec),
                            .private_imports = NEW(Vec),
                            .public_macros = NEW(Vec),
                            .private_macros = NEW(Vec),
                            .decls = NEW(Vec),
                            .package = NEW(LilyPreparserPackage, package_name),
                            .current = NULL,
                            .position = 0,
                            .count_error = 0 };
}

void
run__LilyPreparser(LilyPreparser *self)
{
    self->current = get__Vec(self->scanner->tokens, 0);

    bool package_is_preparse = false;

    while (self->current->kind != LILY_TOKEN_KIND_EOF) {
        location = clone__Location(&self->current->location);

        switch (self->current->kind) {
            case LILY_TOKEN_KIND_KEYWORD_IMPORT: {
                LilyPreparserImport *import =
                  preparse_import__LilyPreparser(self);

                if (import) {
                    push__Vec(self->private_imports, import);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_MACRO: {
                LilyPreparserMacro *macro = preparse_macro__LilyPreparser(self);

                if (macro) {
                    push__Vec(self->private_macros, macro);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_PACKAGE: {
                if (package_is_preparse) {
                    emit__Diagnostic(
                      NEW_VARIANT(
                        Diagnostic,
                        simple_lily_error,
                        self->scanner->source.file,
                        &self->current->location,
                        NEW(LilyError,
                            LILY_ERROR_KIND_DUPLICATE_PACKAGE_DECLARATION),
                        NULL,
                        NULL,
                        NULL),
                      &self->count_error);

                    goto exit_preparser;
                }

                if (!preparse_package__LilyPreparser(self)) {
                    goto exit_preparser;
                } else {
                    package_is_preparse = true;
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_PUB:
                visibility_decl = LILY_VISIBILITY_PUBLIC;

                eat_and_next_token__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_IMPORT: {
                        LilyPreparserImport *import =
                          preparse_import__LilyPreparser(self);

                        if (import) {
                            push__Vec(self->public_imports, import);
                        }

                        break;
                    }
                    case LILY_TOKEN_KIND_KEYWORD_FUN: {
                        LilyPreparserDecl *fun =
                          preparse_fun__LilyPreparser(self);

                        if (fun) {
                            push__Vec(self->decls, fun);
                        }

                        break;
                    }
                    case LILY_TOKEN_KIND_KEYWORD_MODULE: {
                        LilyPreparserDecl *module =
                          preparse_module__LilyPreparser(self);

                        if (module) {
                            push__Vec(self->decls, module);
                        }

                        break;
                    }
                    case LILY_TOKEN_KIND_KEYWORD_TYPE: {
                        LilyPreparserDecl *type =
                          preparse_type__LilyPreparser(self);

                        if (type) {
                            push__Vec(self->decls, type);
                        }

                        break;
                    }
                    case LILY_TOKEN_KIND_KEYWORD_MACRO: {
                        LilyPreparserMacro *macro =
                          preparse_macro__LilyPreparser(self);

                        if (macro) {
                            push__Vec(self->public_macros, macro);
                        }

                        break;
                    }
                    case LILY_TOKEN_KIND_KEYWORD_object: {
                        LilyPreparserDecl *object =
                          preparse_object__LilyPreparser(self);

                        if (object) {
                            push__Vec(self->decls, object);
                        }

                        break;
                    }
                    default:
                        // ERROR: unexpected keyword after `pub`
                        break;
                }

                visibility_decl = LILY_VISIBILITY_PRIVATE;

                break;
            case LILY_TOKEN_KIND_KEYWORD_MODULE: {
                LilyPreparserDecl *module =
                  preparse_module__LilyPreparser(self);

                if (module) {
                    push__Vec(self->decls, module);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_TEST:
                preparse_test__LilyPreparser(self);

                break;
            case LILY_TOKEN_KIND_KEYWORD_FUN: {
                LilyPreparserDecl *fun = preparse_fun__LilyPreparser(self);

                if (fun) {
                    push__Vec(self->decls, fun);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_object: {
                LilyPreparserDecl *object =
                  preparse_object__LilyPreparser(self);

                if (object) {
                    push__Vec(self->decls, object);
                }

                break;
            }
            case LILY_TOKEN_KIND_KEYWORD_TYPE: {
                LilyPreparserDecl *type = preparse_type__LilyPreparser(self);

                if (type) {
                    push__Vec(self->decls, type);
                }

                break;
            }
            case LILY_TOKEN_KIND_IDENTIFIER_NORMAL: {
                LilyToken *peeked = peek_token__LilyPreparser(self, 1);

                if (peeked) {
                    if (peeked->kind != LILY_TOKEN_KIND_BANG) {
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
            case LILY_TOKEN_KIND_HASHTAG:
                preparse_preprocess__LilyPreparser(self);

                break;
            case LILY_TOKEN_KIND_KEYWORD_WHEN:
                preparse_when_condition__LilyPreparser(self);

                switch (self->current->kind) {
                    case LILY_TOKEN_KIND_KEYWORD_FUN: {
                        LilyPreparserDecl *fun =
                          preparse_fun__LilyPreparser(self);

                        if (fun) {
                            push__Vec(self->decls, fun);
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
                                    push__Vec(self->decls, fun);
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
                            self->scanner->source.file,
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
                    self->scanner->source.file,
                    &self->current->location,
                    NEW_VARIANT(LilyError, unexpected_token, current_s->buffer),
                    NULL,
                    NULL,
                    NULL),
                  &self->count_error);

                FREE(String, current_s);

                eat_and_next_token__LilyPreparser(self);
            }

            break;
            }
        }
    }

exit_preparser : {
}

#ifdef DEBUG_PREPARSER
    printf("\n====Preparser(%s)====\n", self->scanner->source.file->name);

    for (Usize i = 0; i < self->scanner->tokens->len; i++) {
        CALL_DEBUG(LilyToken, get__Vec(self->scanner->tokens, i));
    }

    printf("\n====Preparser public imports(%s)====\n",
           self->scanner->source.file->name);

    for (Usize i = 0; i < self->public_imports->len; i++) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(self->public_imports, i));
    }

    printf("\n====Preparser private imports(%s)====\n",
           self->scanner->source.file->name);

    for (Usize i = 0; i < self->private_imports->len; i++) {
        CALL_DEBUG(LilyPreparserImport, get__Vec(self->private_imports, i));
    }

    printf("\n====Preparser public macros(%s)====\n",
           self->scanner->source.file->name);

    for (Usize i = 0; i < self->public_macros->len; i++) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(self->public_macros, i));
    }

    printf("\n====Preparser private macros(%s)====\n",
           self->scanner->source.file->name);

    for (Usize i = 0; i < self->private_macros->len; i++) {
        CALL_DEBUG(LilyPreparserMacro, get__Vec(self->private_macros, i));
    }

    printf("\n====Package(%s)====\n", self->scanner->source.file->name);

    CALL_DEBUG(LilyPreparserPackage, self->package);
#endif

    if (self->count_error > 0) {
        exit(1);
    }
}

DESTRUCTOR(LilyPreparser, const LilyPreparser *self)
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

    FREE_BUFFER_ITEMS(self->decls->buffer, self->decls->len, LilyPreparserDecl);
    FREE(Vec, self->decls);

    FREE(LilyPreparserPackage, self->package);
}
