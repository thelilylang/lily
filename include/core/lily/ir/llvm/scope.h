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

#ifndef LILY_CORE_LILY_IR_LLVM_SCOPE_H
#define LILY_CORE_LILY_IR_LLVM_SCOPE_H

#include <base/alloc.h>
#include <base/string.h>
#include <base/vec.h>

#include <llvm-c/Core.h>

typedef struct LilyIrLlvm LilyIrLlvm;

typedef struct LilyLlvmValue
{
    String *name;       // String* (&)
    LLVMValueRef value; // LLVMValueRef (&)
} LilyLlvmValue;

/**
 *
 * @brief Construct LilyLlvmValue type.
 */
CONSTRUCTOR(LilyLlvmValue *, LilyLlvmValue, String *name, LLVMValueRef value);

/**
 *
 * @brief Free LilyLlvmValue type.
 */
inline DESTRUCTOR(LilyLlvmValue, LilyLlvmValue *self)
{
    lily_free(self);
}

typedef struct LilyLlvmType
{
    String *name;     // String* (&)
    LLVMTypeRef type; // LLVMTypeRef (&)
} LilyLlvmType;

/**
 *
 * @brief Construct LilyLlvmType type.
 */
CONSTRUCTOR(LilyLlvmType *, LilyLlvmType, String *name, LLVMTypeRef type);

/**
 *
 * @brief Free LilyLlvmType type.
 */
inline DESTRUCTOR(LilyLlvmType, LilyLlvmType *self)
{
    lily_free(self);
}

typedef struct LilyLlvmFun
{
    String *name; // String* (&)
    LLVMValueRef fun;
    LLVMTypeRef fun_type;
} LilyLlvmFun;

/**
 *
 * @brief Construct LilyLlvmFun type.
 */
CONSTRUCTOR(LilyLlvmFun *,
            LilyLlvmFun,
            String *name,
            LLVMValueRef fun,
            LLVMTypeRef fun_type);

/**
 *
 * @brief Free LilyLlvmType type.
 */
inline DESTRUCTOR(LilyLlvmFun, LilyLlvmFun *self)
{
    lily_free(self);
}

typedef struct LilyLlvmScope
{
    LLVMMetadataRef scope;
    Vec *values;                  // Vec<LilyLlvmValue*>*
    Vec *types;                   // Vec<LilyLlvmType*>*
    Vec *loads;                   // Vec<LilyLlvmValue*>*
    Vec *funs;                    // Vec<LilyLlvmFun*>*
    struct LilyLlvmScope *parent; // LilyLlvmScope*? (&)
} LilyLlvmScope;

/**
 *
 * @brief Construct LilyLlvmScope type.
 */
CONSTRUCTOR(LilyLlvmScope *, LilyLlvmScope, LilyLlvmScope *parent);

/**
 *
 * @brief Search for a value in the scope.
 */
LilyLlvmValue *
search_value__LilyLlvmScope(LilyLlvmScope *self, String *name);

/**
 *
 * @brief Search for a type in the scope.
 */
LilyLlvmType *
search_type__LilyLlvmScope(LilyLlvmScope *self, String *name);

/**
 *
 * @brief Search for a loaded value in the scope.
 */
LilyLlvmValue *
search_load__LilyLlvmScope(LilyLlvmScope *self, String *name);

/**
 *
 * @brief Search for a loaded value in the scope. If no load value is found
 * below the specified value.
 */
LLVMValueRef
load_value__LilyLlvmScope(LilyLlvmScope *self,
                          const LilyIrLlvm *llvm,
                          LLVMTypeRef type,
                          String *name);

/**
 *
 * @brief Search for a fun in the scope.
 */
LilyLlvmFun *
search_fun__LilyLlvmScope(LilyLlvmScope *self, String *name);

/**
 *
 * @brief Free LilyLlvmScope type.
 */
DESTRUCTOR(LilyLlvmScope, LilyLlvmScope *self);

#endif // LILY_CORE_LILY_IR_LLVM_SCOPE_H
