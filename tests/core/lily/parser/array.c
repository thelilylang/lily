#include "util.c"

#include <base/test.h>

#include <core/lily/parser.h>

#include <string.h>

SKIP_CASE(expr_array, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY);
    FREE_RUN_PARSE_EXPR();
});

SKIP_CASE(expr_array2, {
    RUN_PARSE_EXPR(FILE_EXPR_ARRAY2);
    FREE_RUN_PARSE_EXPR();
});
