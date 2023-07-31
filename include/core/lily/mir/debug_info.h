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

typedef struct LilyMirDebugInfoBlock
{
    LilyMirDebugInfo *scope;    // LilyMirDebugInfo* (&)
    LilyMirDebugInfoFile *file; // LilyMirDebugInfoFile* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoBlock;

typedef struct LilyMirDebugInfoLocation
{
    LilyMirDebugInfo *scope; // LilyMirDebugInfo* (&)
    Usize line;
    Usize column;
} LilyMirDebugInfoLocation;

typedef struct LilyMirDebugInfoSubProgram
{
    LilyMirDebugInfo *scope;    // LilyMirDebugInfo* (&)
    LilyMirDebugInfoFile *file; // LilyMirDebugInfoFile* (&)
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
} LilyMirDebugInfoGlobalVariable;

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
    };
} LilyMirDebugInfo;

#endif // LILY_CORE_LILY_MIR_DEBUG_INFO_H
