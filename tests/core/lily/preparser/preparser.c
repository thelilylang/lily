#include "import.c"
#include "macro.c"
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
    RUN_TEST();
}
