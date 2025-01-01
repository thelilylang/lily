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

#ifndef LILY_BASE_HASH_CHOICE_H
#define LILY_BASE_HASH_CHOICE_H

// Choice different algorithm to create an hash
#define HASH_FNV1A
#undef HASH_FNV1A

#define HASH_CUSTOM
#undef HASH_CUSTOM

#define HASH_JENKINS
#undef HASH_JENKINS

#define HASH_SIP
// #undef HASH_SIP

#ifdef HASH_FNV1A
#include <base/hash/fnv.h>
#elif defined(HASH_CUSTOM)
#include <base/hash/custom.h>
#elif defined(HASH_JENKINS)
#include <base/hash/jenkins.h>
#elif defined(HASH_SIP)
#include <base/hash/sip.h>
#else
#error "cannot generate an hash"
#endif

#endif // LILY_BASE_HASH_CHOICE_H