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

#ifndef LILY_CORE_LILY_CHECKED_SCOPE_H
#define LILY_CORE_LILY_CHECKED_SCOPE_H

#include <base/alloc.h>
#include <base/macros.h>
#include <base/string.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/checked/scope_container.h>
#include <core/lily/file.h>
#include <core/lily/package.h>
#include <core/lily/visibility.h>

typedef struct LilyCheckedScope
{
    Vec *modules;        // Vec<LilyCheckedScopeContainerModule*>*
    Vec *constants;      // Vec<LilyCheckedScopeContainerConstant*>*
    Vec *enums;          // Vec<LilyCheckedScopeContainerenum*>*
    Vec *records;        // Vec<LilyCheckedScopeContainerRecord*>*
    Vec *aliases;        // Vec<LilyCheckedScopeContainerAlias*>*
    Vec *enums_object;   // Vec<LilyCheckedScopeContainerEnumObject*>*
    Vec *records_object; // Vec<LilyCheckedScopeContainerRecordObject*>*
    Vec *classes;        // Vec<LilyCheckedScopeContainerClass*>*
    Vec *traits;         // Vec<LilyCheckedScopeContainerTrait*>*
    Vec *funs;           // Vec<LilyCheckedScopeContainerFun*>*
    Vec *labels;         // Vec<LilyCheckedScopeContainerLabel*>*
    Vec *variables;      // Vec<LilyCheckedScopeContainerVariable*>*
    LilyCheckedAccessScope *parent; // LilyCheckedAccessScope*?
    Vec *children;                  // Vec<LilyCheckedAccessScope*>*
} LilyCheckedScope;

/**
 *
 * @brief Construct LilyCheckedScope type.
 * @param parent LilyCheckedAccessScope*?
 */
CONSTRUCTOR(LilyCheckedScope *,
            LilyCheckedScope,
            LilyCheckedAccessScope *parent);

/**
 *
 * @brief Free LilyCheckedScope type.
 */
DESTRUCTOR(LilyCheckedScope, LilyCheckedScope *self);

#endif // LILY_CORE_LILY_CHECKED_SCOPE_H
