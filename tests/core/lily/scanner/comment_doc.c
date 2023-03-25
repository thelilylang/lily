#include "util.c"

#include <base/test.h>

#include <stdio.h>
#include <string.h>

SIMPLE(comment_doc, {
    RUN_SCANNER(FILE_COMMENT_DOC);

	LilyToken *token = get__Vec(scanner.tokens, 0);

	TEST_ASSERT_EQ(token->kind, LILY_TOKEN_KIND_COMMENT_DOC);
	TEST_ASSERT(!strcmp(token->comment_doc->buffer, " this is a comment doc"));

    FREE_SCANNER();
});
