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

#include <core/lily/analysis/checked/decl.h>
#include <core/lily/analysis/checked/package.h>
#include <core/lily/analysis/checked/scope.h>

#include <stdio.h>
#include <stdlib.h>

#ifdef ENV_DEBUG
#include <base/format.h>
#endif

#define PUSH(to, value) \
    if (!to) {          \
        to = NEW(Vec);  \
    }                   \
                        \
    push__Vec(to, value);

void
push_constant__LilyCheckedPackage(LilyCheckedPackage *self,
                                  LilyCheckedDecl *constant)
{
    PUSH(self->constants, constant);
}

void
push_enum__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *enum_)
{
    PUSH(self->enums, enum_);
}

void
push_record__LilyCheckedPackage(LilyCheckedPackage *self,
                                LilyCheckedDecl *record)
{
    PUSH(self->records, record);
}

void
push_alias__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *alias)
{
    PUSH(self->aliases, alias);
}

void
push_error__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *error)
{
    PUSH(self->errors, error);
}

void
push_enum_object__LilyCheckedPackage(LilyCheckedPackage *self,
                                     LilyCheckedDecl *enum_object)
{
    PUSH(self->enums_object, enum_object);
}

void
push_record_object__LilyCheckedPackage(LilyCheckedPackage *self,
                                       LilyCheckedDecl *record_object)
{
    PUSH(self->records_object, record_object);
}

void
push_class__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *class)
{
    PUSH(self->classes, class);
}

void
push_trait__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *trait)
{
    PUSH(self->traits, trait);
}

void
push_fun__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *fun)
{
    PUSH(self->funs, fun);
}

void
push_module__LilyCheckedPackage(LilyCheckedPackage *self,
                                LilyCheckedDecl *module)
{
    PUSH(self->modules, module);
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPackage, const LilyCheckedPackage *self)
{
    String *res =
      format__String("LilyCheckedPackage{{ id = {zu}, constants =", self->id);

    if (self->constants) {
        DEBUG_VEC_STRING(self->constants, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", enums =");

    if (self->enums) {
        DEBUG_VEC_STRING(self->enums, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", records =");

    if (self->records) {
        DEBUG_VEC_STRING(self->records, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", aliases =");

    if (self->aliases) {
        DEBUG_VEC_STRING(self->aliases, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", errors =");

    if (self->errors) {
        DEBUG_VEC_STRING(self->errors, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", enums_object =");

    if (self->enums_object) {
        DEBUG_VEC_STRING(self->enums_object, res, LilyCheckedDecl);
    } else {
        push_str__string(res, " NULL");
    }

    push_str__String(res, ", records_object =");

    if (self->records_object) {
        DEBUG_VEC_STRING(self->records_object, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", classes =");

    if (self->classes) {
        DEBUG_VEC_STRING(self->classes, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", traits =");

    if (self->traits) {
        DEBUG_VEC_STRING(self->traits, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", funs =");

    if (self->funs) {
        DEBUG_VEC_STRING(self->funs, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, ", modules =");

    if (self->modules) {
        DEBUG_VEC_STRING(self->modules, res, LilyCheckedDecl);
    } else {
        push_str__String(res, " NULL");
    }

    push_str__String(res, " }");

    return res;
}
#endif

DESTRUCTOR(LilyCheckedPackage, const LilyCheckedPackage *self)
{
    if (self->constants) {
        FREE_BUFFER_ITEMS(
          self->constants->buffer, self->constants->len, LilyCheckedDecl);
        FREE(Vec, self->constants);
    }

    if (self->enums) {
        FREE_BUFFER_ITEMS(
          self->enums->buffer, self->enums->len, LilyCheckedDecl);
        FREE(Vec, self->enums);
    }

    if (self->records) {
        FREE_BUFFER_ITEMS(
          self->records->buffer, self->records->len, LilyCheckedDecl);
        FREE(Vec, self->records);
    }

    if (self->aliases) {
        FREE_BUFFER_ITEMS(
          self->aliases->buffer, self->aliases->len, LilyCheckedDecl);
        FREE(Vec, self->aliases);
    }

    if (self->errors) {
        FREE_BUFFER_ITEMS(
          self->errors->buffer, self->errors->len, LilyCheckedDecl);
        FREE(Vec, self->errors);
    }

    if (self->enums_object) {
        FREE_BUFFER_ITEMS(
          self->enums_object->buffer, self->enums_object->len, LilyCheckedDecl);
        FREE(Vec, self->enums_object);
    }

    if (self->records_object) {
        FREE_BUFFER_ITEMS(self->records_object->buffer,
                          self->records_object->len,
                          LilyCheckedDecl);
        FREE(Vec, self->records_object);
    }

    if (self->classes) {
        FREE_BUFFER_ITEMS(
          self->classes->buffer, self->classes->len, LilyCheckedDecl);
        FREE(Vec, self->classes);
    }

    if (self->traits) {
        FREE_BUFFER_ITEMS(
          self->traits->buffer, self->traits->len, LilyCheckedDecl);
        FREE(Vec, self->traits);
    }

    if (self->funs) {
        FREE_BUFFER_ITEMS(self->funs->buffer, self->funs->len, LilyCheckedDecl);
        FREE(Vec, self->funs);
    }

    if (self->modules) {
        FREE_BUFFER_ITEMS(
          self->modules->buffer, self->modules->len, LilyCheckedScope);
        FREE(Vec, self->modules);
    }
}
