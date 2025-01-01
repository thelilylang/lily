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
#include <base/format.h>

#include <core/lily/analysis/checked/access.h>

CONSTRUCTOR(LilyCheckedAccessScope *, LilyCheckedAccessScope, Usize id)
{
    LilyCheckedAccessScope *self = lily_malloc(sizeof(LilyCheckedAccessScope));

    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessScope,
               const LilyCheckedAccessScope *self)
{
    return format("LilyCheckedAccessScope{{ id = {d} }", self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessModule,
               const LilyCheckedAccessModule *self)
{
    return format("LilyCheckedAccessModule{{ id = {d} }", self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessConstant,
               const LilyCheckedAccessConstant *self)
{
    return format("LilyCheckedAccessConstant{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessEnum,
               const LilyCheckedAccessEnum *self)
{
    return format("LilyCheckedAccessEnum{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessRecord,
               const LilyCheckedAccessRecord *self)
{
    return format("LilyCheckedAccessRecord{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessAlias,
               const LilyCheckedAccessAlias *self)
{
    return format("LilyCheckedAccessAlias{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessError,
               const LilyCheckedAccessError *self)
{
    return format("LilyCheckedAccessError{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessEnumObject,
               const LilyCheckedAccessEnumObject *self)
{
    return format("LilyCheckedAccessEnumObject{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessRecordObject,
               const LilyCheckedAccessRecordObject *self)
{
    return format("LilyCheckedAccessRecordObject{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessClass,
               const LilyCheckedAccessClass *self)
{
    return format("LilyCheckedAccessClass{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessTrait,
               const LilyCheckedAccessTrait *self)
{
    return format("LilyCheckedAccessTrait{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

CONSTRUCTOR(LilyCheckedAccessFun *,
            LilyCheckedAccessFun,
            LilyCheckedAccessScope scope,
            Usize id)
{
    LilyCheckedAccessFun *self = lily_malloc(sizeof(LilyCheckedAccessFun));

    self->scope = scope;
    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessFun,
               const LilyCheckedAccessFun *self)
{
    return format("LilyCheckedAccessFun{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif

CONSTRUCTOR(LilyCheckedAccessLabel *,
            LilyCheckedAccessLabel,
            LilyCheckedAccessScope scope,
            Usize id)
{
    LilyCheckedAccessLabel *self = lily_malloc(sizeof(LilyCheckedAccessLabel));

    self->scope = scope;
    self->id = id;

    return self;
}

#ifdef ENV_DEBUG
char *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedAccessLabel,
               const LilyCheckedAccessLabel *self)
{
    return format("LilyCheckedAccessLabel{{ scope = {sa}, id = {d} }",
                  to_string__Debug__LilyCheckedAccessScope(&self->scope),
                  self->id);
}
#endif
