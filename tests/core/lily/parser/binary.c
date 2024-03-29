#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_binary, {
    RUN_PARSE_EXPR(FILE_EXPR_BINARY);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_binary2, {
    RUN_PARSE_EXPR(FILE_EXPR_BINARY_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

CASE(expr_binary3, {
    RUN_PARSE_EXPR(FILE_EXPR_BINARY_3);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
