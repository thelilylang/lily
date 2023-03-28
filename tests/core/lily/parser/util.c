#ifndef UTIL_C
#define UTIL_C

#include <base/file.h>

#include <core/lily/parser.h>
#include <core/lily/scanner.h>

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
#define FILE_EXPR_ARRAY2 "./tests/core/lily/parser/input/array/array2.lily"

LilyAstDataType *
run_parse_data_type(File *file)
{
    LilyScanner scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, file->content), file));

    run__LilyScanner(&scanner, false);

    Usize count_error = 0;
    Usize count_warning = 0;
    LilyParseBlock parse_block =
      (LilyParseBlock){ .tokens = scanner.tokens,
                        .current = get__Vec(scanner.tokens, 0),
                        .file = file,
                        .count_error = &count_error,
                        .count_warning = &count_warning,
                        .position = 0 };

    LilyAstDataType *dt = CALL_TEST(parse_data_type, &parse_block);

    FREE(LilyScanner, &scanner);

    return dt;
}

#define RUN_PARSE_DATA_TYPE(filename)          \
    char *content = read_file__File(filename); \
    File file = NEW(File, filename, content);  \
    LilyAstDataType *dt = run_parse_data_type(&file);

#define FREE_RUN_PARSE_DATA_TYPE() \
    FREE(File, &file);             \
    if (dt)                        \
        FREE(LilyAstDataType, dt);

LilyAstExpr *
run_parse_expr(File *file)
{
    LilyScanner scanner =
      NEW(LilyScanner, NEW(Source, NEW(Cursor, file->content), file));

    run__LilyScanner(&scanner, false);

    Usize count_error = 0;
    Usize count_warning = 0;

    LilyParseBlock parse_block =
      (LilyParseBlock){ .tokens = scanner.tokens,
                        .current = get__Vec(scanner.tokens, 0),
                        .file = file,
                        .count_error = &count_error,
                        .count_warning = &count_warning,
                        .position = 0 };

    LilyAstExpr *expr = CALL_TEST(parse_expr, &parse_block);

    FREE(LilyScanner, &scanner);

    return expr;
}

#define RUN_PARSE_EXPR(filename)               \
    char *content = read_file__File(filename); \
    File file = NEW(File, filename, content);  \
    LilyAstExpr *expr = run_parse_expr(&file);

#define FREE_RUN_PARSE_EXPR() \
    FREE(File, &file);        \
    if (expr)                 \
        FREE(LilyAstExpr, expr);

#endif // UTIL_C
