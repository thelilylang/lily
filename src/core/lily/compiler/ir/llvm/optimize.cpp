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

#include <base/macros.h>

#include <core/lily/compiler/ir/llvm/optimize.h>

#include <llvm/ADT/Triple.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/IR/LegacyPassManager.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Target/TargetMachine.h>

#include <cstdlib>

using namespace llvm;

void
LilyLLVMOptimize(const LilyIrLlvm *self, enum LilyOptLevel lily_opt_level)
{
    auto &module = *unwrap(self->module);
    auto &machine = *reinterpret_cast<TargetMachine *>(self->machine);

    PipelineTuningOptions pipeline_opts;

    pipeline_opts.LoopUnrolling = !lily_opt_level;
    pipeline_opts.SLPVectorization = !lily_opt_level;
    pipeline_opts.LoopVectorization = !lily_opt_level;
    pipeline_opts.LoopInterleaving = !lily_opt_level;
    pipeline_opts.MergeFunctions = !lily_opt_level;

    //        PassInstrumentationCallbacks instr_callbacks;
    //        StandardInstrumentations std_instrumentations(module.getContext(),
    //        false); std_instrumentations.registerCallbacks(instr_callbacks);

    auto pb = PassBuilder(&machine, pipeline_opts);

    LoopAnalysisManager lam;
    FunctionAnalysisManager fam;
    ModuleAnalysisManager mam;
    CGSCCAnalysisManager cgam;

    Triple target_triple(module.getTargetTriple());

    pb.registerModuleAnalyses(mam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.registerCGSCCAnalyses(cgam);
    pb.crossRegisterProxies(lam, fam, cgam, mam);

    //    fam.registerPass([&] {
    //        return pb.buildDefaultAAPipeline();
    //    });

    ModulePassManager mpm;
    OptimizationLevel opt_level;

    switch (lily_opt_level) {
        case LILY_OPT_LEVEL_DEBUG:
            opt_level = OptimizationLevel::O0;
            break;
        case LILY_OPT_LEVEL_O1:
            opt_level = OptimizationLevel::O1;
            break;
        case LILY_OPT_LEVEL_O2:
            opt_level = OptimizationLevel::O2;
            break;
        case LILY_OPT_LEVEL_O3:
            opt_level = OptimizationLevel::O3;
            break;
        default:
            UNREACHABLE("invalid optimization level");
    }

    if (opt_level == OptimizationLevel::O0) {
        mpm = pb.buildO0DefaultPipeline(opt_level);
    } else {
        mpm = pb.buildPerModuleDefaultPipeline(opt_level);
    }

    legacy::PassManager cpm;

    // Run optimization
    mpm.run(module, mam);

    // Run codegen
    cpm.run(module);
}
