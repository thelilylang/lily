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

#include <core/lily/compiler/ir/llvm/generator/data_type.h>
#include <core/lily/compiler/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

LLVMTypeRef
generate_data_type__LilyIrLlvm(const LilyIrLlvm *self,
                               LilyCheckedDataType *data_type,
                               LilyLlvmScope *scope)
{
    switch (data_type->kind) {
        case LILY_CHECKED_DATA_TYPE_KIND_ANY:
            return ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self));
        case LILY_CHECKED_DATA_TYPE_KIND_ARRAY:
            TODO("generate array data type");
        case LILY_CHECKED_DATA_TYPE_KIND_BOOL:
            return i1__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_BYTE:
            return i8__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_BYTES:
            if (data_type->bytes == -1) {
                return ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self));
            }

            return LLVMArrayType(i8__LilyIrLlvm(self), data_type->bytes);
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
            return i8__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            return LLVMGetTypeByName2(self->context,
                                      data_type->custom.global_name->buffer);
            // return search_type__LilyLlvmScope(scope,
            //                                   data_type->custom.global_name)->type;
        case LILY_CHECKED_DATA_TYPE_KIND_RESULT:
            TODO("generate exception data type");
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
        case LILY_CHECKED_DATA_TYPE_KIND_CFLOAT:
            return float__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
        case LILY_CHECKED_DATA_TYPE_KIND_CDOUBLE:
            return double__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
        case LILY_CHECKED_DATA_TYPE_KIND_CSHORT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUSHORT:
            return i16__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
        case LILY_CHECKED_DATA_TYPE_KIND_CINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CUINT:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONG:
            return i32__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
        case LILY_CHECKED_DATA_TYPE_KIND_CLONGLONG:
        case LILY_CHECKED_DATA_TYPE_KIND_CULONGLONG:
            return i64__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_CSTR:
            return ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self));
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return i8__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return intptr__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            TODO("generate lambda data type");
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            return LLVMStructType(
              (LLVMTypeRef[]){ ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)),
                               intptr__LilyIrLlvm(self) },
              2,
              0);
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            return generate_data_type__LilyIrLlvm(self, data_type->mut, scope);
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
        case LILY_CHECKED_DATA_TYPE_KIND_CVOID:
            return void__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            TODO("generate optional data type");
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            return ptr__LilyIrLlvm(
              self,
              generate_data_type__LilyIrLlvm(self, data_type->ptr, scope));
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            return LLVMStructType(
              (LLVMTypeRef[]){ ptr__LilyIrLlvm(self, i8__LilyIrLlvm(self)),
                               intptr__LilyIrLlvm(self) },
              2,
              0);
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return generate_data_type__LilyIrLlvm(
              self, data_type->trace, scope);
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE: {
            LLVMTypeRef types[252] = { 0 };

            for (Usize i = 0; i < data_type->tuple->len; ++i) {
                types[i] = generate_data_type__LilyIrLlvm(
                  self, get__Vec(data_type->tuple, i), scope);
            }

            return LLVMStructType(types, data_type->tuple->len, false);
        }
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            // TODO: Replace todo with unreachable after performing a type
            // inference system.
            TODO("impossible to get unknown data type");
        default:
            UNREACHABLE("unknown variant");
    }
}
