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
#include <core/cc/ci/predefined.h>
#include <core/cc/ci/scanner.h>
#include <core/cc/ci/token.h>
#include <core/shared/file.h>

#include <stdio.h>
#include <stdlib.h>

typedef struct CIResultFileAnalysis CIResultFileAnalysis;
typedef struct CIResultFile CIResultFile;
typedef struct CIResult CIResult;

typedef struct CIResultDefine
{
    const CITokenPreprocessorDefine
      *define; // const CITokenPreprocessorDefine* (&)
    CIFileID file_id;
    Usize ref_count;
} CIResultDefine;

/**
 *
 * @brief Construct CIResultDefine type.
 */
CONSTRUCTOR(CIResultDefine *,
            CIResultDefine,
            const CITokenPreprocessorDefine *define,
            CIFileID file_id);

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

enum CIResultEntityKind
{
    CI_RESULT_ENTITY_KIND_BIN,
    CI_RESULT_ENTITY_KIND_FILE,
    CI_RESULT_ENTITY_KIND_LIB,
};

typedef struct CIResultEntity
{
    Usize id;
    const CIResult *result;  // const CIResult* (&)
    Vec *enums;              // Vec<CIDecl*>*
    Vec *functions;          // Vec<CIDecl*>*
    Vec *labels;             // Vec<CIDecl*>*
    Vec *structs;            // Vec<CIDecl*>*
    Vec *typedefs;           // Vec<CIDecl*>*
    Vec *unions;             // Vec<CIDecl*>*
    Vec *variables;          // Vec<CIDecl*>*
    Vec *decls;              // Vec<CIDecl*>*
    String *filename_result; // String*?
    enum CIResultEntityKind kind;
} CIResultEntity;

/**
 *
 * @brief Construct CIResultEntity type.
 */
inline CONSTRUCTOR(CIResultEntity,
                   CIResultEntity,
                   Usize id,
                   const CIResult *result,
                   enum CIResultEntityKind kind,
                   String *filename_result)
{
    return (CIResultEntity){ .id = id,
                             .result = result,
                             .enums = NEW(Vec),
                             .functions = NEW(Vec),
                             .labels = NEW(Vec),
                             .structs = NEW(Vec),
                             .typedefs = NEW(Vec),
                             .unions = NEW(Vec),
                             .variables = NEW(Vec),
                             .decls = NEW(Vec),
                             .filename_result = filename_result,
                             .kind = kind };
}

/**
 *
 * @brief Reset `CIResultEntity` type.
 * @example empties all vectors such as: enums, functions, ...
 */
void
reset__CIResultEntity(CIResultEntity *self);

/**
 *
 * @brief Free CIResultEntity type.
 */
DESTRUCTOR(CIResultEntity, const CIResultEntity *self);

// This structure represents the organization of an *.hci or *.ci file.
typedef struct CIResultFile
{
    File file_input;
    bool kind : 1;
    CIResultFileAnalysis *file_analysis; // CIResultFileAnalysis*?
    CIScope *scope_base;                 // CIScope*? (&)
    CIResultFile *owner;                 // CIResultFile*? (&)
    const CIProjectConfig *config;       // const CIProjectConfig* (&)
    CIResultEntity entity;
    CIScanner scanner;
} CIResultFile;

// This structure represents the file being analysis.
typedef struct CIResultFileAnalysis
{
    CIResultEntity *entity; // CIResultEntity* (&)
    CIScope *scope_base;    // CIScope* (&)
    Vec *scopes;            // Vec<CIScope*>*
    HashMap *defines;       // HashMap<CIResultDefine*>*
    HashMap *includes;      // HashMap<CIResultInclude*>*
    Usize count_error;
    Usize count_warning;
    CIParser parser;
} CIResultFileAnalysis;

/**
 *
 * @brief Construct CIResultFileAnalysis type.
 */
CONSTRUCTOR(CIResultFileAnalysis *, CIResultFileAnalysis, CIResultFile *file);

/**
 *
 * @brief Construct CIResultFileANalysis type.
 */
DESTRUCTOR(CIResultFileAnalysis, CIResultFileAnalysis *self);

/**
 *
 * @brief Construct CIResultFile type.
 * @param predefined If the predefined parameter is NULL, this means that the
 * CIResultFile type is a pre-defined.
 * @param predefined CIResultFile*? (&)
 */
CONSTRUCTOR(CIResultFile *,
            CIResultFile,
            File file_input,
            bool kind,
            CIResultFile *owner,
            const CIProjectConfig *config,
            Usize id,
            const CIResult *result,
            enum CIResultEntityKind entity_kind,
            String *filename_result);

/**
 *
 * @brief Set `file_analysis` field.
 */
void
set_file_analysis__CIResultFile(CIResultFile *self,
                                CIResultFileAnalysis *file_analysis);

/**
 *
 * @brief Unset & Destroy `file_analysis` field and associated assignment such
 * as `self->scanner.base.count_error`.
 */
void
destroy_file_analysis__CIResultFile(CIResultFile *self);

/**
 *
 * @brief Get the next scope ID.
 */
inline Usize
get_next_scope_id__CIResultFile(const CIResultFile *self)
{
    return self->file_analysis->scopes->len;
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
 * @brief Include predefined to file.
 */
void
include_predefined__CIResultFile(const CIResultFile *self,
                                 const CIResultFile *predefined);

/**
 *
 * @brief Add include preprocessor to `includes` map and apply it.
 */
void
add_include__CIResultFile(const CIResultFile *self);

/**
 *
 * @brief Add enum declaration to enums field. If the enumeration name is
 * already declared, the enumeration found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_enum__CIResultFile(const CIResultFile *self, CIDecl *enum_);

/**
 *
 * @brief Add function declaration to functions field. If the function name is
 * already declared, the function found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_function__CIResultFile(const CIResultFile *self, CIDecl *function);

/**
 *
 * @brief Add label declaration to labels field. If the label name is
 * already declared, the label found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_label__CIResultFile(const CIResultFile *self,
                        const CIScope *scope,
                        CIDecl *label);

/**
 *
 * @brief Add struct declaration to structs field. If the struct name is
 * already declared, the struct found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_struct__CIResultFile(const CIResultFile *self, CIDecl *struct_);

/**
 *
 * @brief Add typedef declaration to typedefs field. If the typedef name is
 * already declared, the typedef found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_typedef__CIResultFile(const CIResultFile *self, CIDecl *typedef_);

/**
 *
 * @brief Add union declaration to unions field. If the union name is
 * already declared, the union found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
 */
const CIDecl *
add_union__CIResultFile(const CIResultFile *self, CIDecl *union_);

/**
 *
 * @brief Add variable declaration to variables field. If the variable name is
 * already declared, the variable found will be returned, but if it's a valid
 * redeclaration, for example in the case of prototypes, the `CIDecl` pointer
 * passed in this function will be returned. Finally, if the addition is
 * successful, a NULL pointer is returned.
 * @return const CIDecl*? (&)
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
 * @brief Get label declaration from id.
 */
CIDecl *
get_label_from_id__CIResultFile(const CIResultFile *self,
                                const CILabelID *label_id);

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
 * @brief Search label declaration in labels map.
 */
CIDecl *
search_label__CIResultFile(const CIResultFile *self,
                           const CIScope *scope,
                           const String *name);

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
 * @brief Search identifier declaration in decls map.
 */
CIDecl *
search_identifier__CIResultFile(const CIResultFile *self,
                                const CIScope *scope,
                                const String *name);

/**
 *
 * @brief Run the scanner, parser and the C generator.
 */
void
run__CIResultFile(CIResultFile *self);

/**
 *
 * @brief Add `enums` vector to `self` file.
 */
void
add_enums__CIResultFile(const CIResultFile *self, const CIResultFile *other);

/**
 *
 * @brief Add `functions` vector to `self` file.
 */
void
add_functions__CIResultFile(const CIResultFile *self,
                            const CIResultFile *other);

/**
 *
 * @brief Add `labels` vector to `self` file.
 */
void
add_labels__CIResultFile(const CIResultFile *self, const CIResultFile *other);

/**
 *
 * @brief Add `structs` vector to `self` file.
 */
void
add_structs__CIResultFile(const CIResultFile *self, const CIResultFile *other);

/**
 *
 * @brief Add `typedefs` vector to `self` file.
 */
void
add_typedefs__CIResultFile(const CIResultFile *self, const CIResultFile *other);

/**
 *
 * @brief Add `unions` vector to `self` file.
 */
void
add_unions__CIResultFile(const CIResultFile *self, const CIResultFile *other);

/**
 *
 * @brief Add `variables` vector to `self` file.
 */
void
add_variables__CIResultFile(const CIResultFile *self,
                            const CIResultFile *other);

/**
 *
 * @brief Merge `other` file to `self` file.
 */
void
merge_content__CIResultFile(const CIResultFile *self, CIResultFile *other);

/**
 *
 * @brief Merge `other` file to `self` file.
 */
void
include_content__CIResultFile(const CIResultFile *self, CIResultFile *other);

/**
 *
 * @brief Check if is the same filename (check the file id).
 */
bool
is_same_filename__CIResultFile(const CIResultFile *self, const char *filename);

/**
 *
 * @brief Free CIResultFile type.
 */
DESTRUCTOR(CIResultFile, CIResultFile *self);

typedef struct CIResultLib
{
    const char *name;        // const char* (&)
    OrderedHashMap *sources; // OrderedHashMap<CIResultFile*>*
    CIResultFile *file;
} CIResultLib;

/**
 *
 * @brief Construct CIResultLib type.
 */
CONSTRUCTOR(CIResultLib *,
            CIResultLib,
            const char *name,
            Usize id,
            const CIResult *result,
            const CIProjectConfig *config);

/**
 *
 * @brief Build library.
 */
void
build__CIResultLib(const CIResultLib *self);

/**
 *
 * @brief Free CIResultLib type.
 */
DESTRUCTOR(CIResultLib, CIResultLib *self);

typedef struct CIResultBin
{
    const char *name;   // const char* (&)
    CIResultFile *file; // CIResultFile*?
} CIResultBin;

/**
 *
 * @brief Construct CIResultBin type.
 */
CONSTRUCTOR(CIResultBin *, CIResultBin, const char *name);

/**
 *
 * @brief Set `file` field.
 */
void
set_file__CIResultBin(CIResultBin *self, CIResultFile *file);

/**
 *
 * @brief Free CIResultBin type.
 */
DESTRUCTOR(CIResultBin, CIResultBin *self);

typedef struct CIResult
{
    CIResultFile *predefined;      // CIResultFile* (&)
    const CIProjectConfig *config; // const CIProjectConfig* (&)
    const CIBuiltin *builtin;      // const CIBuiltin* (&)
    OrderedHashMap *headers;       // OrderedHashMap<CIResultFile*>*
    OrderedHashMap *sources;       // OrderedHashMap<CIResultFile* (&)>*
    OrderedHashMap *bins;          // OrderedHashMap<CIResultBin*>*
    OrderedHashMap *libs;          // OrderedHashMap<CIResultLib*>*
} CIResult;

/**
 *
 * @brief Construct CIResult type.
 */
inline CONSTRUCTOR(CIResult,
                   CIResult,
                   const CIProjectConfig *config,
                   const CIBuiltin *builtin)
{
    return (CIResult){
        .predefined = NULL,
        .config = config,
        .builtin = builtin,
        .headers = NEW(OrderedHashMap),
        .sources = NEW(OrderedHashMap),
        .bins = NEW(OrderedHashMap),
        .libs = NEW(OrderedHashMap),
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
                     String *filename_result,
                     File file_input);

/**
 *
 * @brief Add a bin to bins OrderedHashMap.
 */
CIResultBin *
add_bin__CIResult(const CIResult *self, char *name);

/**
 *
 * @brief Add a lib to libs OrderedHashMap.
 */
CIResultLib *
add_lib__CIResult(const CIResult *self, char *name);

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
 * @brief Check if the bin is already in the bins OrderedHashMap.
 */
inline bool
has_bin__CIResult(const CIResult *self, char *bin_name)
{
    return get__OrderedHashMap(self->bins, bin_name);
}

/**
 *
 * @brief Check if the lib is already in the libs OrderedHashMap.
 */
inline bool
has_lib__CIResult(const CIResult *self, char *lib_name)
{
    return get__OrderedHashMap(self->libs, lib_name);
}

/**
 *
 * @brief Load pre-defined file.
 */
void
load_predefined__CIResult(CIResult *self);

/**
 *
 * @brief Scan file.
 * @param path char* (&)
 */
CIResultFile *
scan_file__CIResult(const CIResult *self,
                    CIResultFile *owner,
                    CIResultFile *file_parent,
                    char *path,
                    Usize id);

/**
 *
 * @brief Run (Scan & Parse) file.
 * @param path char* (&)
 */
CIResultFile *
run_file__CIResult(const CIResult *self,
                   CIResultFile *owner,
                   CIResultFile *file_parent,
                   char *path,
                   Usize id);

/**
 *
 * @brief Add & Run from the passed library.
 */
void
add_and_run_lib__CIResult(const CIResult *self,
                          const CIProjectConfigLibrary *lib);

/**
 *
 * @brief Add & Run from the passed binary.
 */
void
add_and_run_bin__CIResult(const CIResult *self, const CIProjectConfigBin *bin);

/**
 *
 * @brief Add & Run from the passed header.
 */
CIResultFile *
add_and_run_header__CIResult(const CIResult *self,
                             CIResultFile *file_parent,
                             char *path,
                             const CIProjectConfig *config);

/**
 *
 * @brief Build result (all libraries and binaries).
 */
void
build__CIResult(CIResult *self);

/**
 *
 * @brief Free CIResult type.
 */
DESTRUCTOR(CIResult, const CIResult *self);

#endif // LILY_CORE_CC_CI_RESULT_H
