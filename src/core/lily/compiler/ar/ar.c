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

#include <base/macros.h>

#include <core/lily/compiler/ar/ar.h>
#include <core/lily/compiler/ir/llvm/ar.h>
#include <core/lily/package/library.h>

#include <stdio.h>
#include <stdlib.h>

void
compile_lib__LilyLinker(LilyLibrary *self)
{
    switch (self->ar) {
        case LILY_AR_KIND_CC:
            // TODO: add cpp archiver
            break;
        case LILY_AR_KIND_CPP:
            // TODO: add cc archiver
            break;
        case LILY_AR_KIND_LLVM:
            return compile_lib__LilyIrLlvmAr(self);
        case LILY_AR_KIND_UNKNOWN:
            UNREACHABLE("unknown ar kind, maybe you are using the interpreter");
        default:
            UNREACHABLE("unknown variant");
    }
}
