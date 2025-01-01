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

#ifndef LILY_CORE_LILY_COMPILER_IR_LLVM_ATTR_H
#define LILY_CORE_LILY_COMPILER_IR_LLVM_ATTR_H

#include <base/types.h>

#include <core/lily/compiler/ir/llvm.h>

#define LLVM_ATTR(name, name_len, val) \
    return LLVMCreateEnumAttribute(    \
      self->context, LLVMGetEnumAttributeKindForName(name, name_len), val);

#define LLVM_ADD_FN_ATTR(fn, attr) \
    LLVMAddAttributeAtIndex(fn, LLVMAttributeFunctionIndex, attr);

#define ADD_CUSTOM_HOST_ATTR(fun)                                 \
    LLVMAttributeRef target_cpu = LLVMCreateStringAttribute(      \
      self->context, "target-cpu", 10, self->cpu, self->cpu_len); \
    LLVMAttributeRef target_features =                            \
      LLVMCreateStringAttribute(self->context,                    \
                                "target-features",                \
                                15,                               \
                                self->features,                   \
                                self->features_len);              \
                                                                  \
    LLVM_ADD_FN_ATTR(fun, target_cpu);                            \
    LLVM_ADD_FN_ATTR(fun, target_features);

/**
 *
 * @brief This indicates that the pointer value or vector of pointers has the
 * specified alignment. If applied to a vector of pointers, all pointers
 * (elements) have the specified alignment. If the pointer value does not have
 * the specified alignment, poison value is returned or passed instead. The
 * align attribute should be combined with the noundef attribute to ensure a
 * pointer is aligned, or otherwise the behavior is undefined. Note that align 1
 * has no effect on non-byval, non-preallocated arguments.
 * @note Note that this attribute has additional semantics when combined with
 * the byval or preallocated attribute, which are documented there.
 * @url https://llvm.org/docs/LangRef.html#attr-align
 */
inline LLVMAttributeRef
align_attr__LilyIrLlvm(const LilyIrLlvm *self, Uint64 align)
{
    LLVM_ATTR("align", 5, align);
}

/**
 *
 * @brief This attribute indicates that the inliner should attempt to inline
 * this function into callers whenever possible, ignoring any active inlining
 * size threshold for this caller.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
alwaysinline_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("alwaysinline", 12, 0);
}

/**
 *
 * @brief This indicates that the callee function at a call site should be
 * recognized as a built-in function, even though the function’s declaration
 * uses the nobuiltin attribute. This is only valid at call sites for direct
 * calls to functions that are declared with the nobuiltin attribute.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
builtin_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("builtin", 7, 0);
}

/**
 *
 * @brief This attribute indicates that this function is rarely called. When
 * computing edge weights, basic blocks post-dominated by a cold function call
 * are also considered to be cold; and, thus, given low weight.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
cold_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("cold", 5, 0);
}

/**
 *
 * @brief This attribute indicates that this function is a hot spot of the
 * program execution. The function will be optimized more aggressively and will
 * be placed into special subsection of the text section to improving
 * locality.When profile feedback is enabled, this attribute has the precedence
 * over the profile information. By marking a function hot, users can work
 * around the cases where the training input does not have good coverage on all
 * the hot functions.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
hot_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("hot", 3, 0);
}

/**
 *
 * @brief This indicates the parameter is required to be an immediate value.
 * This must be a trivial immediate integer or floating-point constant. Undef or
 * constant expressions are not valid. This is only valid on intrinsic
 * declarations and cannot be applied to a call site or arbitrary function.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
immarg_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("immarg", 6, 0);
}

/**
 *
 * @url https://llvm.org/docs/LangRef.html#noalias-and-alias-scope-metadata
 */
inline LLVMAttributeRef
noalias_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("noalias", 7, 0);
}

/**
 *
 * @brief This indicates that the callee function at a call site is not
 * recognized as a built-in function. LLVM will retain the original call and not
 * replace it with equivalent code based on the semantics of the built-in
 * function, unless the call site uses the builtin attribute. This is valid at
 * call sites and on function declarations and definitions.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
nobuiltin_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nobuiltin", 9, 0);
}

/**
 *
 * @brief This attribute indicates that the function is only allowed to jump
 * back into caller’s module by a return or an exception, and is not allowed to
 * jump back by invoking a callback function, a direct, possibly transitive,
 * external function call, use of longjmp, or other means. It is a compiler hint
 * that is used at module level to improve dataflow analysis, dropped during
 * linking, and has no effect on functions defined in the current module.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
nocallback_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nocallback", 10, 0);
}

/**
 *
 * @brief This indicates that callee does not free the pointer argument. This is
 * not a valid attribute for return values.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
nofree_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nofree", 6, 0);
}

/**
 *
 * @brief This attribute indicates that the inliner should never inline this
 * function in any situation. This attribute may not be used together with the
 * alwaysinline attribute.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
noinline_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("noinline", 8, 0);
}

/**
 *
 * @brief This indicates that the parameter or return pointer is not null. This
 * attribute may only be applied to pointer typed parameters. This is not
 * checked or enforced by LLVM; if the parameter or return pointer is null,
 * poison value is returned or passed instead. The nonnull attribute should be
 * combined with the noundef attribute to ensure a pointer is not null or
 * otherwise the behavior is undefined.
 * @url https://llvm.org/docs/LangRef.html#parameter-attributes
 */
inline LLVMAttributeRef
nonnull_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nonnull", 7, 0);
}

/**
 *
 * @brief This function attribute indicates that the function does not call
 * itself either directly or indirectly down any possible call path. This
 * produces undefined behavior at runtime if the function ever does recurse.
 * @url https://llvm.org/docs/LangRef.html#parameter-attributes
 */
inline LLVMAttributeRef
norecurse_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("norecurse", 9, 0);
}

/**
 *
 * @brief This function attribute indicates that the function never returns
 * normally, hence through a return instruction. This produces undefined
 * behavior at runtime if the function ever does dynamically return. Annotated
 * functions may still raise an exception, i.a., nounwind is not implied.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
noreturn_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("noreturn", 8, 0);
}

/**
 *
 * @brief This function attribute indicates that the function does not
 * communicate (synchronize) with another thread through memory or other
 * well-defined means. Synchronization is considered possible in the presence of
 * atomic accesses that enforce an order, thus not “unordered” and “monotonic”,
 * volatile accesses, as well as convergent function calls. Note that through
 * convergent function calls non-memory communication, e.g., cross-lane
 * operations, are possible and are also considered synchronization. However
 * convergent does not contradict nosync. If an annotated function does ever
 * synchronize with another thread, the behavior is undefined.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
nosync_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nosync", 6, 0);
}

/**
 *
 * @brief This function attribute indicates that the function never raises an
 * exception. If the function does raise an exception, its runtime behavior is
 * undefined. However, functions marked nounwind may still trap or generate
 * asynchronous exceptions. Exception handling schemes that are recognized by
 * LLVM to handle asynchronous exceptions, such as SEH, will still provide their
 * implementation defined semantics.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
nounwind_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("nounwind", 8, 0);
}

/**
 *
 * @brief This function attribute indicates that most optimization passes will
 * skip this function, with the exception of interprocedural optimization
 * passes. Code generation defaults to the “fast” instruction selector. This
 * attribute cannot be used together with the alwaysinline attribute; this
 * attribute is also incompatible with the minsize attribute and the optsize
 * attribute. This attribute requires the noinline attribute to be specified on
 * the function as well, so the function is never inlined into any caller. Only
 * functions with the alwaysinline attribute are valid candidates for inlining
 * into the body of this function.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
optnone_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("optnone", 7, 0);
}

/**
 *
 * @brief This attribute indicates that the function does not write through this
 * pointer argument, even though it may write to the memory that the pointer
 * points to. If a function writes to a readonly pointer argument, the behavior
 * is undefined.
 * @url https://llvm.org/docs/LangRef.html#parameter-attributes
 */
inline LLVMAttributeRef
readonly_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("readonly", 8, 0);
}

/**
 *
 * @brief This attribute indicates that the function does not dereference that
 * pointer argument, even though it may read or write the memory that the
 * pointer points to if accessed through other pointers. If a function reads
 * from or writes to a readnone pointer argument, the behavior is undefined.
 * @url https://llvm.org/docs/LangRef.html#parameter-attributes
 */
inline LLVMAttributeRef
readnone_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("readnone", 8, 0);
}

/**
 *
 * @brief This function attribute indicates that the function does not have any
 * effects besides calculating its result and does not have undefined behavior.
 * Note that speculatable is not enough to conclude that along any particular
 * execution path the number of calls to this function will not be externally
 * observable. This attribute is only valid on functions and declarations, not
 * on individual call sites. If a function is incorrectly marked as speculatable
 * and really does exhibit undefined behavior, the undefined behavior may be
 * observed even if the call site is dead code.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
speculatable_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("speculatable", 12, 0);
}

/**
 *
 * @brief This attribute indicates that the ABI being targeted requires that an
 * unwind table entry be produced for this function even if we can show that no
 * exceptions passes by it. This is normally the case for the ELF x86-64 abi,
 * but it can be disabled for some compilation units. The optional parameter
 * describes what kind of unwind tables to generate: sync for normal unwind
 * tables, async for asynchronous (instruction precise) unwind tables. Without
 * the parameter, the attribute uwtable is equivalent to uwtable(async).
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
uwtable_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("uwtable", 7, 0);
}

/**
 *
 * @brief This attribute indicates that the function may write to, but does not
 * read through this pointer argument (even though it may read from the memory
 * that the pointer points to). If a function reads from a writeonly pointer
 * argument, the behavior is undefined.
 * @url https://llvm.org/docs/LangRef.html#parameter-attributes
 */
inline LLVMAttributeRef
writeonly_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("writeonly", 9, 0);
}

/**
 *
 * @brief This function attribute indicates that a call of this function will
 * either exhibit undefined behavior or comes back and continues execution at a
 * point in the existing call stack that includes the current invocation.
 * Annotated functions may still raise an exception, i.a., nounwind is not
 * implied. If an invocation of an annotated function does not return control
 * back to a point in the call stack, the behavior is undefined.
 * @url https://llvm.org/docs/LangRef.html#function-attributes
 */
inline LLVMAttributeRef
willreturn_attr__LilyIrLlvm(const LilyIrLlvm *self)
{
    LLVM_ATTR("willreturn", 10, 0);
}

#endif // LILY_CORE_LILY_COMPILER_IR_LLVM_ATTR_H
