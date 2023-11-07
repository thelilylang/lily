/*
 * MIT License
 *
 * Copyright (c) 2022-2024 ArthurPV
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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_MODULE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_MODULE_H

#include <base/macros.h>
#include <base/string.h>
#include <base/vec.h>

#include <core/lily/analysis/checked/access.h>
#include <core/lily/shared/visibility.h>

typedef struct LilyCheckedDecl LilyCheckedDecl;
typedef struct LilyCheckedScopeModule LilyCheckedScopeModule;

typedef struct LilyCheckedDeclModule
{
    String *name; // String* (&)
    String *global_name;
    Vec *constants;      // Vec<LilyCheckedDecl*>*?
    Vec *enums;          // Vec<LilyCheckedDecl*>*?
    Vec *records;        // Vec<LilyCheckedDecl*>*?
    Vec *aliases;        // Vec<LilyCheckedDecl*>*?
    Vec *errors;         // Vec<LilyCheckedDecl*>*?
    Vec *enums_object;   // Vec<LilyCheckedDecl*>*?
    Vec *modules;        // Vec<LilyCheckedDecl*>*?
    Vec *records_object; // Vec<LilyCheckedDecl*>*?
    Vec *classes;        // Vec<LilyCheckedDecl*>*?
    Vec *traits;         // Vec<LilyCheckedDecl*>*?
    Vec *funs;           // Vec<LilyCheckedDecl*>*?
    LilyCheckedScopeModule *scope;
    enum LilyVisibility visibility;
    bool is_checked;
} LilyCheckedDeclModule;

/**
 *
 * @brief Construct LilyCheckedDeclModule type.
 */
inline CONSTRUCTOR(LilyCheckedDeclModule,
                   LilyCheckedDeclModule,
                   String *name,
                   String *global_name,
                   Vec *decls,
                   LilyCheckedScopeModule *scope,
                   enum LilyVisibility visibility)
{
    return (LilyCheckedDeclModule){ .name = name,
                                    .global_name = global_name,
                                    .constants = NULL,
                                    .enums = NULL,
                                    .records = NULL,
                                    .aliases = NULL,
                                    .errors = NULL,
                                    .enums_object = NULL,
                                    .modules = NULL,
                                    .records_object = NULL,
                                    .classes = NULL,
                                    .traits = NULL,
                                    .funs = NULL,
                                    .scope = scope,
                                    .visibility = visibility,
                                    .is_checked = false };
}

/**
 *
 * @brief Push a constant to the module.
 */
void
push_constant__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                     LilyCheckedDecl *constant);

/**
 *
 * @brief Push an enum to the module.
 */
void
push_enum__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                 LilyCheckedDecl *enum_);

/**
 *
 * @brief Push a record to the module.
 */
void
push_record__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                   LilyCheckedDecl *record);

/**
 *
 * @brief Push an alias to the module.
 */
void
push_alias__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                  LilyCheckedDecl *alias);

/**
 *
 * @brief Push an error to the module.
 */
void
push_error__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                  LilyCheckedDecl *error);

/**
 *
 * @brief Push an enum object to the module.
 */
void
push_enum_object__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                        LilyCheckedDecl *enum_object);

/**
 *
 * @brief Push a module to the module.
 */
void
push_module__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                   LilyCheckedDecl *module);

/**
 *
 * @brief Push a record object to the module.
 */
void
push_record_object__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                          LilyCheckedDecl *record_object);

/**
 *
 * @brief Push a class to the module.
 */
void
push_class__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                  LilyCheckedDecl *class);

/**
 *
 * @brief Push a trait to the module.
 */
void
push_trait__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                  LilyCheckedDecl *trait);

/**
 *
 * @brief Push a fun to the module.
 */
void
push_fun__LilyCheckedDeclModule(LilyCheckedDeclModule *self,
                                LilyCheckedDecl *fun);

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

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_DECL_MODULE_H
