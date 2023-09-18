#include "util.c"

#include <base/test.h>

SIMPLE(package, {
    RUN_PRECOMPILER(FILE_PACKAGE);
    FREE_PRECOMPILER();
});
