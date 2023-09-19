#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_array, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_array2, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_array3, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_array4, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY_4);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
