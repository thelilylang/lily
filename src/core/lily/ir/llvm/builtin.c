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

#include <core/lily/ir/llvm/builtin.h>
#include <core/lily/ir/llvm/primary.h>

#include <stdbool.h>

static void
init_math_builtin__LilyIrLlvm(LilyIrLlvm *self);

void
init_math_builtin__LilyIrLlvm(LilyIrLlvm *self)
{
    // declare double acos(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "acos",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double asin(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "asin",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double atan(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "atan",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double atan2(doube, double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "atan2",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }

    // declare double cos(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "cos",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double cosh(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "cosh",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double sin(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "sin",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double sinh(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "sinh",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double tanh(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "tanh",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double exp(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "exp",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double fexp(double, int*)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 ptr__LilyIrLlvm(self, i32__LilyIrLlvm(self)) };

        LLVMAddFunction(
          self->module,
          "frexp",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }

    // declare double ldexp(double, int)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 i32__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "ldexp",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }

    // declare double log(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "log",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double log10(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "log10",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double modf(double, double*)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 ptr__LilyIrLlvm(self,
                                                 double__LilyIrLlvm(self)) };

        LLVMAddFunction(
          self->module,
          "modf",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }

    // declare double pow(double, double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "pow",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }

    // declare double sqrt(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "sqrt",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double ceil(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "ceil",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double fabs(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "fabs",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double floor(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "floor",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 1, false));
    }

    // declare double fmod(double)
    {
        LLVMTypeRef params[] = { double__LilyIrLlvm(self),
                                 double__LilyIrLlvm(self) };

        LLVMAddFunction(
          self->module,
          "fmod",
          LLVMFunctionType(double__LilyIrLlvm(self), params, 2, false));
    }
}

void
init_builtin__LilyIrLlvm(LilyIrLlvm *self)
{
    init_math_builtin__LilyIrLlvm(self);
}
