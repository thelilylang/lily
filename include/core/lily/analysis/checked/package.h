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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_PACKAGE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_PACKAGE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/access.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedDecl LilyCheckedDecl;
typedef struct LilyCheckedScope LilyCheckedScope;

// TODO: Create LilyCheckedPackageID generator.
typedef struct LilyCheckedPackage
{
    Usize id;
    Vec *constants;      // Vec<LilyCheckedDecl*>*?
    Vec *enums;          // Vec<LilyCheckedDecl*>*?
    Vec *records;        // Vec<LilyCheckedDecl*>*?
    Vec *aliases;        // Vec<LilyCheckedDecl*>*?
    Vec *errors;         // Vec<LilyCheckedDecl*>*?
    Vec *enums_object;   // Vec<LilyCheckedDecl*>*?
    Vec *records_object; // Vec<LilyCheckedDecl*>*?
    Vec *classes;        // Vec<LilyCheckedDecl*>*?
    Vec *traits;         // Vec<LilyCheckedDecl*>*?
    Vec *funs;           // Vec<LilyCheckedDecl*>*?
    Vec *modules;        // Vec<LilyCheckedDecl*>*?
    Vec *scopes;         // Vec<LilyCheckedScope*>*?
} LilyCheckedPackage;

/**
 *
 * @brief Construct LilyCheckedPackage type.
 */
inline CONSTRUCTOR(LilyCheckedPackage,
                   LilyCheckedPackage,
                   Usize id,
                   LilyCheckedDecl *module)
{
    return (LilyCheckedPackage){ .id = id,
                                 .constants = NULL,
                                 .enums = NULL,
                                 .records = NULL,
                                 .aliases = NULL,
                                 .errors = NULL,
                                 .enums_object = NULL,
                                 .records_object = NULL,
                                 .classes = NULL,
                                 .traits = NULL,
                                 .funs = NULL,
                                 .modules =
                                   module ? init__Vec(1, module) : NULL,
                                 .scopes = NULL };
}

/**
 *
 * @brief Push a constant to the package.
 */
void
push_constant__LilyCheckedPackage(LilyCheckedPackage *self,
                                  LilyCheckedDecl *constant);

/**
 *
 * @brief Push an enum to the package.
 */
void
push_enum__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *enum_);

/**
 *
 * @brief Push a record to the package.
 */
void
push_record__LilyCheckedPackage(LilyCheckedPackage *self,
                                LilyCheckedDecl *record);

/**
 *
 * @brief Push an alias to the package.
 */
void
push_alias__LilyCheckedPackage(LilyCheckedPackage *self,
                               LilyCheckedDecl *alias);

/**
 *
 * @brief Push an error to the package.
 */
void
push_error__LilyCheckedPackage(LilyCheckedPackage *self,
                               LilyCheckedDecl *error);

/**
 *
 * @brief Push an enum object to the package.
 */
void
push_enum_object__LilyCheckedPackage(LilyCheckedPackage *self,
                                     LilyCheckedDecl *enum_object);

/**
 *
 * @brief Push a record object to the package.
 */
void
push_record_object__LilyCheckedPackage(LilyCheckedPackage *self,
                                       LilyCheckedDecl *record_object);

/**
 *
 * @brief Push a class to the package.
 */
void
push_class__LilyCheckedPackage(LilyCheckedPackage *self,
                               LilyCheckedDecl *class);

/**
 *
 * @brief Push a trait to the package.
 */
void
push_trait__LilyCheckedPackage(LilyCheckedPackage *self,
                               LilyCheckedDecl *trait);

/**
 *
 * @brief Push a fun to the package.
 */
void
push_fun__LilyCheckedPackage(LilyCheckedPackage *self, LilyCheckedDecl *fun);

/**
 *
 * @brief Push a module to the package.
 */
void
push_module__LilyCheckedPackage(LilyCheckedPackage *self,
                                LilyCheckedDecl *module);

/**
 *
 * @brief Convert LilyCheckedPackage in String.
 * @note This function is only used to debug.
 */
#ifdef ENV_DEBUG
String *
IMPL_FOR_DEBUG(to_string, LilyCheckedPackage, const LilyCheckedPackage *self);
#endif

/**
 *
 * @brief Free LilyCheckedPackage type.
 */
DESTRUCTOR(LilyCheckedPackage, const LilyCheckedPackage *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_PACKAGE_H
