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

#include <core/lily/mir/instruction.h>
#include <core/lily/mir/scope.h>

#include <stdio.h>

static VARIANT_DESTRUCTOR(LilyMirScopeInstance,
                          field,
                          LilyMirScopeInstance *self);

static VARIANT_DESTRUCTOR(LilyMirScopeInstance,
                          param,
                          LilyMirScopeInstance *self);

static VARIANT_DESTRUCTOR(LilyMirScopeInstance,
                          var,
                          LilyMirScopeInstance *self);

CONSTRUCTOR(LilyMirScopeParam *,
            LilyMirScopeParam,
            LilyCheckedDataType *data_type)
{
    LilyMirScopeParam *self = lily_malloc(sizeof(LilyMirScopeParam));

    self->data_type = data_type;

    return self;
}

CONSTRUCTOR(LilyMirScopeVar *,
            LilyMirScopeVar,
            String *name,
            LilyCheckedDataType *data_type)
{
    LilyMirScopeVar *self = lily_malloc(sizeof(LilyMirScopeVar));

    self->name = name;
    self->data_type = data_type;

    return self;
}

CONSTRUCTOR(LilyMirScopeField *,
            LilyMirScopeField,
            String *global_name,
            LilyCheckedDataType *data_type,
            struct LilyMirScopeInstance *field_instance)
{
    LilyMirScopeField *self = lily_malloc(sizeof(LilyMirScopeField));

    self->global_name = global_name;
    self->data_type = data_type;
    self->field_instance = field_instance;

    return self;
}

DESTRUCTOR(LilyMirScopeField, LilyMirScopeField *self)
{
    if (self->field_instance) {
        FREE(LilyMirScopeInstance, self->field_instance);
    }

    lily_free(self);
}

VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    field,
                    Vec *fields,
                    LilyMirScopeField *field)
{
    LilyMirScopeInstance *self = lily_malloc(sizeof(LilyMirScopeInstance));

    self->kind = LILY_MIR_SCOPE_INSTANCE_KIND_FIELD;
    self->fields = fields;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    param,
                    Vec *fields,
                    const LilyMirScopeParam *param)
{
    LilyMirScopeInstance *self = lily_malloc(sizeof(LilyMirScopeInstance));

    self->kind = LILY_MIR_SCOPE_INSTANCE_KIND_PARAM;
    self->fields = fields;
    self->param = param;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    var,
                    Vec *fields,
                    const LilyMirScopeVar *var)
{
    LilyMirScopeInstance *self = lily_malloc(sizeof(LilyMirScopeInstance));

    self->kind = LILY_MIR_SCOPE_INSTANCE_KIND_VAR;
    self->fields = fields;
    self->var = var;

    return self;
}

VARIANT_DESTRUCTOR(LilyMirScopeInstance, field, LilyMirScopeInstance *self)
{
    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyMirScopeField);
    FREE(Vec, self->fields);
    FREE(LilyMirScopeField, self->field);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirScopeInstance, param, LilyMirScopeInstance *self)
{
    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyMirScopeField);
    FREE(Vec, self->fields);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirScopeInstance, var, LilyMirScopeInstance *self)
{
    FREE_BUFFER_ITEMS(
      self->fields->buffer, self->fields->len, LilyMirScopeField);
    FREE(Vec, self->fields);
    lily_free(self);
}

DESTRUCTOR(LilyMirScopeInstance, LilyMirScopeInstance *self)
{
    switch (self->kind) {
        case LILY_MIR_SCOPE_INSTANCE_KIND_FIELD:
            return FREE_VARIANT(LilyMirScopeInstance, field, self);
        case LILY_MIR_SCOPE_INSTANCE_KIND_PARAM:
            return FREE_VARIANT(LilyMirScopeInstance, param, self);
        case LILY_MIR_SCOPE_INSTANCE_KIND_VAR:
            return FREE_VARIANT(LilyMirScopeInstance, var, self);
        default:
            UNREACHABLE("unknown variant");
    }
}

CONSTRUCTOR(LilyMirScope *,
            LilyMirScope,
            enum LilyMirScopeKind kind,
            LilyMirBlockLimit *limit,
            LilyMirScope *parent)
{
    LilyMirScope *self = lily_malloc(sizeof(LilyMirScope));

    self->kind = kind;
    self->loads = NEW(Vec);
    self->params = self->kind == LILY_MIR_SCOPE_KIND_ROOT ? NEW(Vec) : NULL;
    self->vars = NEW(Vec);
    self->instances = NEW(Vec);
    self->limit = limit;
    self->parent = parent;

    return self;
}

DESTRUCTOR(LilyMirScope, LilyMirScope *self)
{
    FREE_BUFFER_ITEMS(
      self->loads->buffer, self->loads->len, LilyMirInstructionFunLoad);
    FREE(Vec, self->loads);

    if (self->params) {
        FREE_BUFFER_ITEMS(
          self->params->buffer, self->params->len, LilyMirScopeParam);
        FREE(Vec, self->params);
    }

    FREE_BUFFER_ITEMS(self->vars->buffer, self->vars->len, LilyMirScopeVar);
    FREE(Vec, self->vars);
    FREE_BUFFER_ITEMS(
      self->instances->buffer, self->instances->len, LilyMirScopeInstance);
    FREE(Vec, self->instances);
    lily_free(self);
}
