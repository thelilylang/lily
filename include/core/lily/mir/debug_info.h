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

#ifndef LILY_CORE_LILY_MIR_DEBUG_INFO_H
#define LILY_CORE_LILY_MIR_DEBUG_INFO_H

#include <base/string.h>

typedef struct LilyMirDebugInfo LilyMirDebugInfo;

typedef struct LilyMirDebugInfoFile
{
    String *filename;
    String *directory;
} LilyMirDebugInfoFile;

/**
 *
 * @brief Construct LilyMirDebugInfoFile type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoFile,
                   LilyMirDebugInfoFile,
                   String *filename,
                   String *directory)
{
    return (LilyMirDebugInfoFile){ .filename = filename,
                                   .directory = directory };
}

/**
 *
 * @brief Free LilyMirDebugInfoFile type.
 */
DESTRUCTOR(LilyMirDebugInfoFile, const LilyMirDebugInfoFile *self);

typedef struct LilyMirDebugInfoBlock
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoBlock;

typedef struct LilyMirDebugInfoLocation
{
    const LilyMirDebugInfo *scope; // const LilyMirDebugInfo* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoLocation;

typedef struct LilyMirDebugInfoSubProgram
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoSubProgram;

typedef struct LilyMirDebugInfoEnumerator
{
    const char *name;
    Usize value;
    bool is_unsigned;
} LilyMirDebugInfoEnumerator;

typedef struct LilyMirDebugInfoGlobalVariable
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    const char *name;
    const char *linkage_name;
    bool is_local;
    bool is_definition;
} LilyMirDebugInfoGlobalVariable;

typedef struct LilyMirDebugInfoLocalVariable
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    const LilyMirDebugInfo *type;     // const LilyMirDebugInfo* (&)
    const char *name;
    Usize arg_count;
    Usize line;
} LilyMirDebugInfoLocalVariable;

enum LilyMirDebugInfoEncoding
{
    LILY_MIR_DEBUG_INFO_ENCODING_ADDR,
    LILY_MIR_DEBUG_INFO_ENCODING_BOOL,
    LILY_MIR_DEBUG_INFO_ENCODING_CMPX_FLOAT,
    LILY_MIR_DEBUG_INFO_ENCODING_FLOAT,
    LILY_MIR_DEBUG_INFO_ENCODING_SIGNED,
    LILY_MIR_DEBUG_INFO_ENCODING_UNSIGNED,
    LILY_MIR_DEBUG_INFO_ENCODING_SIGNED_CHAR,
    LILY_MIR_DEBUG_INFO_ENCODING_UNSIGNED_CHAR,
    LILY_MIR_DEBUG_INFO_ENCODING_VOID
};

typedef struct LilyMirDebugInfoType
{
    const char *name;
    Usize size;
    enum LilyMirDebugInfoEncoding encoding;
} LilyMirDebugInfoType;

enum LilyMirDebugInfoTag
{
    LILY_MIR_DEBUG_INFO_TAG_MEMBER,
    LILY_MIR_DEBUG_INFO_TAG_STRUCTURE_TYPE
};

typedef struct LilyMirDebugInfoDerivedType
{
    const LilyMirDebugInfo *scope;     // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfo *base_type; // const LilyMirDebugInfo* (&)
    enum LilyMirDebugInfoTag tag;
    const char *name;
    Usize size;
    Usize align;
    Usize offset;
} LilyMirDebugInfoDerivedType;

typedef struct LilyMirDebugInfoCompositeType
{
    enum LilyMirDebugInfoTag tag;
    const char *name;
    Usize size;
    Usize align;
    const LilyMirDebugInfo *elements; // const LilyMirDebugInfo* (&)
} LilyMirDebugInfoCompositeType;

typedef struct LilyMirDebugInfoElements
{
    Vec *items; // Vec<LilyMirDebugInfo* (&)>*
} LilyMirDebugInfoElements;

enum LilyMirDebugInfoKind
{
    LILY_MIR_DEBUG_INFO_KIND_FILE,
    LILY_MIR_DEBUG_INFO_KIND_BLOCK,
    LILY_MIR_DEBUG_INFO_KIND_LOCATION,
    LILY_MIR_DEBUG_INFO_KIND_SUB_PROGRAM,
    LILY_MIR_DEBUG_INFO_KIND_ENUMERATOR,
    LILY_MIR_DEBUG_INFO_KIND_GLOBAL_VARIABLE,
    LILY_MIR_DEBUG_INFO_KIND_LOCAL_VARIABLE,
    LILY_MIR_DEBUG_INFO_KIND_EXPRESSION,
    LILY_MIR_DEBUG_INFO_KIND_TYPE,
    LILY_MIR_DEBUG_INFO_KIND_DERIVED_TYPE,
    LILY_MIR_DEBUG_INFO_KIND_COMP_TYPE,
    LILY_MIR_DEBUG_INFO_KIND_ELEMENTS,
};

typedef struct LilyMirDebugInfo
{
    enum LilyMirDebugInfoKind kind;
    Usize id;
    union
    {
        LilyMirDebugInfoFile file;
        LilyMirDebugInfoBlock block;
        LilyMirDebugInfoLocation location;
        LilyMirDebugInfoSubProgram sub_program;
        LilyMirDebugInfoEnumerator enumerator;
        LilyMirDebugInfoGlobalVariable global_variable;
        LilyMirDebugInfoLocalVariable local_variable;
        LilyMirDebugInfo *expression;
        LilyMirDebugInfoType type;
        LilyMirDebugInfoDerivedType dervied_type;
        LilyMirDebugInfoCompositeType composite_type;
        LilyMirDebugInfoElements elements;
    };
} LilyMirDebugInfo;

#endif // LILY_CORE_LILY_MIR_DEBUG_INFO_H
