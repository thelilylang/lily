/*
 * MIT License
 *
 * Copyright (c) 2022-2025 ArthurPV
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

#ifndef LILY_CORE_LILY_PARSER_H
#define LILY_CORE_LILY_PARSER_H

#include <base/macros.h>
#include <base/new.h>
#include <base/types.h>
#include <base/vec.h>

#include <core/lily/parser/ast/data_type.h>
#include <core/lily/parser/ast/expr.h>
#include <core/lily/preparser/preparser.h>

typedef struct LilyPackage LilyPackage;
typedef struct LilyParser LilyParser;

typedef struct LilyParseBlock
{
    LilyParser *parser;
    Vec *tokens; // Vec<LilyToken*>*
    LilyToken *current;
    LilyToken *previous;
    const File *file;
    Usize *count_error;
    Usize *count_warning;
    Usize position;
} LilyParseBlock;

/**
 *
 * @brief Construct LilyParseBlock
 */
CONSTRUCTOR(LilyParseBlock, LilyParseBlock, LilyParser *parser, Vec *tokens);

struct LilyParser
{
    Vec *decls; // Vec<LilyAstDecl*>*?
    LilyPackage *package;
    LilyPackage *root_package;
    LilyPreparserDecl *current;              // LilyPreparserDecl*?
    const LilyPreparserInfo *preparser_info; // LilyPreparserInfo*? (&)
    // LilyDumpConfig *dump_config;
    Usize position;
    // NOTE: All optional null values of this struct are possible in case the
    // parser is construct to apply macro-expansion in records, record objects,
    // classes, traits, enums or enum objects. In other uses all values should
    // be non-null.
};

/**
 *
 * @brief Construct LilyParser type.
 * @param preparser_info LilyPreparserInfo*?
 */
CONSTRUCTOR(LilyParser,
            LilyParser,
            LilyPackage *package,
            LilyPackage *root_package,
            const LilyPreparserInfo *preparser_info);

/**
 *
 * @brief Parse data type.
 * @note This function is only used in test.
 */
TEST(LilyAstDataType *, parse_data_type, LilyParseBlock *self);

/**
 *
 * @brief Parse expression.
 * @note This function is only used in test.
 */
TEST(LilyAstExpr *, parse_expr, LilyParseBlock *self);

/**
 *
 * @brief Run the parser.
 * @param Disable the debug when the parser parse into a macro expand.
 */
void
run__LilyParser(LilyParser *self, bool parse_for_macro_expand);

/**
 *
 * @brief Free LilyParser type.
 */
DESTRUCTOR(LilyParser, const LilyParser *self);

#endif // LILY_CORE_LILY_PARSER_H
