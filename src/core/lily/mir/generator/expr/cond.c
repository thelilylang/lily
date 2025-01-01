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

#include <core/lily/mir/generator/expr.h>
#include <core/lily/mir/generator/expr/cond.h>

static threadlocal enum LilyCheckedExprBinaryKind last_cond_binary = -1;

#define GENERATE_ASSIGN0_BLOCK()                                            \
    /* Generate assign to 0 block */                                        \
    /* assign0: */                                                          \
    LilyMirAddBlock(module, current_assign0_block);                         \
    LilyMirSetBlockLimit(current_assign0_block->block.limit,                \
                         current_assign0_block->block.id);                  \
                                                                            \
    /* store val(i1) %.0, val(i1) 0 */                                      \
    LilyMirAddInst(                                                         \
      module,                                                               \
      NEW_VARIANT(LilyMirInstruction,                                       \
                  store,                                                    \
                  NEW(LilyMirInstructionDestSrc,                            \
                      ref__LilyMirInstructionVal(current_virtual_variable), \
                      NEW_VARIANT(LilyMirInstructionVal,                    \
                                  int,                                      \
                                  NEW(LilyMirDt, LILY_MIR_DT_KIND_I1),      \
                                  0))));                                    \
                                                                            \
    /* jmp exit_block */                                                    \
    LilyMirAddInst(module,                                                  \
                   LilyMirBuildJmpWithInst(module, current_exit_block));    \
                                                                            \
    LilyMirPopBlock(module);

#define GENERATE_ASSIGN1_BLOCK()                                            \
    /* Generate assign to 1 block */                                        \
    /* assign1: */                                                          \
    LilyMirAddBlock(module, current_assign1_block);                         \
    LilyMirSetBlockLimit(current_assign1_block->block.limit,                \
                         current_assign1_block->block.id);                  \
                                                                            \
    /* store val(i1) %.0, val(i1) 1 */                                      \
    LilyMirAddInst(                                                         \
      module,                                                               \
      NEW_VARIANT(LilyMirInstruction,                                       \
                  store,                                                    \
                  NEW(LilyMirInstructionDestSrc,                            \
                      ref__LilyMirInstructionVal(current_virtual_variable), \
                      NEW_VARIANT(LilyMirInstructionVal,                    \
                                  int,                                      \
                                  NEW(LilyMirDt, LILY_MIR_DT_KIND_I1),      \
                                  1))));                                    \
                                                                            \
    /* jmp exit_block */                                                    \
    LilyMirAddInst(module,                                                  \
                   LilyMirBuildJmpWithInst(module, current_exit_block));    \
                                                                            \
    LilyMirPopBlock(module);

#define IS_LAST_EXPR()                                              \
    expr->kind == LILY_CHECKED_EXPR_KIND_BINARY                     \
      ? !is_node__LilyCheckedExpr(expr->binary.right)               \
          ? is_second || !exit_block                                \
        : expr->binary.right->kind == LILY_CHECKED_EXPR_KIND_BINARY \
          ? (expr->binary.right->binary.kind !=                     \
               LILY_CHECKED_EXPR_BINARY_KIND_AND &&                 \
             expr->binary.right->binary.kind !=                     \
               LILY_CHECKED_EXPR_BINARY_KIND_OR) &&                 \
              (is_second || !exit_block)                            \
          : is_second || !exit_block                                \
      : is_second || !exit_block

#define GENERATE_COND_BLOCK()                                                  \
    {                                                                          \
        ASSERT(last_cond_binary != -1);                                        \
                                                                               \
        bool is_last_expr = IS_LAST_EXPR();                                    \
        LilyMirInstruction *cond_block =                                       \
          LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));                   \
                                                                               \
        LilyMirAddBlock(module, cond_block);                                   \
                                                                               \
        LilyMirInstruction *cond = generate_expr__LilyMir(                     \
          module, fun_signature, scope, expr, virtual_variable, false);        \
                                                                               \
        ASSERT(cond);                                                          \
        ASSERT(cond->kind == LILY_MIR_INSTRUCTION_KIND_VAL);                   \
        ASSERT((is_last_expr && assign0_block) ||                              \
               (!is_last_expr && next_block));                                 \
        ASSERT(!LilyMirHasFinalInstruction(module));                           \
                                                                               \
        /* or */                                                               \
        /* jmpcond val(i1) %r.1, assign1, next_block/assign0 */                \
        /* and */                                                              \
        /* jmpcond val(i1) %r.1, next_block/assign1, assign0 */                \
        LilyMirAddInst(module,                                                 \
                       LilyMirBuildJmpCondWithInst(                            \
                         module,                                               \
                         cond,                                                 \
                         last_cond_binary == LILY_CHECKED_EXPR_BINARY_KIND_AND \
                           ? is_last_expr ? assign1_block : next_block         \
                           : assign1_block,                                    \
                         last_cond_binary == LILY_CHECKED_EXPR_BINARY_KIND_AND \
                           ? assign0_block                                     \
                         : is_last_expr ? assign0_block                        \
                                        : next_block));                        \
                                                                               \
        LilyMirSetBlockLimit(cond_block->block.limit,                          \
                             LilyMirGetInsertBlock(module)->id);               \
        LilyMirPopBlock(module);                                               \
        partial_free__LilyMirInstruction(cond);                                \
                                                                               \
        return cond_block;                                                     \
    }

LilyMirInstruction *
generate_cond__LilyMir(LilyMirModule *module,
                       LilyCheckedSignatureFun *fun_signature,
                       LilyMirScope *scope,
                       LilyCheckedExpr *expr,
                       LilyMirInstructionVal *virtual_variable,
                       LilyMirInstruction *assign0_block,
                       LilyMirInstruction *assign1_block,
                       LilyMirInstruction *next_block,
                       LilyMirInstruction *exit_block,
                       bool is_second)
{
    // NOTE: We don't add assert to verify the kind of data type (because we
    // have the possibility to get compiler defined data type), btw that's
    // already verify in the analysis.

    ASSERT((!assign0_block && !assign1_block && !next_block && !exit_block) ||
           (assign0_block && assign1_block && next_block && exit_block));

    switch (expr->kind) {
        case LILY_CHECKED_EXPR_KIND_BINARY:
            switch (expr->binary.kind) {
                case LILY_CHECKED_EXPR_BINARY_KIND_AND: {
                    // NOTE: expected this signature: fun `and`(Bool, Bool) Bool

                    // => cond and cond
                    //
                    // ; ...
                    // var .0 = alloc i1
                    // jmp first_cond
                    // first_cond:
                    //   %r.0 = <cond>
                    //   jmpcond val(i1) %r.0, second_cond, assign0
                    // second_cond:
                    //   %r.1 = <cond>
                    //   jmpcond val(i1) %r.1, assign1, assign0
                    // assign0:
                    //   store val(i1) %.0, val(i1) 0
                    //   jmp exit_block
                    // assign1:
                    //   store val(i1) %.0, val(i1) 1
                    //   jmp exit_block
                    // exit_block:
                    //   ; ...
                    last_cond_binary = LILY_CHECKED_EXPR_BINARY_KIND_AND;

                    LilyMirInstructionVal *current_virtual_variable =
                      virtual_variable
                        ? !exit_block
                            ? ref__LilyMirInstructionVal(virtual_variable)
                            : NULL
                      : !exit_block
                        ? LilyMirBuildVirtualVariable(
                            module, NEW(LilyMirDt, LILY_MIR_DT_KIND_I1))
                        : NULL;
                    LilyMirInstruction *current_exit_block =
                      exit_block ? exit_block
                                 : LilyMirBuildBlock(
                                     module,
                                     ref__LilyMirBlockLimit(
                                       LilyMirGetInsertBlock(module)->limit));
                    LilyMirInstruction *current_assign0_block =
                      assign0_block
                        ? assign0_block
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *current_assign1_block =
                      assign1_block
                        ? assign1_block
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *second_cond = generate_cond__LilyMir(
                      module,
                      fun_signature,
                      scope,
                      expr->binary.right,
                      current_virtual_variable,
                      current_assign0_block,
                      current_assign1_block,
                      next_block ? next_block : current_exit_block,
                      current_exit_block,
                      true);
                    LilyMirInstruction *second_cond_block =
                      second_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK
                        ? second_cond
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *first_cond =
                      generate_cond__LilyMir(module,
                                             fun_signature,
                                             scope,
                                             expr->binary.left,
                                             current_virtual_variable,
                                             current_assign0_block,
                                             current_assign1_block,
                                             second_cond_block,
                                             current_exit_block,
                                             false);
                    LilyMirInstruction *first_cond_block =
                      first_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK
                        ? first_cond
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    bool is_last_expr = IS_LAST_EXPR();

                    ASSERT(first_cond);
                    ASSERT(second_cond);
                    ASSERT(
                      (!is_last_expr || next_block ||
                       second_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK) ||
                      second_cond->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

                    if (!exit_block) {
                        // jmp first_cond
                        LilyMirAddInst(
                          module,
                          LilyMirBuildJmpWithInst(module, first_cond_block));
                        LilyMirPopBlock(module);

                        GENERATE_ASSIGN0_BLOCK();
                        GENERATE_ASSIGN1_BLOCK();
                        LilyMirAddBlock(module, current_exit_block);

                        last_cond_binary = -1; // Reset value.
                    }

                    return !exit_block ? NEW_VARIANT(LilyMirInstruction,
                                                     val,
                                                     current_virtual_variable)
                                       : first_cond_block;
                }
                case LILY_CHECKED_EXPR_BINARY_KIND_OR: {
                    // NOTE: expected this signature: fun `or`(Bool, Bool) Bool

                    // => cond or cond
                    //
                    // ; ...
                    // var .0 = alloc i1
                    // jmp first_cond
                    // first_cond:
                    //   %r.0 = <cond>
                    //   jmpcond val(i1) %r.0, assign1, second_cond
                    // second_cond:
                    //   %r.1 = <cond>
                    //   jmpcond val(i1) %r.1, assign1, assign0
                    // assign0:
                    //   store val(i1) %.0, val(i1) 0
                    //   jmp exit_block
                    // assign1:
                    //   store val(i1) %.0, val(i1) 1
                    //   jmp exit_block
                    // exit_block:
                    //   ; ...
                    last_cond_binary = LILY_CHECKED_EXPR_BINARY_KIND_OR;

                    LilyMirInstructionVal *current_virtual_variable =
                      virtual_variable
                        ? !exit_block
                            ? ref__LilyMirInstructionVal(virtual_variable)
                            : NULL
                      : !exit_block
                        ? LilyMirBuildVirtualVariable(
                            module, NEW(LilyMirDt, LILY_MIR_DT_KIND_I1))
                        : NULL;
                    LilyMirInstruction *current_exit_block =
                      exit_block ? exit_block
                                 : LilyMirBuildBlock(
                                     module,
                                     ref__LilyMirBlockLimit(
                                       LilyMirGetInsertBlock(module)->limit));
                    LilyMirInstruction *current_assign0_block =
                      assign0_block
                        ? assign0_block
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *current_assign1_block =
                      assign1_block
                        ? assign1_block
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *second_cond = generate_cond__LilyMir(
                      module,
                      fun_signature,
                      scope,
                      expr->binary.right,
                      current_virtual_variable,
                      current_assign0_block,
                      current_assign1_block,
                      next_block ? next_block : current_exit_block,
                      current_exit_block,
                      true);
                    LilyMirInstruction *second_cond_block =
                      second_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK
                        ? second_cond
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    LilyMirInstruction *first_cond =
                      generate_cond__LilyMir(module,
                                             fun_signature,
                                             scope,
                                             expr->binary.left,
                                             current_virtual_variable,
                                             current_assign0_block,
                                             current_assign1_block,
                                             second_cond_block,
                                             current_exit_block,
                                             false);
                    LilyMirInstruction *first_cond_block =
                      first_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK
                        ? first_cond
                        : LilyMirBuildBlock(module, NEW(LilyMirBlockLimit));
                    bool is_last_expr = IS_LAST_EXPR();

                    ASSERT(first_cond);
                    ASSERT(second_cond);
                    ASSERT(
                      (!is_last_expr || next_block ||
                       second_cond->kind == LILY_MIR_INSTRUCTION_KIND_BLOCK) ||
                      second_cond->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

                    if (!exit_block) {
                        // jmp first_cond
                        LilyMirAddInst(
                          module,
                          LilyMirBuildJmpWithInst(module, first_cond_block));
                        LilyMirPopBlock(module);

                        GENERATE_ASSIGN0_BLOCK();
                        GENERATE_ASSIGN1_BLOCK();
                        LilyMirAddBlock(module, current_exit_block);

                        last_cond_binary = -1; // Reset value.
                    }

                    return !exit_block ? NEW_VARIANT(LilyMirInstruction,
                                                     val,
                                                     current_virtual_variable)
                                       : first_cond_block;
                }
                default:
                    GENERATE_COND_BLOCK();
            }
        case LILY_CHECKED_EXPR_KIND_GROUPING:
            return generate_cond__LilyMir(module,
                                          fun_signature,
                                          scope,
                                          expr->grouping,
                                          virtual_variable,
                                          assign0_block,
                                          assign1_block,
                                          next_block,
                                          exit_block,
                                          is_second);
        default:
            GENERATE_COND_BLOCK();
    }
}
