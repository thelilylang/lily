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

typedef struct CIParserWaitForVisit
{
    enum CIDeclKind kind;
    String *name;             // String* (&)
    Vec *generic_params_list; // Vec<CIGenericParams* (&)>*
} CIParserWaitForVisit;

/**
 *
 * @brief Construct CIParserWaitForVisit type.
 */
CONSTRUCTOR(CIParserWaitForVisit *,
            CIParserWaitForVisit,
            enum CIDeclKind kind,
            String *name,
            CIGenericParams *generic_params);

/**
 *
 * @brief Free CIParserWaitForVisit type.
 */
DESTRUCTOR(CIParserWaitForVisit, CIParserWaitForVisit *self);

typedef struct CIParserMacro
{
    Vec *params; // Vec<Vec<CIToken* (&)>*>* (&)
} CIParserMacro;

/**
 *
 * @brief Construct CIParserMacro type.
 */
CONSTRUCTOR(CIParserMacro *, CIParserMacro, Vec *params);

/**
 *
 * @brief Free CIParserMacro type.
 */
inline DESTRUCTOR(CIParserMacro, CIParserMacro *self)
{
    lily_free(self);
}

#define CI_PARSER_MACROS_MAX_SIZE CI_TOKENS_ITERS_MAX_SIZE

typedef struct CIParser
{
    CIResultFile *file;       // CIResultFile* (&)
    const CIScanner *scanner; // CIScanner* (&)
    Usize *count_error;       // Usize* (&)
    Usize *count_warning;     // Usize* (&)
    CITokensIters tokens_iters;
    Stack *macros;            // Vec<CIParserMacro*>*
    HashMap *wait_visit_list; // HashMap<CIParserWaitForVisit*>*
} CIParser;

/**
 *
 * @brief Construct CIParser type.
 */
CONSTRUCTOR(CIParser, CIParser, CIResultFile *file, const CIScanner *scanner);

/**
 *
 * @brief Run the parser.
 */
void
run__CIParser(CIParser *self);

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
 * @brief Free CIParser type.
 */
DESTRUCTOR(CIParser, const CIParser *self);

#endif // LILY_CORE_CC_CI_PARSER_H
