#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_call, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_call2, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_call3, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_call4, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_call5, {
    RUN_PARSE_EXPR(FILE_EXPR_ACCESS_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
