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

#ifndef LILY_CORE_CC_CI_RESULT_H
#define LILY_CORE_CC_CI_RESULT_H

#include <base/hash_map.h>
#include <base/macros.h>
#include <base/ordered_hash_map.h>

#include <core/cc/ci/ast.h>
#include <core/cc/ci/builtin.h>
#include <core/cc/ci/features.h>
#include <core/cc/ci/parser.h>
#include <core/cc/ci/scanner.h>
#include <core/cc/ci/token.h>
#include <core/shared/file.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct CIResultFile CIResultFile;
typedef struct CIResult CIResult;

typedef struct CIResultDefine
{
    const CITokenPreprocessorDefine
      *define; // const CITokenPreprocessorDefine* (&)
    Usize ref_count;
} CIResultDefine;

/**
 *
 * @brief Construct CIResultDefine type.
 */
CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define);

/**
 *
 * @brief Pass to ref a pointer of `CIResultDefine` and increment
 * the `ref_count`.
 * @return CIResultDefine* (&)
 */
inline CIResultDefine *
ref__CIResultDefine(CIResultDefine *self)
{
    ++self->ref_count;
    return self;
}

/**
 *
 * @brief Free CIResultDefine type.
 */
DESTRUCTOR(CIResultDefine, CIResultDefine *self);

enum CIResultIncludeKind
{
    CI_RESULT_INCLUDE_KIND_QUOTE,
    CI_RESULT_INCLUDE_KIND_ANGLE,
};

typedef struct CIResultInclude
{
    enum CIResultIncludeKind kind;
    const CIResultFile *path_ref; // const CIResultFile* (&)
    Usize repeat_count; // NOTE: This is used to count the number of times the
                        // same include is used in the same file.
} CIResultInclude;

/**
 *
 * @brief Construct CIResultInclude type.
 */
CONSTRUCTOR(CIResultInclude *,
            CIResultInclude,
            enum CIResultIncludeKind kind,
            const CIResultFile *path_ref);

/**
 *
 * @brief Increment the repeat count of the include.
 */
inline void
increment_repeat_count__CIResultInclude(CIResultInclude *self)
{
    ++self->repeat_count;
}

/**
 *
 * @brief Free CIResultInclude type.
 */
inline DESTRUCTOR(CIResultInclude, CIResultInclude *self)
{
    lily_free(self);
}

// This structure represents the organization of an *.hci or *.ci file.
typedef struct CIResultFile
{
    const CIResult *result; // const CIResult* (&)
    Usize id;
    bool kind : 1;
    String *filename_result;
    File file_input;
    HashMap *defines;    // HashMap<CIResultDefine*>*
    HashMap *includes;   // HashMap<CIResultInclude*>*
    CIScope *scope_base; // CIScope* (&)
    Vec *scopes;         // Vec<CIScope*>*
    Vec *enums;          // Vec<CIDecl*>*
    Vec *functions;      // Vec<CIDecl*>*
    Vec *structs;        // Vec<CIDecl*>*
    Vec *typedefs;       // Vec<CIDecl*>*
    Vec *unions;         // Vec<CIDecl*>*
    Vec *variables;      // Vec<CIDecl*>*
    Usize count_error;
    Usize count_warning;
    CIScanner scanner;
    CIParser parser;
} CIResultFile;

/**
 *
 * @brief Construct CIResultFile type.
 * @param builtin If the builtin parameter is NULL, this means that the
 * CIResultFile type is a builtin.
 * @param builtin CIResultFile*? (&)
 */
CONSTRUCTOR(CIResultFile *,
            CIResultFile,
            const CIResult *result,
            Usize id,
            bool kind,
            enum CIStandard standard,
            String *filename_result,
            File file_input,
            const CIResultFile *builtin);

/**
 *
 * @brief Get the next scope ID.
 */
inline Usize
get_next_scope_id__CIResultFile(const CIResultFile *self)
{
    return self->scopes->len;
}

/**
 *
 * @brief Add scope to scopes vector.
 */
CIScope *
add_scope__CIResultFile(const CIResultFile *self,
                        CIScopeID *parent,
                        bool is_block);

/**
 *
 * @brief Add define to defines field. If the define is already defined, the
 * result define pointer is rerturned, otherwise NULL is returned.
 * @return const CIResultDefine*?
 */
const CIResultDefine *
add_define__CIResultFile(const CIResultFile *self,
                         CIResultDefine *result_define);

/**
 *
 * @brief Search define.
 * @return const CIResultDefine*?
 */
const CIResultDefine *
get_define__CIResultFile(const CIResultFile *self, String *name);

/**
 *
 * @brief Search define from str.
 * @return const CIResultDefine*?
 */
const CIResultDefine *
get_define_from_str__CIResultFile(const CIResultFile *self, char *name);

/**
 *
 * @brief Undef define.
 * @example #undef <name>
 */
bool
undef_define__CIResultFile(const CIResultFile *self, String *name);

/**
 *
 * @brief Include builtin to file.
 */
void
include_builtin__CIResultFile(const CIResultFile *self,
                              const CIResultFile *builtin);

/**
 *
 * @brief Add include preprocessor to `includes` map and apply it.
 */
void
add_include__CIResultFile(const CIResultFile *self);

/**
 *
 * @brief Add enum declaration to enums field. If the enum name is already
 * defined, the declaration pointer is returned, otherwise NULL is returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_enum__CIResultFile(const CIResultFile *self, CIDecl *enum_);

/**
 *
 * @brief Add function declaration to functions field. If the function name is
 * already defined, the declaration pointer is returned, otherwise NULL is
 * returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function);

/**
 *
 * @brief Add struct declaration to structs field. If the struct name is already
 * defined, the declaration pointer is returned, otherwise NULL is returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_struct__CIResultFile(const CIResultFile *self, CIDecl *struct_);

/**
 *
 * @brief Add typedef declaration to typedefs field. If the typedef name is
 * already defined, the declaration pointer is returned, otherwise NULL is
 * returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_typedef__CIResultFile(const CIResultFile *self, CIDecl *typedef_);

/**
 *
 * @brief Add union declaration to unions field. If the union name is already
 * defined, the declaration pointer is returned, otherwise NULL is returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_union__CIResultFile(const CIResultFile *self, CIDecl *union_);

/**
 *
 * @brief Add variable declaration to variables field. If the variable name is
 * already defined, the declaration pointer is returned, otherwise NULL is
 * returned.
 * @return const CIDecl*?
 */
const CIDecl *
add_variable__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           CIDecl *variable);

/**
 *
 * @brief Get scope from id.
 */
CIScope *
get_scope_from_id__CIResultFile(const CIResultFile *self,
                                const CIScopeID *scope_id);

/**
 *
 * @brief Get enum declaration from id.
 */
CIDecl *
get_enum_from_id__CIResultFile(const CIResultFile *self,
                               const CIEnumID *enum_id);

/**
 *
 * @brief Get function declaration from id.
 */
CIDecl *
get_function_from_id__CIResultFile(const CIResultFile *self,
                                   const CIFunctionID *function_id);

/**
 *
 * @brief Get struct declaration from id.
 */
CIDecl *
get_struct_from_id__CIResultFile(const CIResultFile *self,
                                 const CIStructID *struct_id);

/**
 *
 * @brief Get typedef declaration from id.
 */
CIDecl *
get_typedef_from_id__CIResultFile(const CIResultFile *self,
                                  const CITypedefID *typedef_id);

/**
 *
 * @brief Get union declaration from id.
 */
CIDecl *
get_union_from_id__CIResultFile(const CIResultFile *self,
                                const CIUnionID *union_id);

/**
 *
 * @brief Get variable declaration from id.
 */
CIDecl *
get_variable_from_id__CIResultFile(const CIResultFile *self,
                                   const CIVariableID *variable_id);

/**
 *
 * @brief Search enum declaration in enums map.
 */
CIDecl *
search_enum__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Search function declaration in functions map.
 */
CIDecl *
search_function__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Search struct declaration in structs map.
 */
CIDecl *
search_struct__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Search union declaration in unions map.
 */
CIDecl *
search_union__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Search variable declaration in variables map.
 */
CIDecl *
search_variable__CIResultFile(const CIResultFile *self,
                              const CIScope *scope,
                              const String *name);

/**
 *
 * @brief Search typedef declaration in decls map.
 */
CIDecl *
search_typedef__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Search data type declaration in decls map.
 */
CIDecl *
search_data_type__CIResultFile(const CIResultFile *self, const String *name);

/**
 *
 * @brief Run the scanner, parser and the C generator.
 */
void
run__CIResultFile(CIResultFile *self);

/**
 *
 * @brief Free CIResultFile type.
 */
DESTRUCTOR(CIResultFile, CIResultFile *self);

typedef struct CIResult
{
    CIResultFile *builtin;
    OrderedHashMap *headers; // OrderedHashMap<CIResultFile*>*
    OrderedHashMap *sources; // OrderedHashMap<CIResultFile*>*
} CIResult;

/**
 *
 * @brief Construct CIResult type.
 */
inline CONSTRUCTOR(CIResult, CIResult)
{
    return (CIResult){
        .builtin = NULL,
        .headers = NEW(OrderedHashMap),
        .sources = NEW(OrderedHashMap),
    };
}

/**
 *
 * @brief Get file from FileID.
 */
const CIResultFile *
get_from_file_id__CIResult(const CIResult *self, const CIFileID *file_id);

/**
 *
 * @brief Get enum declaration from CIResult type.
 */
const CIDecl *
get_enum_from_id__CIResult(const CIResult *self, const CIEnumID *enum_id);

/**
 *
 * @brief Add a header to headers OrderedHashMap.
 */
CIResultFile *
add_header__CIResult(const CIResult *self,
                     enum CIStandard standard,
                     String *filename_result,
                     File file_input);

/**
 *
 * @brief Add a source to sources OrderedHashMap.
 */
CIResultFile *
add_source__CIResult(const CIResult *self,
                     enum CIStandard standard,
                     String *filename_result,
                     File file_input);

/**
 *
 * @brief Check if the header is already in the headers OrderedHashMap.
 */
inline bool
has_header__CIResult(const CIResult *self, const String *filename_result)
{
    return get__OrderedHashMap(self->headers, filename_result->buffer);
}

/**
 *
 * @brief Check if the source is already in the sources OrderedHashMap.
 */
inline bool
has_source__CIResult(const CIResult *self, const String *filename_result)
{
    return get__OrderedHashMap(self->sources, filename_result->buffer);
}

/**
 *
 * @brief Load builtin file.
 */
void
load_builtin__CIResult(CIResult *self, const CIConfig *config);

/**
 *
 * @brief Add & Run from the passed path.
 * @param path char*
 * @return If true, the file has been successfully added and ran; otherwise, it
 * returns false.
 */
bool
add_and_run__CIResult(const CIResult *self,
                      char *path,
                      enum CIStandard standard);

/**
 *
 * @brief Free CIResult type.
 */
DESTRUCTOR(CIResult, const CIResult *self);

#endif // LILY_CORE_CC_CI_RESULT_H
