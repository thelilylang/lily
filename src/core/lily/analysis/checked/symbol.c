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

#include <core/lily/analysis/checked/symbol.h>

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    alias,
                    LilyCheckedSymbolAlias alias)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_ALIAS;
    self->alias = alias;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    class,
                    LilyCheckedSymbolClass class)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_CLASS;
    self->class = class;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    constant,
                    LilyCheckedSymbolConstant constant)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_CLASS;
    self->constant = constant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    enum,
                    LilyCheckedSymbolEnum enum_)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_ENUM;
    self->enum_ = enum_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    enum_object,
                    LilyCheckedSymbolEnumObject enum_object)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_ENUM_OBJECT;
    self->enum_object = enum_object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    error,
                    LilyCheckedSymbolError error)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_ERROR;
    self->error = error;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    field,
                    LilyCheckedSymbolField field)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_FIELD;
    self->field = field;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    field_object,
                    LilyCheckedSymbolFieldObject field_object)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_FIELD_OBJECT;
    self->field_object = field_object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    fun,
                    LilyCheckedSymbolFun fun)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_FUN;
    self->fun = fun;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    label,
                    LilyCheckedSymbolLabel label)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_LABEL;
    self->label = label;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    method,
                    LilyCheckedSymbolMethod method)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_METHOD;
    self->method = method;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    module,
                    LilyCheckedSymbolModule module)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_MODULE;
    self->module = module;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    package,
                    LilyCheckedSymbolPackage package)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_PACKAGE;
    self->package = package;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    param,
                    LilyCheckedSymbolParam param)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_PARAM;
    self->param = param;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    property,
                    LilyCheckedSymbolProperty property)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_PROPERTY;
    self->property = property;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    record,
                    LilyCheckedSymbolRecord record)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_RECORD;
    self->record = record;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    record_object,
                    LilyCheckedSymbolRecordObject record_object)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_RECORD_OBJECT;
    self->record_object = record_object;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    scope,
                    LilyCheckedSymbolScope scope)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_SCOPE;
    self->scope = scope;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    trait,
                    LilyCheckedSymbolTrait trait)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_TRAIT;
    self->trait = trait;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variable,
                    LilyCheckedSymbolVariable variable)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_VARIABLE;
    self->variable = variable;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variant,
                    LilyCheckedSymbolVariant variant)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_VARIANT;
    self->variant = variant;

    return self;
}

VARIANT_CONSTRUCTOR(LilyCheckedSymbol *,
                    LilyCheckedSymbol,
                    variant_object,
                    LilyCheckedSymbolVariantObject variant_object)
{
    LilyCheckedSymbol *self = lily_malloc(sizeof(LilyCheckedSymbol));

    self->kind = LILY_CHECKED_SYMBOL_KIND_VARIANT_OBJECT;
    self->variant_object = variant_object;

    return self;
}
