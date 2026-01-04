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

#ifndef LILY_CORE_LILY_MIR_DEBUG_INFO_H
#define LILY_CORE_LILY_MIR_DEBUG_INFO_H

#include <base/new.h>
#include <base/string.h>
#include <base/vec.h>

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

/**
 *
 * @brief Construct LilyMirDebugInfoBlock type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoBlock,
                   LilyMirDebugInfoBlock,
                   const LilyMirDebugInfo *scope,
                   const LilyMirDebugInfoFile *file,
                   Usize line,
                   Usize column)
{
    return (LilyMirDebugInfoBlock){
        .scope = scope, .file = file, .line = line, .column = column
    };
}

typedef struct LilyMirDebugInfoLocation
{
    const LilyMirDebugInfo *scope; // const LilyMirDebugInfo* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoLocation;

/**
 *
 * @brief Construct LilyMirDebugInfoLocation type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoLocation,
                   LilyMirDebugInfoLocation,
                   const LilyMirDebugInfo *scope,
                   Usize line,
                   Usize column)
{
    return (LilyMirDebugInfoLocation){ .scope = scope,
                                       .line = line,
                                       .column = column };
}

typedef struct LilyMirDebugInfoSubProgram
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoSubProgram;

/**
 *
 * @brief Construct LilyMirDebugInfoSubProgram type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoSubProgram,
                   LilyMirDebugInfoSubProgram,
                   const LilyMirDebugInfo *scope,
                   const LilyMirDebugInfoFile *file,
                   Usize line,
                   Usize column)
{
    return (LilyMirDebugInfoSubProgram){
        .scope = scope, .file = file, .line = line, .column = column
    };
}

typedef struct LilyMirDebugInfoEnumerator
{
    const char *name;
    Usize value;
    bool is_unsigned;
} LilyMirDebugInfoEnumerator;

/**
 *
 * @brief Construct LilyMirDebugInfoEnumerator type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoEnumerator,
                   LilyMirDebugInfoEnumerator,
                   const char *name,
                   Usize value,
                   bool is_unsigned)
{
    return (LilyMirDebugInfoEnumerator){ .name = name,
                                         .value = value,
                                         .is_unsigned = is_unsigned };
}

typedef struct LilyMirDebugInfoGlobalVariable
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    const char *name;
    const char *linkage_name;
    bool is_local;
    bool is_definition;
} LilyMirDebugInfoGlobalVariable;

/**
 *
 * @brief Construct LilyMirDebugInfoGlobalVariable type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoGlobalVariable,
                   LilyMirDebugInfoGlobalVariable,
                   const LilyMirDebugInfo *scope,
                   const LilyMirDebugInfoFile *file,
                   const char *name,
                   const char *linkage_name,
                   bool is_local,
                   bool is_definition)
{
    return (LilyMirDebugInfoGlobalVariable){ .scope = scope,
                                             .file = file,
                                             .name = name,
                                             .linkage_name = linkage_name,
                                             .is_local = is_local,
                                             .is_definition = is_definition };
}

typedef struct LilyMirDebugInfoLocalVariable
{
    const LilyMirDebugInfo *scope;    // const LilyMirDebugInfo* (&)
    const LilyMirDebugInfoFile *file; // const LilyMirDebugInfoFile* (&)
    const LilyMirDebugInfo *type;     // const LilyMirDebugInfo* (&)
    const char *name;
    Usize arg_count;
    Usize line;
} LilyMirDebugInfoLocalVariable;

/**
 *
 * @brief Construct LilyMirDebugInfoLocalVariable type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoLocalVariable,
                   LilyMirDebugInfoLocalVariable,
                   const LilyMirDebugInfo *scope,
                   const LilyMirDebugInfoFile *file,
                   const LilyMirDebugInfo *type,
                   const char *name,
                   Usize arg_count,
                   Usize line)
{
    return (LilyMirDebugInfoLocalVariable){ .scope = scope,
                                            .file = file,
                                            .type = type,
                                            .name = name,
                                            .arg_count = arg_count,
                                            .line = line };
}

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

/**
 *
 * @brief Construct LilyMirDebugInfoType.
 */
inline CONSTRUCTOR(LilyMirDebugInfoType,
                   LilyMirDebugInfoType,
                   const char *name,
                   Usize size,
                   enum LilyMirDebugInfoEncoding encoding)
{
    return (
      LilyMirDebugInfoType){ .name = name, .size = size, .encoding = encoding };
}

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

/**
 *
 * @brief Construct LilyMirDebugInfoDerivedType.
 */
inline CONSTRUCTOR(LilyMirDebugInfoDerivedType,
                   LilyMirDebugInfoDerivedType,
                   const LilyMirDebugInfo *scope,
                   const LilyMirDebugInfo *base_type,
                   enum LilyMirDebugInfoTag tag,
                   const char *name,
                   Usize size,
                   Usize align,
                   Usize offset)
{
    return (LilyMirDebugInfoDerivedType){ .scope = scope,
                                          .base_type = base_type,
                                          .tag = tag,
                                          .name = name,
                                          .size = size,
                                          .align = align,
                                          .offset = offset };
}

typedef struct LilyMirDebugInfoCompositeType
{
    enum LilyMirDebugInfoTag tag;
    const char *name;
    Usize size;
    Usize align;
    const LilyMirDebugInfo *elements; // const LilyMirDebugInfo* (&)
} LilyMirDebugInfoCompositeType;

/**
 *
 * @brief Construct LilyMirDebugInfoCompositeType type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoCompositeType,
                   LilyMirDebugInfoCompositeType,
                   enum LilyMirDebugInfoTag tag,
                   const char *name,
                   Usize size,
                   Usize align,
                   const LilyMirDebugInfo *elements)
{
    return (LilyMirDebugInfoCompositeType){ .tag = tag,
                                            .name = name,
                                            .size = size,
                                            .align = align,
                                            .elements = elements };
}

typedef struct LilyMirDebugInfoElements
{
    Vec *items; // Vec<LilyMirDebugInfo* (&)>*
} LilyMirDebugInfoElements;

/**
 *
 * @brief Construct LilyMirDebugInfoElements type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoElements,
                   LilyMirDebugInfoElements,
                   Vec *items)
{
    return (LilyMirDebugInfoElements){ .items = items };
}

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
        LilyMirDebugInfoDerivedType derived_type;
        LilyMirDebugInfoCompositeType comp_type;
        LilyMirDebugInfoElements elements;
    };
} LilyMirDebugInfo;

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_FILE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    file,
                    Usize id,
                    LilyMirDebugInfoFile file);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_BLOCK).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    block,
                    Usize id,
                    LilyMirDebugInfoBlock block);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_LOCATION).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    location,
                    Usize id,
                    LilyMirDebugInfoLocation location);

/**
 *
 * @brief Construct LilyMirDebugInfo type
 * (LILY_MIR_DEBUG_INFO_KIND_SUB_PROGRAM).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    sub_program,
                    Usize id,
                    LilyMirDebugInfoSubProgram sub_program);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_ENUMERATOR).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    enumerator,
                    Usize id,
                    LilyMirDebugInfoEnumerator enumerator);

/**
 *
 * @brief Construct LilyMirDebugInfo type
 * (LILY_MIR_DEBUG_INFO_KIND_GLOBAL_VARIABLE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    global_variable,
                    Usize id,
                    LilyMirDebugInfoGlobalVariable global_variable);

/**
 *
 * @brief Construct LilyMirDebugInfo type
 * (LILY_MIR_DEBUG_INFO_KIND_LOCAL_VARIABLE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    local_variable,
                    Usize id,
                    LilyMirDebugInfoLocalVariable local_variable);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_EXPRESSION).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    expression,
                    Usize id,
                    LilyMirDebugInfo *expression);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_TYPE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    type,
                    Usize id,
                    LilyMirDebugInfoType type);

/**
 *
 * @brief Construct LilyMirDebugInfo type
 * (LILY_MIR_DEBUG_INFO_KIND_DERIVED_TYPE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    derived_type,
                    Usize id,
                    LilyMirDebugInfoDerivedType derived_type);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_COMP_TYPE).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    comp_type,
                    Usize id,
                    LilyMirDebugInfoCompositeType comp_type);

/**
 *
 * @brief Construct LilyMirDebugInfo type (LILY_MIR_DEBUG_INFO_KIND_ELEMENTS).
 */
VARIANT_CONSTRUCTOR(LilyMirDebugInfo *,
                    LilyMirDebugInfo,
                    elements,
                    Usize id,
                    LilyMirDebugInfoElements elements);

/**
 *
 * @brief Check if the debug info is equal.
 */
bool
eq__LilyMirDebugInfo(const LilyMirDebugInfo *self,
                     const LilyMirDebugInfo *other);

/**
 *
 * @brief Free LilyMirDebugInfo type.
 */
DESTRUCTOR(LilyMirDebugInfo, LilyMirDebugInfo *self);

typedef struct LilyMirDebugInfoManager
{
    Vec *debug_infos; // Vec<LilyMirDebugInfo*>*
    Usize count;
} LilyMirDebugInfoManager;

/**
 *
 * @brief Construct LilyMirDebugInfoManager type.
 */
inline CONSTRUCTOR(LilyMirDebugInfoManager, LilyMirDebugInfoManager)
{
    return (LilyMirDebugInfoManager){ .debug_infos = NEW(Vec) };
}

/**
 *
 * @brief Add debug info to `debug_infos`.
 */
LilyMirDebugInfo *
add__LilyMirDebugInfoManager(LilyMirDebugInfoManager *self,
                             LilyMirDebugInfo *debug_info);

/**
 *
 * @brief Free LilyMirDebugInfoManager type.
 */
DESTRUCTOR(LilyMirDebugInfoManager, const LilyMirDebugInfoManager *self);

#endif // LILY_CORE_LILY_MIR_DEBUG_INFO_H
