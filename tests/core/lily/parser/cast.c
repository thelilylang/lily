#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_cast, {
    RUN_PARSE_EXPR(FILE_EXPR_CAST);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
