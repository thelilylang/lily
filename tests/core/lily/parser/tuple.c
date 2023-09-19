#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_tuple, {
    RUN_PARSE_EXPR(FILE_EXPR_TUPLE);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
