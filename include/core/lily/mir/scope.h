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

#ifndef LILY_CORE_LILY_MIR_SCOPE_H
#define LILY_CORE_LILY_MIR_SCOPE_H

#include <base/vec.h>

#include <core/lily/analysis/checked/data_type.h>
#include <core/lily/mir/block_limit.h>

typedef struct LilyMirScopeParam
{
    LilyCheckedDataType *data_type; // LilyCheckedDataType* (&)
} LilyMirScopeParam;

/**
 *
 * @brief Construct LilyMirScopeParam type.
 */
CONSTRUCTOR(LilyMirScopeParam *,
            LilyMirScopeParam,
            LilyCheckedDataType *data_type);

/**
 *
 * @brief Free LilyMirScopeParam type.
 */
inline DESTRUCTOR(LilyMirScopeParam, LilyMirScopeParam *self)
{
    lily_free(self);
}

typedef struct LilyMirScopeVar
{
    String *name;                   // String* (&)
    LilyCheckedDataType *data_type; // LilyCheckedDataType* (&)
} LilyMirScopeVar;

/**
 *
 * @brief Construct LilyMirScopeVar type.
 */
CONSTRUCTOR(LilyMirScopeVar *,
            LilyMirScopeVar,
            String *name,
            LilyCheckedDataType *data_type);

/**
 *
 * @brief Free LilyMirScopeVar type.
 */
inline DESTRUCTOR(LilyMirScopeVar, LilyMirScopeVar *self)
{
    lily_free(self);
}

typedef struct LilyMirScopeField
{
    String *global_name;                         // String* (&)
    LilyCheckedDataType *data_type;              // LilyCheckedDataType* (&)
    struct LilyMirScopeInstance *field_instance; // LilyMirScopeInstance*?
} LilyMirScopeField;

/**
 *
 * @brief Construct LilyMirScopeField type.
 */
CONSTRUCTOR(LilyMirScopeField *,
            LilyMirScopeField,
            String *global_name,
            LilyCheckedDataType *data_type,
            struct LilyMirScopeInstance *field_instance);

/**
 *
 * @brief Free LilyMirScopeField type.
 */
DESTRUCTOR(LilyMirScopeField, LilyMirScopeField *self);

enum LilyMirScopeInstanceKind
{
    LILY_MIR_SCOPE_INSTANCE_KIND_FIELD,
    LILY_MIR_SCOPE_INSTANCE_KIND_PARAM,
    LILY_MIR_SCOPE_INSTANCE_KIND_VAR
};

typedef struct LilyMirScopeInstance
{
    enum LilyMirScopeInstanceKind kind;
    Vec *fields; // Vec<LilyMirScopeField*>*
    union
    {
        LilyMirScopeField *field;
        const LilyMirScopeParam *param; // const LilyMirScopeParam* (&)
        const LilyMirScopeVar *var;     // const LilyMirScopeVar* (&)
    };
} LilyMirScopeInstance;

/**
 *
 * @brief Construct LilyMirScopeInstance type
 * (LILY_MIR_SCOPE_INSTANCE_KIND_FIELD).
 */
VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    field,
                    Vec *fields,
                    LilyMirScopeField *field);

/**
 *
 * @brief Construct LilyMirScopeInstance type
 * (LILY_MIR_SCOPE_INSTANCE_KIND_PARAM).
 */
VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    param,
                    Vec *fields,
                    const LilyMirScopeParam *param);

/**
 *
 * @brief Construct LilyMirScopeInstance type
 * (LILY_MIR_SCOPE_INSTANCE_KIND_VAR).
 */
VARIANT_CONSTRUCTOR(LilyMirScopeInstance *,
                    LilyMirScopeInstance,
                    var,
                    Vec *fields,
                    const LilyMirScopeVar *var);

/**
 *
 * @brief Free LilyMirScopeInstance type.
 */
DESTRUCTOR(LilyMirScopeInstance, LilyMirScopeInstance *self);

enum LilyMirScopeKind
{
    LILY_MIR_SCOPE_KIND_NORMAL,
    LILY_MIR_SCOPE_KIND_ROOT
};

typedef struct LilyMirScope
{
    enum LilyMirScopeKind kind;
    Vec *loads;                     // Vec<LilyMirInstructionFunLoad*>*
    Vec *params;                    // Vec<LilyMirScopeParam*>*?
    Vec *vars;                      // Vec<LilyMirScopeVar*>*
    Vec *instances;                 // Vec<LilyMirScopeInstance*>*
    const LilyMirBlockLimit *limit; // const LilyMirBlockLimit* (&)
    struct LilyMirScope *parent;    // LilyMirScope*? (&)
} LilyMirScope;

/**
 *
 * @brief Construct LilyMirScope type.
 */
CONSTRUCTOR(LilyMirScope *,
            LilyMirScope,
            enum LilyMirScopeKind kind,
            LilyMirBlockLimit *limit,
            LilyMirScope *parent);

/**
 *
 * @brief Free LilyMirScope type.
 */
DESTRUCTOR(LilyMirScope, LilyMirScope *self);

#endif // LILY_CORE_LILY_MIR_SCOPE_H
