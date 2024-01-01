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

#include <base/assert.h>
#include <base/box.h>
#include <base/macros.h>
#include <base/new.h>

#include <stdio.h>
#include <stdlib.h>

IMPL_BOX(Int8);
IMPL_BOX(Int16);
IMPL_BOX(Int32);
IMPL_BOX(Int64);

IMPL_BOX(Uint8);
IMPL_BOX(Uint16);
IMPL_BOX(Uint32);
IMPL_BOX(Uint64);

IMPL_BOX(Isize);
IMPL_BOX(Usize);

IMPL_BOX(Float32);
IMPL_BOX(Float64);

IMPL_BOX(Uptr);

IMPL_BOX(Bool);
IMPL_BOX(Char);
IMPL_BOX(Short);
IMPL_BOX(Int);
IMPL_BOX(Long);
IMPL_BOX(Longlong);

IMPL_BOX(Uchar);
IMPL_BOX(Ushort);
IMPL_BOX(Uint);
IMPL_BOX(Ulong);
IMPL_BOX(Ulonglong);
