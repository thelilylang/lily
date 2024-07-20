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

#ifndef LILY_CORE_CC_CI_PARSER_H
#define LILY_CORE_CC_CI_PARSER_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/scanner.h>

typedef struct CIResultFile CIResultFile;

typedef struct CIParserVisitWaitingListItem
{
    String *name;             // String* (&)
    Vec *generic_params_list; // Vec<CIGenericParams* (&)>*?
} CIParserVisitWaitingListItem;

/**
 *
 * @brief Construct CIParserVisitWaitingListItem type.
 */
CONSTRUCTOR(CIParserVisitWaitingListItem *,
            CIParserVisitWaitingListItem,
            String *name,
            Vec *generic_params_list);

/**
 *
 * @brief Free CIParserVisitWaitingListItem type.
 */
DESTRUCTOR(CIParserVisitWaitingListItem, CIParserVisitWaitingListItem *self);

typedef struct CIParserVisitWaitingList
{
    HashMap *functions; // HashMap<CIParserVisitWaitingListItem*>*
    HashMap *typedefs;  // HashMap<CIParserVisitWaitingListItem*>*
    HashMap *unions;    // HashMap<CIParserVisitWaitingListItem*>*
    HashMap *structs;   // HashMap<CIParserVisitWaitingListItem*>*
} CIParserVisitWaitingList;

/**
 *
 * @brief Construct CIParserVisitWaitingList type.
 */
inline CONSTRUCTOR(CIParserVisitWaitingList, CIParserVisitWaitingList)
{
    return (CIParserVisitWaitingList){ .functions = NEW(HashMap),
                                       .typedefs = NEW(HashMap),
                                       .unions = NEW(HashMap),
                                       .structs = NEW(HashMap) };
}

/**
 *
 * @brief Free CIParserVisitWaitingList type.
 */
DESTRUCTOR(CIParserVisitWaitingList, const CIParserVisitWaitingList *self);

typedef struct CIParserMacroCallParam
{
    // content.first: CIToken* (&)
    // content.first...content.last: CIToken* (&)
    // content.last: CIToken*
    CITokens content;
    struct CIParserMacroCallParam *next; // CIParserMacroCallParam*?
} CIParserMacroCallParam;

/**
 *
 * @brief Construct CIParserMacroCallParam type.
 */
CONSTRUCTOR(CIParserMacroCallParam *, CIParserMacroCallParam, CITokens content);

/**
 *
 * @brief Free CIParserMacroCallParam type.
 */
DESTRUCTOR(CIParserMacroCallParam, CIParserMacroCallParam *self);

typedef struct CIParserMacroCallParams
{
    CIParserMacroCallParam *first;
    CIParserMacroCallParam *last; // CIParserMacroCallParam* (&)
    Usize len;
} CIParserMacroCallParams;

/**
 *
 * @brief Construct CIParserMacroCallParams type.
 */
inline CONSTRUCTOR(CIParserMacroCallParams, CIParserMacroCallParams)
{
    return (CIParserMacroCallParams){ .first = NULL, .last = NULL, .len = 0 };
}

/**
 *
 * @brief Add a param to params.
 */
void
add__CIParserMacroCallParams(CIParserMacroCallParams *self,
                             CIParserMacroCallParam *param);

/**
 *
 * @brief Get macro call param according the index.
 */
CIParserMacroCallParam *
get__CIParserMacroCallParams(const CIParserMacroCallParams *self, Usize index);

/**
 *
 * @brief Get variadic macro call param.
 */
inline CIParserMacroCallParam *
get_macro_param_variadic__CIParserMacroCallParams(
  const CIParserMacroCallParams *self)
{
    return self->last;
}

/**
 *
 * @brief Free CIParserMacroCallParams type.
 */
DESTRUCTOR(CIParserMacroCallParams, const CIParserMacroCallParams *self);

typedef struct CIParserMacroCall
{
    // Params of macro call
    CIParserMacroCallParams params;
    bool is_empty;
} CIParserMacroCall;

/**
 *
 * @brief Construct CIParserMacroCall type (is_empty=false).
 */
CONSTRUCTOR(CIParserMacroCall *, CIParserMacroCall);

/**
 *
 * @brief Construct CIParserMacroCall type (is_empty=true).
 */
VARIANT_CONSTRUCTOR(CIParserMacroCall *, CIParserMacroCall, is_empty);

/**
 *
 * @brief Free CIParserMacroCall type.
 */
DESTRUCTOR(CIParserMacroCall, CIParserMacroCall *self);

typedef struct CIParser
{
    CIResultFile *file;       // CIResultFile* (&)
    const CIScanner *scanner; // const CIScanner* (&)
    Usize *count_error;       // Usize* (&)
    Usize *count_warning;     // Usize* (&)
    const CITokens *tokens;   // const CITokens* (&)
    CIToken *current_token;   // CIToken* (&)
    CIToken *previous_token;  // CIToken* (&)
    Vec *macros_call;         // Vec<CIParserMacroCall*>*
    CIParserVisitWaitingList visit_waiting_list;
} CIParser;

/**
 *
 * @brief Construct CIParser type.
 */
CONSTRUCTOR(CIParser, CIParser, CIResultFile *file, const CIScanner *scanner);

/**
 *
 * @brief Construct CIParser type from tokens.
 */
CIParser
from_tokens__CIParser(CIResultFile *file, const CITokens *content);

/**
 *
 * @brief Run the parser.
 */
void
run__CIParser(CIParser *self);

/**
 *
 * @brief Parse data type.
 */
CIDataType *
parse_data_type__CIParser(CIParser *self);

/**
 *
 * @brief Parse expression.
 * @return CIExpr*?
 */
CIExpr *
parse_expr__CIParser(CIParser *self);

/**
 *
 * @brief Substitute generic data type.
 */
CIDataType *
substitute_generic__CIParser(const String *generic_name,
                             CIGenericParams *generic_params,
                             CIGenericParams *called_generic_params);

/**
 *
 * @brief Substitute data type (with generic).
 */
CIDataType *
substitute_data_type__CIParser(CIDataType *data_type,
                               CIGenericParams *generic_params,
                               CIGenericParams *called_generic_params);

/**
 *
 * @brief Free CIParser type (from tokens case).
 */
void
free_from_tokens_case__CIParser(const CIParser *self);

/**
 *
 * @brief Free CIParser type.
 */
DESTRUCTOR(CIParser, const CIParser *self);

#endif // LILY_CORE_CC_CI_PARSER_H
