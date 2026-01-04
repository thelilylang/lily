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

#ifndef LILY_CORE_CC_CI_PARSER_H
#define LILY_CORE_CC_CI_PARSER_H

#include <core/cc/ci/ast.h>
#include <core/cc/ci/resolver.h>
#include <core/cc/ci/scanner.h>

typedef struct CIResultFile CIResultFile;

typedef struct CIParserSpan
{
    Usize line;
    Usize column;
} CIParserSpan;

/**
 *
 * @brief Construct CIParserSpan type.
 */
inline CONSTRUCTOR(CIParserSpan, CIParserSpan, Usize line, Usize column)
{
    return (CIParserSpan){ .line = line, .column = column };
}

/**
 *
 * @brief Construct CIParserSpan type from CIToken (token).
 */
inline CIParserSpan
from_token__CIParserSpan(const CIToken *token)
{
    return NEW(
      CIParserSpan, token->location.start_line, token->location.start_column);
}

/**
 *
 * @brief Construct CIParserSpan type with default values.
 */
inline CIParserSpan
default__CIParserSpan()
{
    return (CIParserSpan){ .line = 0, .column = 0 };
}

typedef struct CIParser
{
    CIResultFile *file;   // CIResultFile* (&)
    Usize *count_error;   // Usize* (&)
    Usize *count_warning; // Usize* (&)
    Usize resolved_tokens_count;
    const CIResolvedTokens *resolved_tokens; // const CIResolvedTokens*? (&)
    CIToken *current_token;                  // CIToken*? (&)
    CIToken *previous_token;                 // CIToken*? (&)
    CIParserSpan current_span;
    CIParserSpan previous_span;
} CIParser;

/**
 *
 * @brief Construct CIParser type.
 */
CONSTRUCTOR(CIParser, CIParser, CIResultFile *file);

/**
 *
 * @brief Initialize the parser.
 */
void
init__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens);

/**
 *
 * @brief Run the parser.
 * @param resolved_tokens const CIResolvedTokens* (&)
 */
void
run__CIParser(CIParser *self, const CIResolvedTokens *resolved_tokens);

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
 * @return CIDataType* (&)
 */
CIDataType *
substitute_generic__CIParser(const String *generic_name,
                             CIGenericParams *generic_params,
                             CIGenericParams *called_generic_params);

/**
 *
 * @param unresolved_generic_params CIGenericParams*? (&)
 * @param generic_params const CIGenericParams* (&)
 * @param called_generic_params const CIGenericParams* (&)
 * @return CIGenericParams*?
 */
CIGenericParams *
substitute_generic_params__CIParser(
  CIGenericParams *unresolved_generic_params,
  const CIGenericParams *generic_params,
  const CIGenericParams *called_generic_params);

/**
 *
 * @brief Substitute data type (with generic).
 * @param data_type CIDataType* (&)
 * @param serialized_name String*? (&) - In some cases, we need to give the
 * option of passing a `serialized_name`, to be used in place of the original
 * defined name (in the data type), if both are non-null.
 * @return CIDataType*
 */
CIDataType *
substitute_data_type__CIParser(CIDataType *data_type,
                               CIGenericParams *generic_params,
                               CIGenericParams *called_generic_params,
                               String *serialized_name);

/**
 *
 * @return CIDecl*? (&)
 */
CIDecl *
search_enum__CIParser(const CIParser *self, const String *name);

/**
 *
 * @param called_generic_params CIGenericParams*? (&)
 * @return CIDecl*? (&)
 */
CIDecl *
search_struct__CIParser(const CIParser *self,
                        const String *name,
                        CIGenericParams *called_generic_params);

/**
 *
 * @param called_generic_params CIGenericParams*? (&)
 * @return CIDecl*? (&)
 */
CIDecl *
search_typedef__CIParser(const CIParser *self,
                         const String *name,
                         CIGenericParams *called_generic_params);

/**
 *
 * @param called_generic_params CIGenericParams*? (&)
 * @return CIDecl*? (&)
 */
CIDecl *
search_union__CIParser(const CIParser *self,
                       const String *name,
                       CIGenericParams *called_generic_params);

/**
 *
 * @brief Check if the parser has reach end.
 */
inline bool
has_reach_end__CIParser(CIParser *self)
{
    return self->resolved_tokens_count ==
           count__CIResolvedTokens(self->resolved_tokens);
}

/**
 *
 * @brief Free CIParser type.
 */
DESTRUCTOR(CIParser, const CIParser *self);

#endif // LILY_CORE_CC_CI_PARSER_H
