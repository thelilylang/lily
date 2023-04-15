#include <base/macros.h>
#include <base/new.h>

#include <cli/emit.h>
#include <cli/parse_config.h>

#include <stdio.h>
#include <stdlib.h>

// Parse build config.
static Config
parse_build__ParseConfig(const Option *op);

// Parse cc config.
static Config
parse_cc__ParseConfig(const Option *op);

// Parse compile config.
static Config
parse_compile__ParseConfig(const Option *op);

// Parse cpp config.
static Config
parse_cpp__ParseConfig(const Option *op);

// Parse init config.
static Config
parse_init__ParseConfig(const Option *op);

// Parse new config.
static Config
parse_new__ParseConfig(const Option *op);

// Parse run config.
static Config
parse_run__ParseConfig(const Option *op);

// Parse test config.
static Config
parse_test__ParseConfig(const Option *op);

// Parse to config.
static Config
parse_to__ParseConfig(const Option *op);

#define CHECK_IF_INPUT_IS_ALREADY_SET(input)                         \
    if (input[0]) {                                                  \
        EMIT_ERROR("input (filename, path or name) is already set"); \
        exit(1);                                                     \
    }

Config
parse_build__ParseConfig(const Option *op)
{
    bool verbose = false;

    for (Usize i = 0; i < op->build->len; ++i) {
        switch (CAST(BuildOption *, get__Vec(op->build, i))->kind) {
            case BUILD_OPTION_KIND_VERBOSE:
                verbose = true;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, build, NEW(BuildConfig, verbose));
}

Config
parse_cc__ParseConfig(const Option *op)
{
    char *filename = "";

    for (Usize i = 0; i < op->cc->len; ++i) {
        switch (CAST(CcOption *, get__Vec(op->cc, i))->kind) {
            case CC_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(CcOption *, get__Vec(op->cc, i))->filename;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, cc, NEW(CcConfig, filename));
}

Config
parse_compile__ParseConfig(const Option *op)
{
    char *filename = "";
    bool dump_scanner = false, dump_parser = false, dump_typecheck = false,
         dump_ir = false;
    bool run_scanner = false, run_parser = false, run_typecheck = false,
         run_ir = false;
    bool cc_ir = false, cpp_ir = false, js_ir = false;

    for (Usize i = 0; i < op->compile->len; ++i) {
        switch (CAST(CompileOption *, get__Vec(op->cpp, i))->kind) {
            case COMPILE_OPTION_KIND_CC_IR:
                cc_ir = true;
                break;
            case COMPILE_OPTION_KIND_CPP_IR:
                cpp_ir = true;
                break;
            case COMPILE_OPTION_KIND_DUMP_IR:
                dump_ir = true;
                break;
            case COMPILE_OPTION_KIND_DUMP_PARSER:
                dump_parser = true;
                break;
            case COMPILE_OPTION_KIND_DUMP_SCANNER:
                dump_scanner = true;
                break;
            case COMPILE_OPTION_KIND_DUMP_TYPECHECK:
                dump_typecheck = true;
                break;
            case COMPILE_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(CcOption *, get__Vec(op->compile, i))->filename;
                break;
            case COMPILE_OPTION_KIND_JS_IR:
                js_ir = true;
                break;
            case COMPILE_OPTION_KIND_RUN_IR:
                run_ir = true;
                break;
            case COMPILE_OPTION_KIND_RUN_PARSER:
                run_parser = true;
                break;
            case COMPILE_OPTION_KIND_RUN_SCANNER:
                run_scanner = true;
                break;
            case COMPILE_OPTION_KIND_RUN_TYPECHECK:
                run_typecheck = true;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config,
                       compile,
                       NEW(CompileConfig,
                           filename,
                           run_scanner,
                           run_parser,
                           run_typecheck,
                           run_ir,
                           dump_scanner,
                           dump_parser,
                           dump_typecheck,
                           dump_ir,
                           cc_ir,
                           cpp_ir,
                           js_ir));
}

Config
parse_cpp__ParseConfig(const Option *op)
{
    char *filename = "";

    for (Usize i = 0; i < op->cpp->len; ++i) {
        switch (CAST(CppOption *, get__Vec(op->cpp, i))->kind) {
            case CPP_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(CppOption *, get__Vec(op->cpp, i))->filename;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, cpp, NEW(CppConfig, filename));
}

Config
parse_init__ParseConfig(const Option *op)
{
    char *path = "";

    for (Usize i = 0; i < op->init->len; ++i) {
        switch (CAST(InitOption *, get__Vec(op->init, i))->kind) {
            case INIT_OPTION_KIND_PATH:
                CHECK_IF_INPUT_IS_ALREADY_SET(path);
                path = (char *)CAST(InitOption *, get__Vec(op->init, i))->path;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, init, NEW(InitConfig, path));
}

Config
parse_new__ParseConfig(const Option *op)
{
    char *name = "";

    for (Usize i = 0; i < op->new->len; ++i) {
        switch (CAST(NewOption *, get__Vec(op->new, i))->kind) {
            case NEW_OPTION_KIND_NAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(name);
                name = (char *)CAST(NewOption *, get__Vec(op->new, i))->name;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, new, NEW(NewConfig, name));
}

Config
parse_run__ParseConfig(const Option *op)
{
    char *filename = "";

    for (Usize i = 0; i < op->run->len; ++i) {
        switch (CAST(RunOption *, get__Vec(op->run, i))->kind) {
            case RUN_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(RunOption *, get__Vec(op->run, i))->filename;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, run, NEW(RunConfig, filename));
}

Config
parse_test__ParseConfig(const Option *op)
{
    char *filename = "";

    for (Usize i = 0; i < op->test->len; ++i) {
        switch (CAST(TestOption *, get__Vec(op->test, i))->kind) {
            case TEST_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(TestOption *, get__Vec(op->test, i))->filename;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(Config, test, NEW(TestConfig, filename));
}

Config
parse_to__ParseConfig(const Option *op)
{
    char *filename = "";
    bool from_cc = false, from_cpp = false, from_js = false;

    for (Usize i = 0; i < op->to->len; ++i) {
        switch (CAST(ToOption *, get__Vec(op->to, i))->kind) {
            case TO_OPTION_KIND_FILENAME:
                CHECK_IF_INPUT_IS_ALREADY_SET(filename);
                filename =
                  (char *)CAST(ToOption *, get__Vec(op->to, i))->filename;
                break;
            case TO_OPTION_KIND_FROM_CC:
                from_cc = true;
                break;
            case TO_OPTION_KIND_FROM_CPP:
                from_cpp = true;
                break;
            case TO_OPTION_KIND_FROM_JS:
                from_js = true;
                break;
            default:
                break;
        }
    }

    return NEW_VARIANT(
      Config, to, NEW(ToConfig, filename, from_cc, from_cpp, from_js));
}

Config
run__ParseConfig(const Option *op)
{
    switch (op->kind) {
        case CONFIG_KIND_BUILD:
            return parse_build__ParseConfig(op);
        case CONFIG_KIND_CC:
            return parse_cc__ParseConfig(op);
        case CONFIG_KIND_COMPILE:
            return parse_compile__ParseConfig(op);
        case CONFIG_KIND_CPP:
            return parse_cpp__ParseConfig(op);
        case CONFIG_KIND_INIT:
            return parse_init__ParseConfig(op);
        case CONFIG_KIND_NEW:
            return parse_new__ParseConfig(op);
        case CONFIG_KIND_RUN:
            return parse_run__ParseConfig(op);
        case CONFIG_KIND_TEST:
            return parse_test__ParseConfig(op);
        case CONFIG_KIND_TO:
            return parse_to__ParseConfig(op);
        default:
            UNREACHABLE("unknown config");
    }
}
