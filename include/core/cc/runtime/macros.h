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

#ifndef LILY_CORE_CC_RUNTIME_MACROS_H
#define LILY_CORE_CC_RUNTIME_MACROS_H

#undef __NOP_
#undef __NOP__

// The "NOP" macro is a little trick to force evaluation of macros passed as
// parameter before stringification.
#define __NOP_(x) #x
#define __NOP__(x) __NOP_(x)

#undef __64__
#undef __32__

#if __SIZE_WIDTH__ == 64
#define __64__
#elif __SIZE_WIDTH__ == 32
#define __32__
#else
#error "Expected 32 or 64 bits arch"
#endif

#endif // LILY_CORE_CC_RUNTIME_MACROS_H
