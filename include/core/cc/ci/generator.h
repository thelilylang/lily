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

#ifndef LILY_CORE_CC_CI_GENERATOR_H
#define LILY_CORE_CC_CI_GENERATOR_H

#include <base/vec_bit.h>

#include <core/cc/ci/result.h>

typedef struct CIGeneratorContentSession
{
    String *buffer;
    bool must_inherit;
    struct
    {
        CIGenericParams *current_generic_params;        // CIGenericParams*? (&)
        CIGenericParams *current_called_generic_params; // CIGenericParams*? (&)
        Usize tab_count;
        bool write_semicolon;
        CIScope *current_scope; // CIScope* (&)
    } inherit_props;
} CIGeneratorContentSession;

/**
 *
 * @brief Construct CIGeneratorContentSession type.
 */
CONSTRUCTOR(CIGeneratorContentSession *,
            CIGeneratorContentSession,
            CIScope *current_scope);

/**
 *
 * @brief Free CIGeneratorContentSession type.
 */
DESTRUCTOR(CIGeneratorContentSession, CIGeneratorContentSession *self);

typedef struct CIGeneratorContent
{
    String *final;
    Vec *sessions;                           // Vec<CIGeneratorContentSession*>*
    CIGeneratorContentSession *last_session; // CIGeneratorContentSession*? (&)
} CIGeneratorContent;

/**
 *
 * @brief Construct CIGeneratorContent type.
 */
inline CONSTRUCTOR(CIGeneratorContent, CIGeneratorContent)
{
    return (CIGeneratorContent){ .final = NEW(String),
                                 .sessions = NEW(Vec),
                                 .last_session = NULL };
}

/**
 *
 * @brief Free CIGeneratorContent type.
 */
DESTRUCTOR(CIGeneratorContent, const CIGeneratorContent *self);

typedef struct CIGenerator
{
    const CIResultFile *file; // const CIResultFile* (&)
    CIGeneratorContent content;
    VecBit *generated_decls;
} CIGenerator;

/**
 *
 * @brief Construct CIGenerator type.
 */
inline CONSTRUCTOR(CIGenerator, CIGenerator, const CIResultFile *file)
{
    return (CIGenerator){ .file = file,
                          .content = NEW(CIGeneratorContent),
                          .generated_decls = NEW(VecBit) };
}

/**
 *
 * @brief Run CI generator (transpilation).
 */
void
run__CIGenerator(const CIResult *result);

/**
 *
 * @brief Free CIGenerator type.
 */
DESTRUCTOR(CIGenerator, const CIGenerator *self);

#endif // LILY_CORE_CC_CI_GENERATOR_H
