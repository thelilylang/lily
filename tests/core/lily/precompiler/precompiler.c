#include "import.c"
#include "macro.c"
#include "package.c"

#include <base/test.h>

int
main()
{
    NEW_TEST("precompiler");
    ADD_SIMPLE(import);
    ADD_SIMPLE(macro);
    RUN_TEST();
}
