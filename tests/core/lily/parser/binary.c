#include "util.c"

#include <base/print.h>
#include <base/test.h>

#include <core/lily/parser.h>

#include <stdio.h>
#include <string.h>

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
