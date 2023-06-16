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

#include <core/lily/mir/instruction.h>
#include <core/lily/mir/scope.h>

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

DESTRUCTOR(LilyMirScope, const LilyMirScope *self)
{
    FREE_BUFFER_ITEMS(
      self->loads->buffer, self->loads->len, LilyMirInstructionFunLoad);
    FREE(Vec, self->loads);
    FREE_BUFFER_ITEMS(
      self->params->buffer, self->params->len, LilyMirScopeParam);
    FREE(Vec, self->params);
    FREE_BUFFER_ITEMS(self->vars->buffer, self->vars->len, LilyMirScopeVar);
    FREE(Vec, self->vars);
}
