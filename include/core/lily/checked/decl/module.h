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

#ifndef LILY_CORE_LILY_CHECKED_DECL_MODULE_H
#define LILY_CORE_LILY_CHECKED_DECL_MODULE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/checked/access.h>
#include <core/lily/checked/scope.h>
#include <core/lily/visibility.h>

typedef struct LilyCheckedDeclModule
{
    String *name; // String* (&)
    Vec *decls;   // Vec<LilyCheckedDecl*>*
    LilyCheckedScope *scope;
    LilyCheckedAccessModule access;
    enum LilyVisibility visibility;
} LilyCheckedDeclModule;

/**
 *
 * @brief Construct LilyCheckedDeclModule type.
 */
inline CONSTRUCTOR(LilyCheckedDeclModule,
                   LilyCheckedDeclModule,
                   String *name,
                   Vec *decls,
                   LilyCheckedScope *scope,
                   LilyCheckedAccessModule access,
                   enum LilyVisibility visibility)
{
    return (LilyCheckedDeclModule){ .name = name,
                                    .decls = decls,
                                    .scope = scope,
                                    .access = access,
                                    .visibility = visibility };
}

/**
 *
 * @brief Convert LilyCheckedDeclModule in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string,
               LilyCheckedDeclModule,
               const LilyCheckedDeclModule *self);
#endif

/**
 *
 * @brief Free LilyCheckedDeclModule type.
 */
DESTRUCTOR(LilyCheckedDeclModule, const LilyCheckedDeclModule *self);

#endif // LILY_CORE_LILY_CHECKED_DECL_MODULE_H
