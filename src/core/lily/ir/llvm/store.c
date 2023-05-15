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

#include <core/lily/ir/llvm/store.h>

bool
can_store_in_record_call__LilyIrLlvm(LilyCheckedExpr *expr)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_LITERAL:
            switch (expr->literal.kind) {
                case LILY_CHECKED_EXPR_LITERAL_KIND_STR:
                    return false;
                default:
                    return true;
            }
        default:
            return true;
    }
}

bool
can_store_in_variable_alloc__LilyIrLlvm(LilyCheckedExpr *expr)
{
    if (expr->data_type->kind != LILY_CHECKED_DATA_TYPE_KIND_CUSTOM) {
        switch (expr->kind) {
            case LILY_CHECKED_EXPR_KIND_LITERAL:
                switch (expr->literal.kind) {
                    case LILY_CHECKED_EXPR_LITERAL_KIND_STR:
                        return false;
                    default:
                        return true;
                }
            default:
                return true;
        }
    }

    switch (expr->call.kind) {
        case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
        case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
            return true;
        default:
            return false;
    }
}
