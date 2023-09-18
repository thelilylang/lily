#include "util.c"

#include <base/test.h>

SIMPLE(macro, {
    RUN_PRECOMPILER(FILE_MACRO);
    FREE_PRECOMPILER();
});
