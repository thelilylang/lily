#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_unary, {
    RUN_PARSE_EXPR(FILE_EXPR_UNARY);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_unary2, {
    RUN_PARSE_EXPR(FILE_EXPR_UNARY_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_unary3, {
    RUN_PARSE_EXPR(FILE_EXPR_UNARY_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_unary4, {
    RUN_PARSE_EXPR(FILE_EXPR_UNARY_4);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
