#ifndef UTIL_C
#define UTIL_C

#include <base/file.h>

#include <core/lily/parser/parser.h>
#include <core/lily/scanner/scanner.h>

#define FILE_DATA_TYPE_ARRAY_DYNAMIC \
    "./tests/core/lily/parser/input/data_type/array/dynamic.lily"
#define FILE_DATA_TYPE_ARRAY_DYNAMIC_2 \
    "./tests/core/lily/parser/input/data_type/array/dynamic2.lily"
#define FILE_DATA_TYPE_ARRAY_SIZED \
    "./tests/core/lily/parser/input/data_type/array/sized.lily"
#define FILE_DATA_TYPE_ARRAY_SIZED2 \
    "./tests/core/lily/parser/input/data_type/array/sized2.lily"
#define FILE_DATA_TYPE_ARRAY_MULTI_POINTERS \
    "./tests/core/lily/parser/input/data_type/array/multi_pointers.lily"
#define FILE_DATA_TYPE_ARRAY_MULTI_POINTERS_2 \
    "./tests/core/lily/parser/input/data_type/array/multi_pointers2.lily"
#define FILE_DATA_TYPE_ARRAY_UNDETERMINED \
    "./tests/core/lily/parser/input/data_type/array/undetermined.lily"
#define FILE_DATA_TYPE_ARRAY_UNDETERMINED_2 \
    "./tests/core/lily/parser/input/data_type/array/undetermined2.lily"
#define FILE_DATA_TYPE_LAMBDA \
    "./tests/core/lily/parser/input/data_type/lambda.lily"
#define FILE_DATA_TYPE_TUPLE \
    "./tests/core/lily/parser/input/data_type/tuple.lily"
#define FILE_DATA_TYPE_CUSTOM \
    "./tests/core/lily/parser/input/data_type/custom.lily"
#define FILE_EXPR_ARRAY "./tests/core/lily/parser/input/array/array.lily"
#define FILE_EXPR_ARRAY_2 "./tests/core/lily/parser/input/array/array2.lily"
#define FILE_EXPR_ARRAY_3 "./tests/core/lily/parser/input/array/array3.lily"
#define FILE_EXPR_ARRAY_4 "./tests/core/lily/parser/input/array/array4.lily"
#define FILE_EXPR_BINARY "./tests/core/lily/parser/input/binary/binary.lily"
#define FILE_EXPR_BINARY_2 "./tests/core/lily/parser/input/binary/binary2.lily"
#define FILE_EXPR_BINARY_3 "./tests/core/lily/parser/input/binary/binary3.lily"
#define FILE_EXPR_ACCESS "./tests/core/lily/parser/input/access/access.lily"
#define FILE_EXPR_ACCESS_2 "./tests/core/lily/parser/input/access/access2.lily"
#define FILE_EXPR_ACCESS_3 "./tests/core/lily/parser/input/access/access3.lily"
#define FILE_EXPR_ACCESS_4 "./tests/core/lily/parser/input/access/access4.lily"
#define FILE_EXPR_ACCESS_5 "./tests/core/lily/parser/input/access/access5.lily"
#define FILE_EXPR_CALL "./tests/core/lily/parser/input/access/call.lily"
#define FILE_EXPR_CALL_2 "./tests/core/lily/parser/input/access/call2.lily"
#define FILE_EXPR_CALL_3 "./tests/core/lily/parser/input/access/call3.lily"
#define FILE_EXPR_CALL_4 "./tests/core/lily/parser/input/access/call4.lily"
#define FILE_EXPR_CALL_5 "./tests/core/lily/parser/input/access/call5.lily"
#define FILE_EXPR_CAST "./tests/core/lily/parser/input/cast.lily"
#define FILE_EXPR_IDENTIFIER "./tests/core/lily/parser/input/identifier.lily"
#define FILE_EXPR_LITERAL "./tests/core/lily/parser/input/literal/literal.lily"
#define FILE_EXPR_LITERAL_2 \
    "./tests/core/lily/parser/input/literal/literal2.lily"
#define FILE_EXPR_LITERAL_3 \
    "./tests/core/lily/parser/input/literal/literal3.lily"
#define FILE_EXPR_LITERAL_4 \
    "./tests/core/lily/parser/input/literal/literal4.lily"
#define FILE_EXPR_LITERAL_5 \
    "./tests/core/lily/parser/input/literal/literal5.lily"
#define FILE_EXPR_LITERAL_6 \
    "./tests/core/lily/parser/input/literal/literal6.lily"
#define FILE_EXPR_TUPLE "./tests/core/lily/parser/input/tuple.lily"
#define FILE_EXPR_UNARY "./tests/core/lily/parser/input/unary/unary.lily"
#define FILE_EXPR_UNARY_2 "./tests/core/lily/parser/input/unary/unary2.lily"
#define FILE_EXPR_UNARY_3 "./tests/core/lily/parser/input/unary/unary3.lily"
#define FILE_EXPR_UNARY_4 "./tests/core/lily/parser/input/unary/unary4.lily"

LilyAstDataType *
run_parse_data_type(File *file, Usize *count_error, Usize *count_warning)
{
    LilyScanner scanner = NEW(
      LilyScanner, NEW(Source, NEW(Cursor, file->content), file), count_error);

    run__LilyScanner(&scanner, false);

    LilyParseBlock parse_block =
      (LilyParseBlock){ .tokens = scanner.tokens,
                        .current = get__Vec(scanner.tokens, 0),
                        .file = file,
                        .count_error = count_error,
                        .count_warning = count_warning,
                        .position = 0 };

    LilyAstDataType *dt = CALL_TEST(parse_data_type, &parse_block);

    FREE(LilyScanner, &scanner);

    return dt;
}

#define RUN_PARSE_DATA_TYPE(filename)          \
    char *content = read_file__File(filename); \
    File file = NEW(File, filename, content);  \
    Usize count_error = 0;                     \
    Usize count_warning = 0;                   \
    LilyAstDataType *dt =                      \
      run_parse_data_type(&file, &count_error, &count_warning);

#define FREE_RUN_PARSE_DATA_TYPE() \
    FREE(File, &file);             \
    if (dt)                        \
        FREE(LilyAstDataType, dt);

LilyAstExpr *
run_parse_expr(File *file, Usize *count_error, Usize *count_warning)
{
    LilyScanner scanner = NEW(
      LilyScanner, NEW(Source, NEW(Cursor, file->content), file), count_error);

    run__LilyScanner(&scanner, false);

    LilyParseBlock parse_block =
      (LilyParseBlock){ .tokens = scanner.tokens,
                        .current = get__Vec(scanner.tokens, 0),
                        .file = file,
                        .count_error = count_error,
                        .count_warning = count_warning,
                        .position = 0 };

    LilyAstExpr *expr = CALL_TEST(parse_expr, &parse_block);

    FREE(LilyScanner, &scanner);

    return expr;
}

#define RUN_PARSE_EXPR(filename)               \
    char *content = read_file__File(filename); \
    File file = NEW(File, filename, content);  \
    Usize count_error = 0;                     \
    Usize count_warning = 0;                   \
    LilyAstExpr *expr = run_parse_expr(&file, &count_error, &count_warning);

#define FREE_RUN_PARSE_EXPR() \
    FREE(File, &file);        \
    if (expr)                 \
        FREE(LilyAstExpr, expr);

#endif // UTIL_C
