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

#include <base/format.h>

#include <core/lily/mir.h>

DESTRUCTOR(LilyMirNameManager, const LilyMirNameManager *self)
{
    for (Usize i = 0; i < self->names->len; ++i) {
        lily_free(get__Vec(self->names, i));
    }

    FREE(Vec, self->names);
}

DESTRUCTOR(LilyMirCurrentFun, const LilyMirCurrentFun *self)
{
    FREE(LilyMirNameManager, &self->block_manager);
    FREE(LilyMirNameManager, &self->reg_manager);
}

void
LilyMirAddInst(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    switch (Module->current.kind) {
        case LILY_MIR_CURRENT_KIND_NULL:
            switch (Inst->kind) {
                case LILY_MIR_INSTRUCTION_KIND_CONST:
                    Module->current =
                      NEW_VARIANT(LilyMirCurrent, const, &Inst->const_);
                    break;
                case LILY_MIR_INSTRUCTION_KIND_FUN:
                    Module->current = NEW_VARIANT(
                      LilyMirCurrent, fun, NEW(LilyMirCurrentFun, &Inst->fun));
                    break;
                case LILY_MIR_INSTRUCTION_KIND_STRUCT:
                    Module->current =
                      NEW_VARIANT(LilyMirCurrent, struct, &Inst->struct_);
                    break;
                default:
                    break;
            }

            push__Vec(Module->insts, Inst);

            break;
        case LILY_MIR_CURRENT_KIND_FUN: {
            LilyMirInstructionBlock *block =
              Module->current.fun.fun->block_stack->top;

            ASSERT(block);

            push__Vec(block->insts, Inst);

            break;
        }
        default:
            push__Vec(Module->insts, Inst);

            break;
    }
}

void
LilyMirResetCurrent(LilyMirModule *Module)
{
    FREE(LilyMirCurrent, &Module->current);
    Module->current = NEW_VARIANT(LilyMirCurrent, null);
}

LilyMirInstruction *
LilyMirBuildReg(LilyMirModule *Module, LilyMirInstruction *Inst)
{
    char *name = LilyMirGenerateName(&Module->current.fun.reg_manager);

    return NEW_VARIANT(LilyMirInstruction,
                       reg,
                       NEW(LilyMirInstructionReg, from__String(name), Inst));
}

DESTRUCTOR(LilyMirCurrent, const LilyMirCurrent *self)
{
    switch (self->kind) {
        case LILY_MIR_CURRENT_KIND_FUN:
            FREE(LilyMirCurrentFun, &self->fun);
            break;
        default:
            break;
    }
}

void
LilyMirDisposeModule(const LilyMirModule *Module)
{
    FREE_BUFFER_ITEMS(
      Module->insts->buffer, Module->insts->len, LilyMirInstruction);
    FREE(Vec, Module->insts);
    FREE(LilyMirCurrent, &Module->current);
}

void
LilyMirAddBlock(LilyMirModule *Module, LilyMirInstruction *Block)
{
    ASSERT(Module->current.kind == LILY_MIR_CURRENT_KIND_FUN);

    push__Vec(Module->current.fun.fun->insts, Block);
    push__Stack(Module->current.fun.fun->block_stack, &Block->block);
}

LilyMirInstructionVal *
LilyMirGetValFromInst(LilyMirInstruction *inst)
{
    switch (inst->kind) {
        case LILY_MIR_INSTRUCTION_KIND_VAL:
            return inst->val;
        default:
            return NULL;
    }
}

char *
LilyMirGenerateName(LilyMirNameManager *NameManager)
{
    char *name = format("{s}{d}", NameManager->base_name, NameManager->count);

    push__Vec(NameManager->names, name);

    ++NameManager->count;

    return name;
}
