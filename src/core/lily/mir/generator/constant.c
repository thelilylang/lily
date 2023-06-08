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

#include <core/lily/mir/generator/constant.h>
#include <core/lily/mir/generator/expr.h>

LilyMirInstruction *
generate_constant__LilyMir(LilyCheckedDecl *constant)
{
    LilyMirInstruction *inst = generate_expr__LilyMir(constant->constant.expr);
    LilyMirInstructionVal *val = LilyMirGetValFromInst(inst);

    LilyMirInstruction *const_ = NEW_VARIANT(
      LilyMirInstruction,
      const,
      NEW(LilyMirInstructionConst,
          get_linkage_from_visibility(constant->constant.visibility),
          constant->constant.global_name->buffer,
          val));

    lily_free(inst);

    return const_;
}
