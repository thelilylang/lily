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

#include <core/lily/checked/scope_container.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

CONSTRUCTOR(LilyCheckedScopeContainerModule *,
            LilyCheckedScopeContainerModule,
            String *name,
            LilyCheckedAccessModule access)
{
    LilyCheckedScopeContainerModule *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerModule));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerModule,
               const LilyCheckedScopeContainerModule *self)
{
    return format(
      "LilyCheckedScopeContainerModule{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessModule(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerConstant *,
            LilyCheckedScopeContainerConstant,
            String *name,
            LilyCheckedAccessConstant access)
{
    LilyCheckedScopeContainerConstant *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerConstant));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerConstant,
               const LilyCheckedScopeContainerConstant *self)
{
    return format(
      "LilyCheckedScopeContainerConstant{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessConstant(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerEnum *,
            LilyCheckedScopeContainerEnum,
            String *name,
            LilyCheckedAccessEnum access)
{
    LilyCheckedScopeContainerEnum *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerEnum));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerEnum,
               const LilyCheckedScopeContainerEnum *self)
{
    return format("LilyCheckedScopeContainerEnum{{ name = {S}, access = {sa} }",
                  self->name,
                  to_string__Debug__LilyCheckedAccessEnum(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerRecord *,
            LilyCheckedScopeContainerRecord,
            String *name,
            LilyCheckedAccessRecord access)
{
    LilyCheckedScopeContainerRecord *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerRecord));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerRecord,
               const LilyCheckedScopeContainerRecord *self)
{
    return format(
      "LilyCheckedScopeContainerRecord{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessRecord(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerAlias *,
            LilyCheckedScopeContainerAlias,
            String *name,
            LilyCheckedAccessAlias access)
{
    LilyCheckedScopeContainerAlias *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerAlias));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerAlias,
               const LilyCheckedScopeContainerAlias *self)
{
    return format(
      "LilyCheckedScopeContainerAlias{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessAlias(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerError *,
            LilyCheckedScopeContainerError,
            String *name,
            LilyCheckedAccessError access)
{
    LilyCheckedScopeContainerError *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerError));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerError,
               const LilyCheckedScopeContainerError *self)
{
    return format(
      "LilyCheckedScopeContainerError{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessError(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerEnumObject *,
            LilyCheckedScopeContainerEnumObject,
            String *name,
            LilyCheckedAccessEnumObject access)
{
    LilyCheckedScopeContainerEnumObject *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerEnumObject));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerEnumObject,
               const LilyCheckedScopeContainerEnumObject *self)
{
    return format(
      "LilyCheckedScopeContainerEnumObject{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessEnumObject(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerRecordObject *,
            LilyCheckedScopeContainerRecordObject,
            String *name,
            LilyCheckedAccessRecordObject access)
{
    LilyCheckedScopeContainerRecordObject *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerRecordObject));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerRecordObject,
               const LilyCheckedScopeContainerRecordObject *self)
{
    return format(
      "LilyCheckedScopeContainerRecordObject{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessRecordObject(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerClass *,
            LilyCheckedScopeContainerClass,
            String *name,
            LilyCheckedAccessClass access)
{
    LilyCheckedScopeContainerClass *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerClass));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerClass,
               const LilyCheckedScopeContainerClass *self)
{
    return format(
      "LilyCheckedScopeContainerClass{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessClass(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerTrait *,
            LilyCheckedScopeContainerTrait,
            String *name,
            LilyCheckedAccessTrait access)
{
    LilyCheckedScopeContainerTrait *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerTrait));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerTrait,
               const LilyCheckedScopeContainerTrait *self)
{
    return format(
      "LilyCheckedScopeContainerTrait{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessTrait(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerFun *,
            LilyCheckedScopeContainerFun,
            String *name,
            Vec *accesses)
{
    LilyCheckedScopeContainerFun *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerFun));

    self->name = name;
    self->accesses = accesses;

    return self;
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerFun,
               const LilyCheckedScopeContainerFun *self)
{
    String *res = format__String(
      "LilyCheckedScopeContainerFun{{ name = {S}, accesses =", self->name);

    DEBUG_VEC_STR(self->accesses, res, LilyCheckedAccessFun);
    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedScopeContainerFun, LilyCheckedScopeContainerFun *self)
{
    FREE(Vec, self->accesses);
    FREE_BUFFER_ITEMS(
      self->accesses->buffer, self->accesses->len, LilyCheckedAccessFun);
    lily_free(self);
}

CONSTRUCTOR(LilyCheckedScopeContainerLabel *,
            LilyCheckedScopeContainerLabel,
            String *name,
            LilyCheckedAccessLabel access)
{
    LilyCheckedScopeContainerLabel *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerLabel));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerLabel,
               const LilyCheckedScopeContainerLabel *self)
{
    return format(
      "LilyCheckedScopeContainerLabel{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessLabel(&self->access));
}
#endif

CONSTRUCTOR(LilyCheckedScopeContainerVariable *,
            LilyCheckedScopeContainerVariable,
            String *name,
            LilyCheckedAccessScope access)
{
    LilyCheckedScopeContainerVariable *self =
      lily_malloc(sizeof(LilyCheckedScopeContainerVariable));

    self->name = name;
    self->access = access;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedScopeContainerVariable,
               const LilyCheckedScopeContainerVariable *self)
{
    return format(
      "LilyCheckedScopeContainerVariable{{ name = {S}, access = {sa} }",
      self->name,
      to_string__Debug__LilyCheckedAccessScope(&self->access));
}
#endif
