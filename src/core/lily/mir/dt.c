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

#include <base/alloc.h>
#include <base/macros.h>
#include <base/new.h>

#include <core/lily/mir/dt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static VARIANT_DESTRUCTOR(LilyMirDt, array, LilyMirDt *self);

static inline VARIANT_DESTRUCTOR(LilyMirDt, bytes, LilyMirDt *self);

static inline VARIANT_DESTRUCTOR(LilyMirDt, cstr, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, list, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, ptr, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, ref, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, result, LilyMirDt *self);

static inline VARIANT_DESTRUCTOR(LilyMirDt, str, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, struct, LilyMirDt *self);

static inline VARIANT_DESTRUCTOR(LilyMirDt, struct_name, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, trace, LilyMirDt *self);

static VARIANT_DESTRUCTOR(LilyMirDt, tuple, LilyMirDt *self);

CONSTRUCTOR(LilyMirDt *, LilyMirDt, enum LilyMirDtKind kind)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = kind;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, array, LilyMirDtArray array)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_ARRAY;
    self->array = array;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, bytes, LilyMirDtLen bytes)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_BYTES;
    self->bytes = bytes;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, cstr, LilyMirDtLen cstr)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_CSTR;
    self->cstr = cstr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, list, LilyMirDt *list)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_LIST;
    self->list = list;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, ptr, LilyMirDt *ptr)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_PTR;
    self->ptr = ptr;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, ref, LilyMirDt *ref)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_REF;
    self->ref = ref;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, result, LilyMirDtResult result)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_RESULT;
    self->result = result;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, str, LilyMirDtLen str)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_STR;
    self->str = str;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, struct, Vec *struct_)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_STRUCT;
    self->struct_ = struct_;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *,
                    LilyMirDt,
                    struct_name,
                    const char *struct_name)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_STRUCT_NAME;
    self->struct_name = struct_name;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, trace, LilyMirDt *trace)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_TRACE;
    self->trace = trace;

    return self;
}

VARIANT_CONSTRUCTOR(LilyMirDt *, LilyMirDt, tuple, Vec *tuple)
{
    LilyMirDt *self = lily_malloc(sizeof(LilyMirDt));

    self->kind = LILY_MIR_DT_KIND_TUPLE;
    self->tuple = tuple;

    return self;
}

LilyMirDt *
clone__LilyMirDt(LilyMirDt *self)
{
    switch (self->kind) {
        case LILY_MIR_DT_KIND_ARRAY:
            return NEW_VARIANT(LilyMirDt,
                               array,
                               NEW(LilyMirDtArray,
                                   clone__LilyMirDtLen(&self->array.len),
                                   clone__LilyMirDt(self->array.dt)));
        case LILY_MIR_DT_KIND_BYTES:
            return NEW_VARIANT(
              LilyMirDt, bytes, clone__LilyMirDtLen(&self->bytes));
        case LILY_MIR_DT_KIND_CSTR:
            return NEW_VARIANT(
              LilyMirDt, cstr, clone__LilyMirDtLen(&self->cstr));
        case LILY_MIR_DT_KIND_LIST:
            return NEW_VARIANT(LilyMirDt, list, clone__LilyMirDt(self->list));
        case LILY_MIR_DT_KIND_PTR:
            return NEW_VARIANT(LilyMirDt, ptr, clone__LilyMirDt(self->list));
        case LILY_MIR_DT_KIND_REF:
            return NEW_VARIANT(LilyMirDt, ref, clone__LilyMirDt(self->list));
        case LILY_MIR_DT_KIND_RESULT:
            return NEW_VARIANT(LilyMirDt,
                               result,
                               NEW(LilyMirDtResult,
                                   clone__LilyMirDt(self->result.ok),
                                   clone__LilyMirDt(self->result.err)));
        case LILY_MIR_DT_KIND_STR:
            return NEW_VARIANT(LilyMirDt, str, clone__LilyMirDtLen(&self->str));
        case LILY_MIR_DT_KIND_STRUCT: {
            Vec *struct_ = NEW(Vec);

            for (Usize i = 0; i < self->struct_->len; ++i) {
                push__Vec(struct_,
                          clone__LilyMirDt(get__Vec(self->struct_, i)));
            }

            return NEW_VARIANT(LilyMirDt, struct, struct_);
        }
        case LILY_MIR_DT_KIND_STRUCT_NAME:
            return NEW_VARIANT(LilyMirDt, struct_name, self->struct_name);
        case LILY_MIR_DT_KIND_TRACE:
            return NEW_VARIANT(LilyMirDt, trace, clone__LilyMirDt(self->trace));
        case LILY_MIR_DT_KIND_TUPLE: {
            Vec *tuple = NEW(Vec);

            for (Usize i = 0; i < self->tuple->len; ++i) {
                push__Vec(tuple, clone__LilyMirDt(get__Vec(self->tuple, i)));
            }

            return NEW_VARIANT(LilyMirDt, tuple, tuple);
        }
        default:
            return NEW(LilyMirDt, self->kind);
    }
}

bool
eq__LilyMirDt(LilyMirDt *self, LilyMirDt *other)
{
    if (self->kind != other->kind) {
        return false;
    }

    switch (self->kind) {
        case LILY_MIR_DT_KIND_ARRAY:
            return eq__LilyMirDt(self->array.dt, other->array.dt) &&
                   eq__LilyMirDtLen(&self->array.len, &other->array.len);
        case LILY_MIR_DT_KIND_RESULT:
            return eq__LilyMirDt(self->result.ok, other->result.ok) &&
                   eq__LilyMirDt(self->result.err, other->result.err);
        case LILY_MIR_DT_KIND_LIST:
            return eq__LilyMirDt(self->list, other->list);
        case LILY_MIR_DT_KIND_PTR:
            return eq__LilyMirDt(self->ptr, other->ptr);
        case LILY_MIR_DT_KIND_REF:
            return eq__LilyMirDt(self->ref, other->ref);
        case LILY_MIR_DT_KIND_STRUCT:
            if (self->struct_->len != other->struct_->len) {
                return false;
            }

            for (Usize i = 0; i < self->struct_->len; ++i) {
                if (!eq__LilyMirDt(get__Vec(self->struct_, i),
                                   get__Vec(other->struct_, i))) {
                    return false;
                }
            }

            return true;
        case LILY_MIR_DT_KIND_STRUCT_NAME:
            return !strcmp(self->struct_name, other->struct_name);
        case LILY_MIR_DT_KIND_TRACE:
            return eq__LilyMirDt(self->trace, other->trace);
        case LILY_MIR_DT_KIND_TUPLE:
            if (self->tuple->len != other->tuple->len) {
                return false;
            }

            for (Usize i = 0; i < self->tuple->len; ++i) {
                if (!eq__LilyMirDt(get__Vec(self->tuple, i),
                                   get__Vec(other->tuple, i))) {
                    return false;
                }
            }

            return true;
        default:
            return true;
    }
}

bool
is_signed__LilyMirDt(LilyMirDt *self)
{
    switch (self->kind) {
        case LILY_MIR_DT_KIND_BYTES:
        case LILY_MIR_DT_KIND_U8:
        case LILY_MIR_DT_KIND_U16:
        case LILY_MIR_DT_KIND_U32:
        case LILY_MIR_DT_KIND_U64:
        case LILY_MIR_DT_KIND_USIZE:
            return false;
        case LILY_MIR_DT_KIND_I1:
        case LILY_MIR_DT_KIND_I8:
        case LILY_MIR_DT_KIND_I16:
        case LILY_MIR_DT_KIND_I32:
        case LILY_MIR_DT_KIND_I64:
        case LILY_MIR_DT_KIND_ISIZE:
            return true;
        default:
            UNREACHABLE("no expected to check if is a signed data type");
    }
}

#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyMirDt, const LilyMirDt *self)
{
    switch (self->kind) {
        case LILY_MIR_DT_KIND_ANY:
            return format__String("\x1b[35many\x1b[0m");
        case LILY_MIR_DT_KIND_ARRAY:
            if (self->array.len.is_undef) {
                return format__String(
                  "\x1b[35m[? x {Sr}]\x1b[0m",
                  to_string__Debug__LilyMirDt(self->array.dt));
            }

            return format__String("\x1b[35m[{zu} x {Sr}]\x1b[0m",
                                  self->array.len.len,
                                  to_string__Debug__LilyMirDt(self->array.dt));
        case LILY_MIR_DT_KIND_BYTES:
            return format__String("\x1b[35mBytes\x1b[0m");
        case LILY_MIR_DT_KIND_CSTR:
            return format__String("\x1b[35mCstr\x1b[0m");
        case LILY_MIR_DT_KIND_RESULT:
            return format__String(
              "\x1b[35mstruct {{{Sr}, {Sr}}\x1b[0m",
              to_string__Debug__LilyMirDt(self->result.ok),
              to_string__Debug__LilyMirDt(self->result.err));
        case LILY_MIR_DT_KIND_I1:
            return format__String("\x1b[35mi1\x1b[0m");
        case LILY_MIR_DT_KIND_I8:
            return format__String("\x1b[35mi8\x1b[0m");
        case LILY_MIR_DT_KIND_I16:
            return format__String("\x1b[35mi16\x1b[0m");
        case LILY_MIR_DT_KIND_I32:
            return format__String("\x1b[35mi32\x1b[0m");
        case LILY_MIR_DT_KIND_I64:
            return format__String("\x1b[35mi64\x1b[0m");
        case LILY_MIR_DT_KIND_ISIZE:
            return format__String("\x1b[35misize\x1b[0m");
        case LILY_MIR_DT_KIND_F32:
            return format__String("\x1b[35mf32\x1b[0m");
        case LILY_MIR_DT_KIND_F64:
            return format__String("\x1b[35mf64\x1b[0m");
        case LILY_MIR_DT_KIND_LIST:
            return format__String("\x1b[35m{{{Sr}}\x1b[0m",
                                  to_string__Debug__LilyMirDt(self->list));
        case LILY_MIR_DT_KIND_PTR:
            return format__String("\x1b[35m*{Sr}\x1b[0m",
                                  to_string__Debug__LilyMirDt(self->ptr));
        case LILY_MIR_DT_KIND_REF:
            return format__String("\x1b[35m&{Sr}\x1b[0m",
                                  to_string__Debug__LilyMirDt(self->ptr));
        case LILY_MIR_DT_KIND_STR:
            return format__String("\x1b[35mStr\x1b[0m");
        case LILY_MIR_DT_KIND_STRUCT: {
            String *res = format__String("\x1b[35mstruct {{");

            for (Usize i = 0; i < self->struct_->len; ++i) {
                String *item =
                  to_string__Debug__LilyMirDt(get__Vec(self->struct_, i));

                APPEND_AND_FREE(res, item);

                if (i + 1 != self->struct_->len) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, "}\x1b[0m");

            return res;
        }
        case LILY_MIR_DT_KIND_STRUCT_NAME:
            return format__String("\x1b[35mstruct {s}\x1b[0m",
                                  self->struct_name);
        case LILY_MIR_DT_KIND_TRACE:
            return format__String("\x1b[35mstruct {{{Sr}, usize}",
                                  to_string__Debug__LilyMirDt(self->trace));
        case LILY_MIR_DT_KIND_TUPLE: {
            String *res = format__String("\x1b[35m(");

            for (Usize i = 0; i < self->tuple->len; ++i) {
                String *item =
                  to_string__Debug__LilyMirDt(get__Vec(self->tuple, i));

                APPEND_AND_FREE(res, item);

                if (i + 1 != self->tuple->len) {
                    push_str__String(res, ", ");
                }
            }

            push_str__String(res, ")\x1b[0m");

            return res;
        }
        case LILY_MIR_DT_KIND_U8:
            return format__String("\x1b[35mu8\x1b[0m");
        case LILY_MIR_DT_KIND_U16:
            return format__String("\x1b[35mu16\x1b[0m");
        case LILY_MIR_DT_KIND_U32:
            return format__String("\x1b[35mu32\x1b[0m");
        case LILY_MIR_DT_KIND_U64:
            return format__String("\x1b[35mu64\x1b[0m");
        case LILY_MIR_DT_KIND_UNIT:
            return format__String("\x1b[35munit\x1b[0m");
        case LILY_MIR_DT_KIND_USIZE:
            return format__String("\x1b[35musize\x1b[0m");
        default:
            UNREACHABLE("unknown variant");
    }
}
#endif

VARIANT_DESTRUCTOR(LilyMirDt, array, LilyMirDt *self)
{
    FREE(LilyMirDtArray, &self->array);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, bytes, LilyMirDt *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, cstr, LilyMirDt *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, result, LilyMirDt *self)
{
    FREE(LilyMirDtResult, &self->result);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, list, LilyMirDt *self)
{
    FREE(LilyMirDt, self->list);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, ptr, LilyMirDt *self)
{
    FREE(LilyMirDt, self->ptr);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, ref, LilyMirDt *self)
{
    FREE(LilyMirDt, self->ref);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, str, LilyMirDt *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, struct, LilyMirDt *self)
{
    FREE_BUFFER_ITEMS(self->struct_->buffer, self->struct_->len, LilyMirDt);
    FREE(Vec, self->struct_);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, struct_name, LilyMirDt *self)
{
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, trace, LilyMirDt *self)
{
    FREE(LilyMirDt, self->trace);
    lily_free(self);
}

VARIANT_DESTRUCTOR(LilyMirDt, tuple, LilyMirDt *self)
{
    FREE_BUFFER_ITEMS(self->tuple->buffer, self->tuple->len, LilyMirDt);
    FREE(Vec, self->tuple);
    lily_free(self);
}

DESTRUCTOR(LilyMirDt, LilyMirDt *self)
{
    switch (self->kind) {
        case LILY_MIR_DT_KIND_ARRAY:
            FREE_VARIANT(LilyMirDt, array, self);
            break;
        case LILY_MIR_DT_KIND_BYTES:
            FREE_VARIANT(LilyMirDt, bytes, self);
            break;
        case LILY_MIR_DT_KIND_CSTR:
            FREE_VARIANT(LilyMirDt, cstr, self);
            break;
        case LILY_MIR_DT_KIND_RESULT:
            FREE_VARIANT(LilyMirDt, result, self);
            break;
        case LILY_MIR_DT_KIND_LIST:
            FREE_VARIANT(LilyMirDt, list, self);
            break;
        case LILY_MIR_DT_KIND_PTR:
            FREE_VARIANT(LilyMirDt, ptr, self);
            break;
        case LILY_MIR_DT_KIND_REF:
            FREE_VARIANT(LilyMirDt, ref, self);
            break;
        case LILY_MIR_DT_KIND_STR:
            FREE_VARIANT(LilyMirDt, str, self);
            break;
        case LILY_MIR_DT_KIND_STRUCT:
            FREE_VARIANT(LilyMirDt, struct, self);
            break;
        case LILY_MIR_DT_KIND_STRUCT_NAME:
            FREE_VARIANT(LilyMirDt, struct_name, self);
            break;
        case LILY_MIR_DT_KIND_TRACE:
            FREE_VARIANT(LilyMirDt, trace, self);
            break;
        case LILY_MIR_DT_KIND_TUPLE:
            FREE_VARIANT(LilyMirDt, tuple, self);
            break;
        default:
            lily_free(self);
    }
}
