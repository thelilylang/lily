#include "util.c"

#include <base/test.h>

#include <core/lily/parser.h>

SKIP_CASE(expr_lambda, {
    RUN_PARSE_EXPR(FILE_EXPR_LAMBDA);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});

SKIP_CASE(expr_lambda2, {
    RUN_PARSE_EXPR(FILE_EXPR_LAMBDA_2);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});