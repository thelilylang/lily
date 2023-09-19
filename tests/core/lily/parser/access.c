#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_access, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_access2, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_access3, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_access4, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_4);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_access5, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_5);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
