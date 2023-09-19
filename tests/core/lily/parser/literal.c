#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_literal, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_literal2, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_literal3, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_literal4, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL_4);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_literal5, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL_5);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_literal6, {
    RUN_PARSE_EXPR(FILE_EXPR_LITERAL_6);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
