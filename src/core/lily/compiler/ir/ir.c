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

#include <base/new.h>

#include <core/lily/compiler/ir.h>
#include <core/lily/compiler/ir/llvm/generator.h>
#include <core/lily/package/package.h>

#include <stdio.h>
#include <stdlib.h>

void
run__LilyIr(LilyPackage *self)
{
    ASSERT(self->kind == LILY_PACKAGE_KIND_COMPILER);

    switch (self->compiler.ir.kind) {
        case LILY_IR_KIND_CC:
            break;
        case LILY_IR_KIND_CPP:
            break;
        case LILY_IR_KIND_JS:
            break;
        case LILY_IR_KIND_LLVM:
            run__LilyIrLlvmGenerator(self);

            break;
        default:
            UNREACHABLE("unknown variant");
    }
}

DESTRUCTOR(LilyIr, const LilyIr *self)
{
    switch (self->kind) {
        case LILY_IR_KIND_LLVM:
            FREE(LilyIrLlvm, &self->llvm);
            break;
        default:
            break;
    }
}
