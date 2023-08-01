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

#include <base/alloc.h>
#include <base/new.h>

#include <core/lily/mir/debug_info.h>

static VARIANT_DESTRUCTOR(LilyMirDebugInfo, file, LilyMirDebugInfo *self);

DESTRUCTOR(LilyMirDebugInfoFile, const LilyMirDebugInfoFile *self)
{
    FREE(String, self->filename);
    FREE(String, self->directory);
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    file,
                    Usize id,
                    LilyMirDebugInfoFile file)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_FILE;
    self->id = id;
    self->file = file;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    block,
                    Usize id,
                    LilyMirDebugInfoBlock block)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_BLOCK;
    self->id = id;
    self->block = block;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    location,
                    Usize id,
                    LilyMirDebugInfoLocation location)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_LOCATION;
    self->id = id;
    self->location = location;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    sub_program,
                    Usize id,
                    LilyMirDebugInfoSubProgram sub_program)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_SUB_PROGRAM;
    self->id = id;
    self->sub_program = sub_program;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    enumerator,
                    Usize id,
                    LilyMirDebugInfoEnumerator enumerator)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_ENUMERATOR;
    self->id = id;
    self->enumerator = enumerator;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    global_variable,
                    Usize id,
                    LilyMirDebugInfoGlobalVariable global_variable)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_GLOBAL_VARIABLE;
    self->id = id;
    self->global_variable = global_variable;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    local_variable,
                    Usize id,
                    LilyMirDebugInfoLocalVariable local_variable)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_LOCAL_VARIABLE;
    self->id = id;
    self->local_variable = local_variable;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    expression,
                    Usize id,
                    LilyMirDebugInfo *expression)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_EXPRESSION;
    self->id = id;
    self->expression = expression;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    type,
                    Usize id,
                    LilyMirDebugInfoType type)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_TYPE;
    self->id = id;
    self->type = type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    derived_type,
                    Usize id,
                    LilyMirDebugInfoDerivedType derived_type)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_DERIVED_TYPE;
    self->id = id;
    self->derived_type = derived_type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    comp_type,
                    Usize id,
                    LilyMirDebugInfoCompositeType comp_type)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_COMP_TYPE;
    self->id = id;
    self->comp_type = comp_type;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    elements,
                    Usize id,
                    LilyMirDebugInfoElements elements)
{
    LilyMirDebugInfo *self = lily_malloc(sizeof(LilyMirDebugInfo));

    self->kind = LILY_MIR_DEBUG_INFO_KIND_ELEMENTS;
    self->id = id;
    self->elements = elements;

    return self;
}

VARIANT_DESTRUCTOR(LilyMirDebugInfo, file, LilyMirDebugInfo *self)
{
    FREE(LilyMirDebugInfoFile, &self->file);
    lily_free(self);
}

DESTRUCTOR(LilyMirDebugInfo, LilyMirDebugInfo *self)
{
    switch (self->kind) {
        case LILY_MIR_DEBUG_INFO_KIND_FILE:
            FREE_VARIANT(LilyMirDebugInfo, file, self);
            break;
        default:
            lily_free(self);
    }
}
