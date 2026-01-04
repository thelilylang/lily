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

#include <core/lily/analysis/checked/virtual_scope.h>

#include <stdio.h>
#include <string.h>

CONSTRUCTOR(LilyCheckedVirtualVariable *,
            LilyCheckedVirtualVariable,
            const LilyCheckedStmtVariable *variable,
            LilyCheckedDataType *virtual_data_type)
{
    LilyCheckedVirtualVariable *self =
      lily_malloc(sizeof(LilyCheckedVirtualVariable));

    self->variable = variable;
    self->virtual_data_type = virtual_data_type;

    return self;
}

CONSTRUCTOR(LilyCheckedVirtualFunParam *,
            LilyCheckedVirtualFunParam,
            const LilyCheckedDeclFunParam *param,
            LilyCheckedDataType *virtual_data_type)
{
    LilyCheckedVirtualFunParam *self =
      lily_malloc(sizeof(LilyCheckedVirtualFunParam));

    self->param = param;
    self->virtual_data_type = virtual_data_type;

    return self;
}

CONSTRUCTOR(LilyCheckedVirtualScope *,
            LilyCheckedVirtualScope,
            LilyCheckedVirtualScope *parent)
{
    LilyCheckedVirtualScope *self =
      lily_malloc(sizeof(LilyCheckedVirtualScope));

    self->parent = parent;
    self->fun_params = NEW(Vec);
    self->variables = NEW(Vec);

    return self;
}

LilyCheckedVirtualFunParam *
search_fun_param__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                          String *name)
{
    for (Usize i = 0; i < self->fun_params->len; ++i) {
        LilyCheckedVirtualFunParam *param = get__Vec(self->fun_params, i);

        if (!strcmp(param->param->name->buffer, name->buffer)) {
            return param;
        }
    }

    if (self->parent) {
        return search_fun_param__LilyCheckedVirtualScope(self->parent, name);
    }

    UNREACHABLE("the analysis has a bug!!");
}

LilyCheckedVirtualVariable *
search_variable__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                         String *name)
{
    for (Usize i = 0; i < self->variables->len; ++i) {
        LilyCheckedVirtualVariable *variable = get__Vec(self->variables, i);

        if (!strcmp(variable->variable->name->buffer, name->buffer)) {
            return variable;
        }
    }

    if (self->parent) {
        return search_variable__LilyCheckedVirtualScope(self->parent, name);
    }

    UNREACHABLE("the analysis has a bug!!");
}

DESTRUCTOR(LilyCheckedVirtualScope, LilyCheckedVirtualScope *self)
{
    FREE_BUFFER_ITEMS(self->fun_params->buffer,
                      self->fun_params->len,
                      LilyCheckedVirtualFunParam);
    FREE(Vec, self->fun_params);
    FREE_BUFFER_ITEMS(self->variables->buffer,
                      self->variables->len,
                      LilyCheckedVirtualVariable);
    FREE(Vec, self->variables);
    lily_free(self);
}
