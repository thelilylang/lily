#include "asm.c"
#include "await.c"
#include "block.c"
#include "break.c"
#include "defer.c"
#include "drop.c"
#include "for.c"
#include "if.c"
#include "import.c"
#include "macro.c"
#include "macro_expand.c"
#include "match.c"
#include "module.c"
#include "next.c"
#include "package.c"
#include "raise.c"
#include "return.c"
#include "try.c"
#include "variable.c"
#include "while.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("preparser");
    ADD_SIMPLE(import);
    ADD_SIMPLE(macro);
    ADD_SIMPLE(package);
    ADD_SIMPLE(module);
    ADD_SIMPLE(macro_expand);
    ADD_SIMPLE(asm);
    ADD_SIMPLE(await);
    ADD_SIMPLE(break);
    ADD_SIMPLE(defer);
    ADD_SIMPLE(drop);
    ADD_SIMPLE(if);
    ADD_SIMPLE(for);
    ADD_SIMPLE(while);
    ADD_SIMPLE(raise);
    ADD_SIMPLE(return);
    ADD_SIMPLE(try);
    ADD_SIMPLE(match);
    ADD_SIMPLE(next);
    ADD_SIMPLE(block);
    ADD_SIMPLE(variable);
    RUN_TEST();
}
