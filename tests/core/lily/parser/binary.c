#include "util.c"

#include <base/test.h>

#include <core/lily/parser.h>

#include <string.h>

SKIP_CASE(expr_binary, {
	RUN_PARSE_EXPR(FILE_EXPR_BINARY);
	FREE_RUN_PARSE_EXPR();
});

SKIP_CASE(expr_binary2, {
	RUN_PARSE_EXPR(FILE_EXPR_BINARY_2);
	FREE_RUN_PARSE_EXPR();
});

SKIP_CASE(expr_binary3, {
	RUN_PARSE_EXPR(FILE_EXPR_BINARY_3);
	FREE_RUN_PARSE_EXPR();
});
