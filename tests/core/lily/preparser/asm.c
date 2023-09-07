#include "util.c"

#include <base/test.h>

SIMPLE(asm, {
    RUN_PREPARSER(FILE_ASM);
    FREE_PREPARSER();
});
