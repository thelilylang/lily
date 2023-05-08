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

#include <core/lily/ir/llvm/generator/data_type.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdio.h>
#include <stdlib.h>

LLVMTypeRef
generate_data_type__LilyIrLlvm(const LilyIrLlvm *self,
                               LilyCheckedDataType *data_type)
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
            TODO("generate bytes data type");
        case LILY_CHECKED_DATA_TYPE_KIND_CHAR:
            return i8__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_CUSTOM:
            TODO("generate custom data type");
        case LILY_CHECKED_DATA_TYPE_KIND_EXCEPTION:
            TODO("generate exception data type");
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT32:
            return float__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_FLOAT64:
            return double__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT16:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT16:
            return i16__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT32:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT32:
            return i32__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT64:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT64:
            return i64__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_INT8:
        case LILY_CHECKED_DATA_TYPE_KIND_UINT8:
            return i8__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_ISIZE:
        case LILY_CHECKED_DATA_TYPE_KIND_USIZE:
            return intptr__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_LAMBDA:
            TODO("generate lambda data type");
        case LILY_CHECKED_DATA_TYPE_KIND_LIST:
            TODO("generate list data type");
        case LILY_CHECKED_DATA_TYPE_KIND_MUT:
            TODO("generate mut data type");
        case LILY_CHECKED_DATA_TYPE_KIND_NEVER:
        case LILY_CHECKED_DATA_TYPE_KIND_UNIT:
            return void__LilyIrLlvm(self);
        case LILY_CHECKED_DATA_TYPE_KIND_OPTIONAL:
            TODO("generate optional data type");
        case LILY_CHECKED_DATA_TYPE_KIND_PTR:
            return ptr__LilyIrLlvm(
              self, generate_data_type__LilyIrLlvm(self, data_type->ptr));
        case LILY_CHECKED_DATA_TYPE_KIND_REF:
            TODO("generate ref data type");
        case LILY_CHECKED_DATA_TYPE_KIND_STR:
            TODO("generate str data type");
        case LILY_CHECKED_DATA_TYPE_KIND_TRACE:
            return generate_data_type__LilyIrLlvm(self, data_type->trace);
        case LILY_CHECKED_DATA_TYPE_KIND_TUPLE:
            TODO("generate tuple data type");
        case LILY_CHECKED_DATA_TYPE_KIND_UNKNOWN:
            UNREACHABLE("impossible to get unknown data type");
        default:
            UNREACHABLE("unknown variant");
    }
}
