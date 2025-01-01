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

#ifndef LILY_CORE_LILY_MIR_GENERATOR_BODY_H
#define LILY_CORE_LILY_MIR_GENERATOR_BODY_H

#define GENERATE_BODY_ITEM(                                                 \
  module, signature, scope, block_limit, exit_block, next_block, item)      \
    switch (item->kind) {                                                   \
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_EXPR: {                        \
            LilyMirInstruction *expr = generate_expr__LilyMir(              \
              module, signature, scope, item->expr, NULL, false);           \
                                                                            \
            if (expr) {                                                     \
                LilyMirAddInst(module, expr);                               \
            }                                                               \
                                                                            \
            break;                                                          \
        }                                                                   \
        case LILY_CHECKED_BODY_FUN_ITEM_KIND_STMT: {                        \
            LilyMirInstruction *stmt = generate_stmt__LilyMir(module,       \
                                                              signature,    \
                                                              scope,        \
                                                              block_limit,  \
                                                              exit_block,   \
                                                              next_block,   \
                                                              &item->stmt); \
                                                                            \
            if (stmt) {                                                     \
                LilyMirAddInst(module, stmt);                               \
            }                                                               \
                                                                            \
            break;                                                          \
        }                                                                   \
        default:                                                            \
            UNREACHABLE("unknown variant");                                 \
    }

#define GENERATE_BODY(                                                 \
  module, signature, scope, block_limit, exit_block, next_block, body) \
    for (Usize i = 0; i < body->len; ++i) {                            \
        LilyCheckedBodyFunItem *item = get__Vec(body, i);              \
        GENERATE_BODY_ITEM(module,                                     \
                           signature,                                  \
                           scope,                                      \
                           block_limit,                                \
                           exit_block,                                 \
                           next_block,                                 \
                           item);                                      \
    }

#endif // LILY_CORE_LILY_MIR_GENERATOR_BODY_H
