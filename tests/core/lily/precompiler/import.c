#include "util.c"

#include <base/test.h>

SIMPLE(import, {
    RUN_PRECOMPILER(FILE_IMPORT);
    FREE_PRECOMPILER();
});
