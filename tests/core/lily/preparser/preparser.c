#include "asm.c"
#include "await.c"
#include "block.c"
#include "break.c"
#include "class.c"
#include "constant.c"
#include "defer.c"
#include "drop.c"
#include "enum_object.c"
#include "for.c"
#include "fun.c"
#include "if.c"
#include "import.c"
#include "macro.c"
#include "macro_expand.c"
#include "match.c"
#include "module.c"
#include "next.c"
#include "package.c"
#include "raise.c"
#include "record.c"
#include "record_object.c"
#include "return.c"
#include "trait.c"
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
    ADD_SIMPLE(fun);
    ADD_SIMPLE(constant);
    ADD_SIMPLE(class);
    ADD_SIMPLE(trait);
    ADD_SIMPLE(record_object);
    ADD_SIMPLE(enum_object);
    ADD_SIMPLE(record);
    RUN_TEST();
}
