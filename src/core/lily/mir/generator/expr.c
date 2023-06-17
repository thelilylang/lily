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

#include <base/assert.h>

#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/expr/assignable.h>
#include <core/lily/mir/generator/expr/binary.h>
#include <core/lily/mir/generator/expr/call.h>
#include <core/lily/mir/generator/expr/unary.h>
#include <core/lily/mir/generator/val.h>

#include <stdio.h>
#include <string.h>

LilyMirInstruction *
generate_expr__LilyMir(LilyMirModule *module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       LilyCheckedExpr *expr)
{
    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_BINARY:
            return generate_binary_expr__LilyMir(
              module, fun_signature, scope, expr);
        case LILY_CHECKED_EXPR_KIND_CALL:
            switch (expr->call.kind) {
                case LILY_CHECKED_EXPR_CALL_KIND_VARIABLE:
                case LILY_CHECKED_EXPR_CALL_KIND_FUN_PARAM:
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_ACCESS:
                case LILY_CHECKED_EXPR_CALL_KIND_RECORD_FIELD_SINGLE:
                    return NEW_VARIANT(LilyMirInstruction,
                                       val,
                                       generate_val__LilyMir(
                                         module, fun_signature, scope, expr));
                default:
                    return generate_call_expr__LilyMir(
                      module, fun_signature, scope, expr);
            }
        case LILY_CHECKED_EXPR_KIND_CAST:
            break;
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            return generate_expr__LilyMir(
              module, fun_signature, scope, expr->grouping);
        case LILY_CHECKED_EXPR_KIND_LAMBDA:
            break;
        case LILY_CHECKED_EXPR_KIND_UNARY:
            return generate_unary_expr__LilyMir(
              module, fun_signature, scope, expr);
        default:
            return NEW_VARIANT(
              LilyMirInstruction,
              val,
              generate_val__LilyMir(module, fun_signature, scope, expr));
    }
}
