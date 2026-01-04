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

#ifndef LILY_CORE_LILY_MIR_GENERATOR_COND_H
#define LILY_CORE_LILY_MIR_GENERATOR_COND_H

#include <core/lily/analysis/checked/expr.h>
#include <core/lily/analysis/checked/signature.h>
#include <core/lily/mir/mir.h>

/**
 *
 * @brief Generate condition in Lily MIR.
 * @param fun_signature LilyCheckedSignatureFun*? (&)
 * @param virtual_variable LilyMirInstructionVal*? (&)
 * @param assign0_block LilyMirInstruction*? (&)
 * @param assign1_block LilyMirInstruction*? (&)
 * @param next_block LilyMirInstruction*? (&)
 * @param exit_block LilyMirInstruction*? (&)
 * @return LilyMirInstruction*?
 */
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
                       bool is_second);

#endif // LILY_CORE_LILY_MIR_GENERATOR_COND_H
