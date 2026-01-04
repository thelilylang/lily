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

#ifndef LILY_CORE_LILY_ANALYSIS_CHECKED_VIRTUAL_SCOPE_H
#define LILY_CORE_LILY_ANALYSIS_CHECKED_VIRTUAL_SCOPE_H

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/analysis/checked/decl/fun.h>
#include <core/lily/analysis/checked/stmt/variable.h>

typedef struct LilyCheckedVirtualVariable
{
    const LilyCheckedStmtVariable
      *variable; // const LilyCheckedStmtVariable* (&)
    LilyCheckedDataType *virtual_data_type; // LilyCheckedDataType* (&)
} LilyCheckedVirtualVariable;

/**
 *
 * @brief Construct LilyCheckedVirtualVariable type.
 */
CONSTRUCTOR(LilyCheckedVirtualVariable *,
            LilyCheckedVirtualVariable,
            const LilyCheckedStmtVariable *variable,
            LilyCheckedDataType *virtual_data_type);

/**
 *
 * @brief Free LilyCheckedVirtualVariable type.
 */
inline DESTRUCTOR(LilyCheckedVirtualVariable, LilyCheckedVirtualVariable *self)
{
    lily_free(self);
}

typedef struct LilyCheckedVirtualFunParam
{
    const LilyCheckedDeclFunParam *param; // const LilyCheckedDeclFunParam* (&)
    LilyCheckedDataType *virtual_data_type; // LilyCheckedDataType* (&)
} LilyCheckedVirtualFunParam;

/**
 *
 * @brief Construct LilyCheckedVirtualFunParam type.
 */
CONSTRUCTOR(LilyCheckedVirtualFunParam *,
            LilyCheckedVirtualFunParam,
            const LilyCheckedDeclFunParam *param,
            LilyCheckedDataType *virtual_data_type);

/**
 *
 * @brief Free LilyCheckedVirtualFunParam type.
 */
inline DESTRUCTOR(LilyCheckedVirtualFunParam, LilyCheckedVirtualFunParam *self)
{
    lily_free(self);
}

typedef struct LilyCheckedVirtualScope
{
    struct LilyCheckedVirtualScope *parent;
    Vec *fun_params; // Vec<LilyCheckedVirtualFunParam*>*
    Vec *variables;  // Vec<LilyCheckedVirtualVariable*>*
} LilyCheckedVirtualScope;

/**
 *
 * @brief Construct LilyCheckedVirtualScope type.
 */
CONSTRUCTOR(LilyCheckedVirtualScope *,
            LilyCheckedVirtualScope,
            LilyCheckedVirtualScope *parent);

/**
 *
 * @brief Add fun param to the scope.
 */
inline void
add_fun_param__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                       LilyCheckedVirtualFunParam *fun_param)
{
    push__Vec(self->fun_params, fun_param);
}

/**
 *
 * @brief Add variable to the scope.
 */
inline void
add_variable__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                      LilyCheckedVirtualVariable *variable)
{
    push__Vec(self->variables, variable);
}

/**
 *
 * @brief Search fun param in the scope.
 */
LilyCheckedVirtualFunParam *
search_fun_param__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                          String *name);

/**
 *
 * @brief Search variable in the scope.
 */
LilyCheckedVirtualVariable *
search_variable__LilyCheckedVirtualScope(LilyCheckedVirtualScope *self,
                                         String *name);

/**
 *
 * @brief Free LilyCheckedVirtualScope type.
 */
DESTRUCTOR(LilyCheckedVirtualScope, LilyCheckedVirtualScope *self);

#endif // LILY_CORE_LILY_ANALYSIS_CHECKED_VIRTUAL_SCOPE_H
