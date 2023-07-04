#include "util.c"

#include <base/test.h>

#include <core/lily/parser/parser.h>

CASE(expr_identifier, {
    RUN_PARSE_EXPR(FILE_EXPR_IDENTIFIER);

    TEST_ASSERT(expr);

    FREE_RUN_PARSE_EXPR();
});
