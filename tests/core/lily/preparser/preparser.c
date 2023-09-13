#include "asm.c"
#include "await.c"
#include "break.c"
#include "import.c"
#include "macro.c"
#include "macro_expand.c"
#include "module.c"
#include "package.c"

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
    RUN_TEST();
}
