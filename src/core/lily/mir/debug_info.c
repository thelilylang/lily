/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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
#include <base/macros.h>
#include <base/new.h>

#include <core/lily/mir/debug_info.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline bool
eq__LilyMirDebugInfoFile(const LilyMirDebugInfoFile *self,
                         const LilyMirDebugInfoFile *other);

static VARIANT_DESTRUCTOR(LilyMirDebugInfo, file, LilyMirDebugInfo *self);

bool
eq__LilyMirDebugInfoFile(const LilyMirDebugInfoFile *self,
                         const LilyMirDebugInfoFile *other)
{
    return !strcmp(self->filename->buffer, other->filename->buffer) &&
           !strcmp(self->directory->buffer, other->directory->buffer);
}

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

bool
eq__LilyMirDebugInfo(const LilyMirDebugInfo *self,
                     const LilyMirDebugInfo *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_MIR_DEBUG_INFO_KIND_FILE:
            return eq__LilyMirDebugInfoFile(&self->file, &other->file);
        case LILY_MIR_DEBUG_INFO_KIND_BLOCK:
            return eq__LilyMirDebugInfo(self->block.scope,
                                        other->block.scope) &&
                   eq__LilyMirDebugInfoFile(self->block.file,
                                            other->block.file) &&
                   self->block.line == other->block.line &&
                   self->block.column == other->block.column;
        case LILY_MIR_DEBUG_INFO_KIND_LOCATION:
            return eq__LilyMirDebugInfo(self->location.scope,
                                        other->location.scope) &&
                   self->location.line == other->location.line &&
                   self->location.column == other->location.column;
        case LILY_MIR_DEBUG_INFO_KIND_SUB_PROGRAM:
            return eq__LilyMirDebugInfo(self->sub_program.scope,
                                        other->sub_program.scope) &&
                   eq__LilyMirDebugInfoFile(self->sub_program.file,
                                            other->sub_program.file) &&
                   self->sub_program.line == other->sub_program.line &&
                   self->sub_program.column == other->sub_program.column;
        case LILY_MIR_DEBUG_INFO_KIND_ENUMERATOR:
            return !strcmp(self->enumerator.name, other->enumerator.name) &&
                   self->enumerator.value == other->enumerator.value &&
                   self->enumerator.is_unsigned ==
                     other->enumerator.is_unsigned;
        case LILY_MIR_DEBUG_INFO_KIND_GLOBAL_VARIABLE:
            return eq__LilyMirDebugInfo(self->global_variable.scope,
                                        other->global_variable.scope) &&
                   eq__LilyMirDebugInfoFile(self->global_variable.file,
                                            other->sub_program.file) &&
                   !strcmp(self->global_variable.name,
                           other->global_variable.name) &&
                   !strcmp(self->global_variable.linkage_name,
                           other->global_variable.linkage_name) &&
                   self->global_variable.is_local ==
                     other->global_variable.is_local &&
                   self->global_variable.is_definition ==
                     other->global_variable.is_definition;
        case LILY_MIR_DEBUG_INFO_KIND_LOCAL_VARIABLE:
            return eq__LilyMirDebugInfo(self->local_variable.scope,
                                        other->local_variable.scope) &&
                   eq__LilyMirDebugInfoFile(self->local_variable.file,
                                            other->local_variable.file) &&
                   eq__LilyMirDebugInfo(self->local_variable.type,
                                        other->local_variable.type) &&
                   !strcmp(self->local_variable.name,
                           other->local_variable.name) &&
                   self->local_variable.arg_count ==
                     other->local_variable.arg_count &&
                   self->local_variable.line == other->local_variable.line;
        case LILY_MIR_DEBUG_INFO_KIND_TYPE:
            return !strcmp(self->type.name, other->type.name) &&
                   self->type.size == other->type.size &&
                   self->type.encoding == other->type.encoding;
        case LILY_MIR_DEBUG_INFO_KIND_DERIVED_TYPE:
            return eq__LilyMirDebugInfo(self->derived_type.scope,
                                        other->derived_type.scope) &&
                   eq__LilyMirDebugInfo(self->derived_type.base_type,
                                        other->derived_type.base_type) &&
                   self->derived_type.tag == other->derived_type.tag &&
                   self->derived_type.size == other->derived_type.size &&
                   self->derived_type.align == other->derived_type.align &&
                   self->derived_type.offset == other->derived_type.offset;
        case LILY_MIR_DEBUG_INFO_KIND_COMP_TYPE:
            return self->comp_type.tag == other->comp_type.tag &&
                   !strcmp(self->comp_type.name, other->comp_type.name) &&
                   self->comp_type.size == other->comp_type.size &&
                   self->comp_type.align == other->comp_type.align &&
                   eq__LilyMirDebugInfo(self->comp_type.elements,
                                        other->comp_type.elements);
        case LILY_MIR_DEBUG_INFO_KIND_ELEMENTS:
            if (self->elements.items->len != other->elements.items->len) {
                return false;
            }

            for (Usize i = 0; i < self->elements.items->len; ++i) {
                if (!eq__LilyMirDebugInfo(get__Vec(self->elements.items, i),
                                          get__Vec(other->elements.items, i))) {
                    return false;
                }
            }

            return true;
        default:
            UNREACHABLE("unknown variant");
    }
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

LilyMirDebugInfo *
add__LilyMirDebugInfoManager(LilyMirDebugInfoManager *self,
                             LilyMirDebugInfo *debug_info)
{
    for (Usize i = 0; i < self->debug_infos->len; ++i) {
        LilyMirDebugInfo *item = get__Vec(self->debug_infos, i);

        if (eq__LilyMirDebugInfo(item, debug_info)) {
            FREE(LilyMirDebugInfo, debug_info);

            return item;
        }
    }

    push__Vec(self->debug_infos, debug_info);

    ++self->count;

    return debug_info;
}

DESTRUCTOR(LilyMirDebugInfoManager, const LilyMirDebugInfoManager *self)
{
    FREE_BUFFER_ITEMS(
      self->debug_infos->buffer, self->debug_infos->len, LilyMirDebugInfo);
    FREE(Vec, self->debug_infos);
}
