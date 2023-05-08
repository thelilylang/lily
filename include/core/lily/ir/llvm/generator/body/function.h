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

#ifndef LILY_CORE_LILY_IR_LLVM_GENERATOR_BODY_FUNCTION_H
#define LILY_CORE_LILY_IR_LLVM_GENERATOR_BODY_FUNCTION_H

#define GENERATE_FUNCTION_BODY(body, fun, exit_block, cond_block)  \
    for (Usize i = 0; i < body->len; ++i) {                        \
        LilyCheckedBodyFunItem *item = get__Vec(body, i);          \
                                                                   \
        switch (item->kind) {                                      \
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR:             \
                generate_expr__LilyIrLlvm(self, item->expr);       \
                                                                   \
                break;                                             \
            case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT:             \
                generate_stmt__LilyIrLlvm(                         \
                  self, &item->stmt, fun, exit_block, cond_block); \
                                                                   \
                break;                                             \
            default:                                               \
                UNREACHABLE("unknown variant");                    \
        }                                                          \
    }

#endif // LILY_CORE_LILY_IR_LLVM_GENERATOR_BODY_FUNCTION_H