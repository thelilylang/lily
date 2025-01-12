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

#include <base/macros.h>

#include <core/lily/compiler/ir/llvm/optimize.h>

#include <llvm/Analysis/AliasAnalysis.h>
#include <llvm/Analysis/LoopAnalysisManager.h>
#include <llvm/Analysis/TargetTransformInfo.h>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Passes/StandardInstrumentations.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/TargetParser/Triple.h>
#include <llvm/Transforms/Utils/AddDiscriminators.h>

#include <llvm-c/DebugInfo.h>

#include <cstdlib>

using namespace llvm;

int
LilyLLVMOptimize(const LilyIrLlvm *self,
                 enum LilyOptLevel lily_opt_level,
                 char **error_msg,
                 const char *filename)
{
    // About PassManager: https://llvm.org/docs/NewPassManager.html
    LLVMDIBuilderFinalize(self->di_builder);

    auto &module = *unwrap(self->module);
    auto &machine = *reinterpret_cast<TargetMachine *>(self->machine);
    auto &context = *unwrap(self->context);

    PipelineTuningOptions pipeline_opts;

    pipeline_opts.LoopUnrolling = !lily_opt_level;
    pipeline_opts.SLPVectorization = !lily_opt_level;
    pipeline_opts.LoopVectorization = !lily_opt_level;
    pipeline_opts.LoopInterleaving = !lily_opt_level;
    pipeline_opts.MergeFunctions = !lily_opt_level;

    PassInstrumentationCallbacks instr_callbacks;
    StandardInstrumentations std_instrumentations(context, false);
    std_instrumentations.registerCallbacks(instr_callbacks);

    std::optional<PGOOptions> opt_pgo_options = {};
    auto pb =
      PassBuilder(&machine, pipeline_opts, opt_pgo_options, &instr_callbacks);

    LoopAnalysisManager lam;
    FunctionAnalysisManager fam;
    ModuleAnalysisManager mam;
    CGSCCAnalysisManager cgam;

    fam.registerPass([&] { return pb.buildDefaultAAPipeline(); });

    Triple target_triple(module.getTargetTriple());
    auto tlii = std::make_unique<TargetLibraryInfoImpl>(target_triple);

    fam.registerPass([&] { return TargetLibraryAnalysis(*tlii); });

    pb.registerModuleAnalyses(mam);
    pb.registerFunctionAnalyses(fam);
    pb.registerLoopAnalyses(lam);
    pb.registerCGSCCAnalyses(cgam);
    pb.crossRegisterProxies(lam, fam, cgam, mam);

    if (!lily_opt_level) {
        pb.registerPipelineStartEPCallback(
          [](ModulePassManager &mpm, OptimizationLevel opt_level) {
              mpm.addPass(
                createModuleToFunctionPassAdaptor(AddDiscriminatorsPass()));
          });
    }

    OptimizationLevel opt_level;

    switch (lily_opt_level) {
        case LILY_OPT_LEVEL_O0:
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

    ModulePassManager mpm = opt_level == OptimizationLevel::O0
                              ? pb.buildO0DefaultPipeline(opt_level)
                              : pb.buildPerModuleDefaultPipeline(opt_level);

    // Run optimization
    mpm.run(module, mam);

    return 0;
}
