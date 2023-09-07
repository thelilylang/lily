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
    RUN_TEST();
}
