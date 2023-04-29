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

DESTRUCTOR(LilyCheckedScopeContainerFun, LilyCheckedScopeContainerFun *self)
{
    FREE(Vec, self->accesses);
    FREE_BUFFER_ITEMS(
      self->accesses->buffer, self->accesses->len, LilyCheckedAccessFun);
    lily_free(self);
}

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
