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

#include <core/lily/mir/generator/expr/call.h>
#include <core/lily/mir/generator/dt.h>
#include <core/lily/mir/generator/expr.h>

LilyMirInstruction *
generate_call_expr__LilyMir(LilyMirModule *module, LilyCheckedExpr *expr)
{
	switch (expr->call.kind) {
		case LILY_CHECKED_EXPR_CALL_KIND_RECORD: {
			LilyMirDt *dt = generate_dt__LilyMir(expr->data_type);
			Vec *struct_ = NEW(Vec); // Vec<LilyMirInstructionVal*>*

			if (expr->call.record.params) {
				for (Usize i = 0; i < expr->call.record.params->len; ++i) {
					LilyMirInstruction *val = generate_expr__LilyMir(module, CAST(LilyCheckedExprCallRecordParam*, get__Vec(expr->call.record.params, i))->value);

					ASSERT(val->kind == LILY_MIR_INSTRUCTION_KIND_VAL);

					push__Vec(struct_, val->val);
					lily_free(val);
				}
			}

			return NEW_VARIANT(LilyMirInstruction, val, LilyMirBuildStruct(module, dt, struct_));
		}
		default:
			UNREACHABLE("not expected to generate in this function");
	}
}
