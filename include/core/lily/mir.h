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

#ifndef LILY_CORE_LILY_MIR_H
#define LILY_CORE_LILY_MIR_H

#include <base/stack.h>
#include <base/vec.h>

#include <core/lily/mir/instruction.h>

typedef struct LilyMirNameManager
{
    Vec *names; // Vec<char*>*
    Usize count;
    char *base_name;
} LilyMirNameManager;

/**
 *
 * @brief Construct LilyMirNameManager type.
 */
inline CONSTRUCTOR(LilyMirNameManager, LilyMirNameManager, char *base_name)
{
    return (LilyMirNameManager){ .names = NEW(Vec),
                                 .count = 0,
                                 .base_name = base_name };
}

/**
 *
 * @brief Free LilyMirNameManager type.
 */
DESTRUCTOR(LilyMirNameManager, const LilyMirNameManager *self);

enum LilyMirCurrentKind
{
    LILY_MIR_CURRENT_KIND_CONST,
    LILY_MIR_CURRENT_KIND_FUN,
    LILY_MIR_CURRENT_KIND_STRUCT,
    LILY_MIR_CURRENT_KIND_NULL
};

typedef struct LilyMirCurrentFun
{
    LilyMirInstructionFun *fun;
    LilyMirNameManager block_manager;
    LilyMirNameManager reg_manager;
} LilyMirCurrentFun;

/**
 *
 * @brief Construct LilyMirCurrentFun type.
 */
inline CONSTRUCTOR(LilyMirCurrentFun,
                   LilyMirCurrentFun,
                   LilyMirInstructionFun *fun)
{
    return (LilyMirCurrentFun){ .fun = fun,
                                .block_manager = NEW(LilyMirNameManager, "bb"),
                                .reg_manager = NEW(LilyMirNameManager, "") };
}

/**
 *
 * @brief Free LilyMirCurrentFun type.
 */
DESTRUCTOR(LilyMirCurrentFun, const LilyMirCurrentFun *self);

typedef struct LilyMirCurrent
{
    enum LilyMirCurrentKind kind;
    union
    {
        LilyMirInstructionConst *const_;
        LilyMirCurrentFun fun;
        LilyMirInstructionStruct *struct_;
    };
} LilyMirCurrent;

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_CONST).
 */
inline VARIANT_CONSTRUCTOR(LilyMirCurrent,
                           LilyMirCurrent,
                           const,
                           LilyMirInstructionConst *const_)
{
    return (LilyMirCurrent){ .kind = LILY_MIR_CURRENT_KIND_CONST,
                             .const_ = const_ };
}

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_FUN).
 */
inline VARIANT_CONSTRUCTOR(LilyMirCurrent,
                           LilyMirCurrent,
                           fun,
                           LilyMirCurrentFun fun)
{
    return (LilyMirCurrent){ .kind = LILY_MIR_CURRENT_KIND_FUN, .fun = fun };
}

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_STRUCT).
 */
inline VARIANT_CONSTRUCTOR(LilyMirCurrent,
                           LilyMirCurrent,
                           struct,
                           LilyMirInstructionStruct *struct_)
{
    return (LilyMirCurrent){ .kind = LILY_MIR_CURRENT_KIND_STRUCT,
                             .struct_ = struct_ };
}

/**
 *
 * @brief Construct LilyMirCurrent type (LILY_MIR_CURRENT_KIND_NULL).
 */
inline VARIANT_CONSTRUCTOR(LilyMirCurrent, LilyMirCurrent, null)
{
    return (LilyMirCurrent){ .kind = LILY_MIR_CURRENT_KIND_NULL };
}

/**
 *
 * @brief Free LilyMirCurrent type.
 */
DESTRUCTOR(LilyMirCurrent, const LilyMirCurrent *self);

typedef struct LilyMirModule
{
    Vec *insts; // Vec<LilyMirInstruction*>*
    LilyMirCurrent current;
} LilyMirModule;

/**
 *
 * @brief Create MIR module.
 */
inline LilyMirModule
LilyMirCreateModule()
{
    return (LilyMirModule){ .insts = NEW(Vec),
                            .current = NEW_VARIANT(LilyMirCurrent, null) };
}

/**
 *
 * @brief Add instruction to the module.
 */
void
LilyMirAddInst(LilyMirModule *Module, LilyMirInstruction *Inst);

/**
 *
 * @brief Reset the current field.
 */
void
LilyMirResetCurrent(LilyMirModule *Module);

void
LilyMirDisposeModule(const LilyMirModule *Module);

void
LilyMirPositionBuilderAtEnd(LilyMirModule *Module,
                            LilyMirInstructionBlock *Block);

LilyMirInstructionBlock *
LilyMirGetInsertBlock(LilyMirModule *Module);

void
LilyMirDeleteBasicBlock(LilyMirInstructionBlock *Block);

void
LilyMirDisposeBuilder(LilyMirModule *Module);

char *
LilyMirGenerateName(LilyMirNameManager *NameManager);

LilyMirInstruction *
LilyMirBuildRetUnit(LilyMirModule *Module);

LilyMirInstruction *
LilyMirBuildRet(LilyMirModule *Module);

LilyMirInstruction *
LilyMirBuildJmp(LilyMirModule *Module);

LilyMirInstructionVal *
LilyMirGetValFromInst(LilyMirInstruction *inst);

#endif // LILY_CORE_LILY_MIR_H
