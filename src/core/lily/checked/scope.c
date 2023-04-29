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

#include <base/format.h>
#include <base/new.h>

#include <core/lily/checked/scope.h>

#ifdef ENV_DEBUG
#include <stdio.h>
#include <stdlib.h>
#endif

CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            LilyCheckedAccessScope *parent)
{
    LilyCheckedScope *self = lily_malloc(sizeof(LilyCheckedScope));

    self->modules = NEW(Vec);
    self->constants = NEW(Vec);
    self->enums = NEW(Vec);
    self->records = NEW(Vec);
    self->aliases = NEW(Vec);
    self->enums_object = NEW(Vec);
    self->records_object = NEW(Vec);
    self->classes = NEW(Vec);
    self->traits = NEW(Vec);
    self->funs = NEW(Vec);
    self->variables = NEW(Vec);
    self->parent = parent;
    self->children = NEW(Vec);

    return self;
}

DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self)
{
    FREE_BUFFER_ITEMS(self->modules->buffer,
                      self->modules->len,
                      LilyCheckedScopeContainerModule);
    FREE(Vec, self->modules);

    FREE_BUFFER_ITEMS(self->constants->buffer,
                      self->constants->len,
                      LilyCheckedScopeContainerConstant);
    FREE(Vec, self->constants);

    FREE_BUFFER_ITEMS(
      self->enums->buffer, self->enums->len, LilyCheckedScopeContainerEnum);
    FREE(Vec, self->enums);

    FREE_BUFFER_ITEMS(self->records->buffer,
                      self->records->len,
                      LilyCheckedScopeContainerRecord);
    FREE(Vec, self->records);

    FREE_BUFFER_ITEMS(self->aliases->buffer,
                      self->aliases->len,
                      LilyCheckedScopeContainerAlias);
    FREE(Vec, self->aliases);

    FREE_BUFFER_ITEMS(self->enums_object->buffer,
                      self->enums_object->len,
                      LilyCheckedScopeContainerEnumObject);
    FREE(Vec, self->enums_object);

    FREE_BUFFER_ITEMS(self->records_object->buffer,
                      self->records_object->len,
                      LilyCheckedScopeContainerRecordObject);
    FREE(Vec, self->records_object);

    FREE_BUFFER_ITEMS(self->classes->buffer,
                      self->classes->len,
                      LilyCheckedScopeContainerClass);
    FREE(Vec, self->classes);

    FREE_BUFFER_ITEMS(
      self->traits->buffer, self->traits->len, LilyCheckedScopeContainerTrait);
    FREE(Vec, self->traits);

    FREE_BUFFER_ITEMS(
      self->funs->buffer, self->funs->len, LilyCheckedScopeContainerFun);
    FREE(Vec, self->funs);

    FREE_BUFFER_ITEMS(self->variables->buffer,
                      self->variables->len,
                      LilyCheckedScopeContainerVariable);
    FREE(Vec, self->variables);

    if (self->parent) {
        FREE(LilyCheckedAccessScope, self->parent);
    }

    FREE_BUFFER_ITEMS(
      self->children->buffer, self->children->len, LilyCheckedAccessScope);
    FREE(Vec, self->children);
}
