cmake_minimum_required(VERSION 3.20)
project(
  lily
  VERSION 0.0.0
  HOMEPAGE_URL "https://github.com/ArthurPV/lily"
  LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

if(NOT WIN32)
  add_compile_options(-Wall -O3 -fdiagnostics-color=always)
endif()

if(CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_C_FLAGS "/wd4710 /wd4711 /wd4255")
endif()

if(UNIX AND NOT APPLE)
  set(LILY_LINUX 1)
endif()

find_package(LLVM 15 CONFIG REQUIRED)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

set(LILY_INCLUDE include lib/local)

# lily_base
set(BASE_SRC
    src/base/alloc.c
    src/base/atof.c
    src/base/atoi.c
    src/base/cli/command.c
    src/base/cli/default_action.c
    src/base/cli/diagnostic.c
    src/base/cli/help.c
    src/base/cli/option.c
    src/base/cli/value.c
    src/base/cli/result/option.c
    src/base/cli/result/value.c
    src/base/cli/result.c
    src/base/cli.c
    src/base/color.c
    src/base/file.c
    src/base/format.c
    src/base/hash/custom.c
    src/base/hash/fnv.c
    src/base/hash/jenkins.c
    src/base/hash/sip.c
    src/base/hash_map.c
    src/base/hash_set.c
    src/base/heap.c
    src/base/itoa.c
    src/base/io.c
    src/base/list.c
    src/base/linked_list.c
    src/base/optional.c
    src/base/ordered_hash_map.c
    src/base/queue.c
    src/base/sized_array.c
    src/base/stack.c
    src/base/str.c
    src/base/string.c
    src/base/test.c
    src/base/tree.c
    src/base/vec.c)

add_library(lily_base STATIC ${BASE_SRC})
target_include_directories(lily_base PRIVATE ${LILY_INCLUDE})

# lily_cli
set(CLI_LILY_SRC src/cli/lily/lily.c src/cli/lily/parse_config.c)

add_library(lily_cli STATIC ${CLI_LILY_SRC})
target_link_libraries(lily_cli PUBLIC lily_base)
target_include_directories(lily_cli PRIVATE ${LILY_INCLUDE})

# lilyc_cli
set(CLI_LILYC_SRC src/cli/lilyc/lilyc.c src/cli/lilyc/parse_config.c)

add_library(lilyc_cli STATIC ${CLI_LILYC_SRC})
target_link_libraries(lilyc_cli PUBLIC lily_base)
target_include_directories(lilyc_cli PRIVATE ${LILY_INCLUDE})

# lily_command
set(COMMAND_LILY_SRC
    src/command/lily/build/build.c
    src/command/lily/cc/cc.c
    src/command/lily/compile/compile.c
    src/command/lily/cpp/cpp.c
    src/command/lily/init/init.c
    src/command/lily/new/new.c
    src/command/lily/run/run.c
    src/command/lily/test/test.c
    src/command/lily/to/to.c)

add_library(lily_command STATIC ${COMMAND_LILY_SRC})
target_link_libraries(lily_command PUBLIC lily_base)
target_include_directories(lily_command PRIVATE ${LILY_INCLUDE})

# lilyc_command
set(COMMAND_LILYC_SRC src/command/lilyc/lilyc.c)

add_library(lilyc_command STATIC ${COMMAND_LILYC_SRC})
target_link_libraries(lilyc_command PUBLIC lily_core_lily_compiler_package)
target_include_directories(lilyc_command PRIVATE ${LILY_INCLUDE})

# lily_core_cc_diagnostic
set(LILY_CORE_CC_DIAGNOSTIC_SRC src/core/cc/diagnostic/error.c
                                src/core/cc/diagnostic/warning.c)

add_library(lily_core_cc_diagnostic STATIC ${LILY_CORE_CC_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_cc_diagnostic PUBLIC lily_core_shared)
target_include_directories(lily_core_cc_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_cpp_diagnostic
set(LILY_CORE_CPP_DIAGNOSTIC_SRC src/core/cpp/diagnostic/error.c
                                 src/core/cpp/diagnostic/warning.c)

add_library(lily_core_cpp_diagnostic STATIC ${LILY_CORE_CPP_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_cpp_diagnostic PUBLIC lily_core_shared)
target_include_directories(lily_core_cpp_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_lily_analysis
set(LILY_CORE_LILY_ANALYSIS_SRC
    src/core/lily/analysis/checked/body/class.c
    src/core/lily/analysis/checked/body/enum_object.c
    src/core/lily/analysis/checked/body/fun.c
    src/core/lily/analysis/checked/body/record_object.c
    src/core/lily/analysis/checked/body/trait.c
    src/core/lily/analysis/checked/decl/alias.c
    src/core/lily/analysis/checked/decl/attribute.c
    src/core/lily/analysis/checked/decl/class.c
    src/core/lily/analysis/checked/decl/constant.c
    src/core/lily/analysis/checked/decl/enum.c
    src/core/lily/analysis/checked/decl/enum_object.c
    src/core/lily/analysis/checked/decl/error.c
    src/core/lily/analysis/checked/decl/fun.c
    src/core/lily/analysis/checked/decl/method.c
    src/core/lily/analysis/checked/decl/module.c
    src/core/lily/analysis/checked/decl/object.c
    src/core/lily/analysis/checked/decl/prototype.c
    src/core/lily/analysis/checked/decl/record.c
    src/core/lily/analysis/checked/decl/record_object.c
    src/core/lily/analysis/checked/decl/trait.c
    src/core/lily/analysis/checked/decl/type.c
    src/core/lily/analysis/checked/expr/array.c
    src/core/lily/analysis/checked/expr/binary.c
    src/core/lily/analysis/checked/expr/call.c
    src/core/lily/analysis/checked/expr/cast.c
    src/core/lily/analysis/checked/expr/lambda.c
    src/core/lily/analysis/checked/expr/list.c
    src/core/lily/analysis/checked/expr/literal.c
    src/core/lily/analysis/checked/expr/tuple.c
    src/core/lily/analysis/checked/expr/unary.c
    src/core/lily/analysis/checked/pattern/array.c
    src/core/lily/analysis/checked/pattern/as.c
    src/core/lily/analysis/checked/pattern/error.c
    src/core/lily/analysis/checked/pattern/list_head.c
    src/core/lily/analysis/checked/pattern/list_tail.c
    src/core/lily/analysis/checked/pattern/list.c
    src/core/lily/analysis/checked/pattern/literal.c
    src/core/lily/analysis/checked/pattern/name.c
    src/core/lily/analysis/checked/pattern/range.c
    src/core/lily/analysis/checked/pattern/record_call.c
    src/core/lily/analysis/checked/pattern/tuple.c
    src/core/lily/analysis/checked/pattern/variant_call.c
    src/core/lily/analysis/checked/stmt/asm.c
    src/core/lily/analysis/checked/stmt/await.c
    src/core/lily/analysis/checked/stmt/block.c
    src/core/lily/analysis/checked/stmt/break.c
    src/core/lily/analysis/checked/stmt/drop.c
    src/core/lily/analysis/checked/stmt/for.c
    src/core/lily/analysis/checked/stmt/if.c
    src/core/lily/analysis/checked/stmt/match.c
    src/core/lily/analysis/checked/stmt/next.c
    src/core/lily/analysis/checked/stmt/raise.c
    src/core/lily/analysis/checked/stmt/return.c
    src/core/lily/analysis/checked/stmt/try.c
    src/core/lily/analysis/checked/stmt/unsafe.c
    src/core/lily/analysis/checked/stmt/variable.c
    src/core/lily/analysis/checked/stmt/while.c
    src/core/lily/analysis/checked/access.c
    src/core/lily/analysis/checked/captured_variable.c
    src/core/lily/analysis/checked/compiler_generic.c
    src/core/lily/analysis/checked/data_type.c
    src/core/lily/analysis/checked/decl.c
    src/core/lily/analysis/checked/expr.c
    src/core/lily/analysis/checked/field.c
    src/core/lily/analysis/checked/field_object.c
    src/core/lily/analysis/checked/generic_param.c
    src/core/lily/analysis/checked/global_name.c
    src/core/lily/analysis/checked/history.c
    src/core/lily/analysis/checked/impl_param.c
    src/core/lily/analysis/checked/inherit_param.c
    src/core/lily/analysis/checked/operator.c
    src/core/lily/analysis/checked/operator_register.c
    src/core/lily/analysis/checked/parent.c
    src/core/lily/analysis/checked/pattern.c
    src/core/lily/analysis/checked/safety_mode.c
    src/core/lily/analysis/checked/scope.c
    src/core/lily/analysis/checked/scope_container.c
    src/core/lily/analysis/checked/scope_decls.c
    src/core/lily/analysis/checked/scope_response.c
    src/core/lily/analysis/checked/scope_stmt.c
    src/core/lily/analysis/checked/signature.c
    src/core/lily/analysis/checked/stmt.c
    src/core/lily/analysis/checked/variant.c
    src/core/lily/analysis/checked/virtual_scope.c
    src/core/lily/analysis/analysis.c)

add_library(lily_core_lily_analysis STATIC ${LILY_CORE_LILY_ANALYSIS_SRC})
target_link_libraries(lily_core_lily_analysis PUBLIC lily_core_lily_functions)
target_include_directories(lily_core_lily_analysis PRIVATE ${LILY_INCLUDE})

# lily_core_lily_bytecode set(LILY_CORE_LILY_BYTECODE_SRC)

# add_library(lily_core_lily_bytecode STATIC ${ LILY_CORE_LILY_BYTECODE_SRC })
# target_include_directories(lily_core_lily_bytecode PRIVATE include)

# lily_core_lily_compiler add_library(LilyCoreLilyCompiler STATIC)

# lily_core_lily_compiler_ir
set(LILY_CORE_LILY_COMPILER_IR_SRC
    src/core/lily/compiler/ir/cc.c src/core/lily/compiler/ir/cpp.c
    src/core/lily/compiler/ir/ir.c src/core/lily/compiler/ir/js.c
    src/core/lily/compiler/ir/llvm.c)

add_library(lily_core_lily_compiler_ir STATIC ${LILY_CORE_LILY_COMPILER_IR_SRC})
target_link_libraries(
  lily_core_lily_compiler_ir
  PUBLIC lily_core_lily_compiler_ir_cc lily_core_lily_compiler_ir_cpp
         lily_core_lily_compiler_ir_llvm)
target_include_directories(lily_core_lily_compiler_ir PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_cc
set(LILY_CORE_LILY_COMPILER_IR_CC_SRC
    src/core/lily/compiler/ir/cc/builder/constant.c
    src/core/lily/compiler/ir/cc/builder/enum.c
    src/core/lily/compiler/ir/cc/builder/function/condition.c
    src/core/lily/compiler/ir/cc/builder/function/function.c
    src/core/lily/compiler/ir/cc/builder/function/loop.c
    src/core/lily/compiler/ir/cc/builder/function/switch.c
    src/core/lily/compiler/ir/cc/builder/function/variable.c
    src/core/lily/compiler/ir/cc/builder/macro.c
    src/core/lily/compiler/ir/cc/builder/struct.c
    src/core/lily/compiler/ir/cc/builder/typedef.c
    src/core/lily/compiler/ir/cc/builder/union.c
    src/core/lily/compiler/ir/cc/builder.c
    src/core/lily/compiler/ir/cc/generator/alias.c
    src/core/lily/compiler/ir/cc/generator/class.c
    src/core/lily/compiler/ir/cc/generator/constant.c
    src/core/lily/compiler/ir/cc/generator/enum.c
    src/core/lily/compiler/ir/cc/generator/enum_object.c
    src/core/lily/compiler/ir/cc/generator/error.c
    src/core/lily/compiler/ir/cc/generator/function.c
    src/core/lily/compiler/ir/cc/generator/macro.c
    src/core/lily/compiler/ir/cc/generator/record.c
    src/core/lily/compiler/ir/cc/generator/record_object.c
    src/core/lily/compiler/ir/cc/generator.c)

add_library(lily_core_lily_compiler_ir_cc STATIC
            ${LILY_CORE_LILY_COMPILER_IR_CC_SRC})
target_include_directories(lily_core_lily_compiler_ir_cc
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_cpp
set(LILY_CORE_LILY_COMPILER_IR_CPP_SRC
    src/core/lily/compiler/ir/cpp/builder/class/attribute.c
    src/core/lily/compiler/ir/cpp/builder/class/class.c
    src/core/lily/compiler/ir/cpp/builder/class/method.c
    src/core/lily/compiler/ir/cpp/builder/constant.c
    src/core/lily/compiler/ir/cpp/builder/function/condition.c
    src/core/lily/compiler/ir/cpp/builder/function/function.c
    src/core/lily/compiler/ir/cpp/builder/function/loop.c
    src/core/lily/compiler/ir/cpp/builder/function/switch.c
    src/core/lily/compiler/ir/cpp/builder/function/variable.c
    src/core/lily/compiler/ir/cpp/builder/namespace.c
    src/core/lily/compiler/ir/cpp/builder/struct.c
    src/core/lily/compiler/ir/cpp/builder/typedef.c
    src/core/lily/compiler/ir/cpp/builder/union.c
    src/core/lily/compiler/ir/cpp/builder.c
    src/core/lily/compiler/ir/cpp/generator/alias.c
    src/core/lily/compiler/ir/cpp/generator/class.c
    src/core/lily/compiler/ir/cpp/generator/constant.c
    src/core/lily/compiler/ir/cpp/generator/enum.c
    src/core/lily/compiler/ir/cpp/generator/enum_object.c
    src/core/lily/compiler/ir/cpp/generator/error.c
    src/core/lily/compiler/ir/cpp/generator/function.c
    src/core/lily/compiler/ir/cpp/generator/macro.c
    src/core/lily/compiler/ir/cpp/generator/record.c
    src/core/lily/compiler/ir/cpp/generator/record_object.c
    src/core/lily/compiler/ir/cpp/generator.c)

add_library(lily_core_lily_compiler_ir_cpp STATIC
            ${LILY_CORE_LILY_COMPILER_IR_CPP_SRC})
target_include_directories(lily_core_lily_compiler_ir_cpp
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_ir_js

# lily_core_lily_compiler_ir_llvm
set(LILY_CORE_LILY_COMPILER_IR_LLVM_SRC
    src/core/lily/compiler/ir/llvm/generator/dt.c
    src/core/lily/compiler/ir/llvm/generator/inst.c
    src/core/lily/compiler/ir/llvm/generator/val.c
    src/core/lily/compiler/ir/llvm/generator.c
    src/core/lily/compiler/ir/llvm/lily.c
    src/core/lily/compiler/ir/llvm/linker.c
    src/core/lily/compiler/ir/llvm/pending.c
    src/core/lily/compiler/ir/llvm/scope.c)

add_library(lily_core_lily_compiler_ir_llvm STATIC
            ${LILY_CORE_LILY_COMPILER_IR_LLVM_SRC})
target_link_libraries(lily_core_lily_compiler_ir_llvm lily_base LLVM)
target_include_directories(lily_core_lily_compiler_ir_llvm
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_linker
set(LILY_CORE_LILY_COMPILER_LINKER_SRC src/core/lily/compiler/linker/linker.c)

add_library(lily_core_lily_compiler_linker STATIC
            ${LILY_CORE_LILY_COMPILER_LINKER_SRC})
target_link_libraries(lily_core_lily_compiler_linker
                      PUBLIC lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_compiler_linker
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_compiler_package
set(LILY_CORE_LILY_COMPILER_PACKAGE_SRC
    src/core/lily/compiler/package/config.c
    src/core/lily/compiler/package/default_path.c
    src/core/lily/compiler/package/dependency_tree.c
    src/core/lily/compiler/package/library.c
    src/core/lily/compiler/package/package.c
    src/core/lily/compiler/package/program.c)

add_library(lily_core_lily_compiler_package STATIC
            ${LILY_CORE_LILY_COMPILER_PACKAGE_SRC})
target_link_libraries(
  lily_core_lily_compiler_package lily_core_lily_compiler_ir
  lily_core_lily_compiler_linker lily_core_lily_mir lily_core_lily_parser
  lily_core_lily_precompiler)
target_include_directories(lily_core_lily_compiler_package
                           PRIVATE ${LILY_INCLUDE})

# lily_core_lily_diagnostic
set(LILY_CORE_LILY_DIAGNOSTIC_SRC src/core/lily/diagnostic/error.c
                                  src/core/lily/diagnostic/warning.c)

add_library(lily_core_lily_diagnostic STATIC ${LILY_CORE_LILY_DIAGNOSTIC_SRC})
target_link_libraries(lily_core_lily_diagnostic PUBLIC lily_core_shared
                                                       lily_base)
target_include_directories(lily_core_lily_diagnostic PRIVATE ${LILY_INCLUDE})

# lily_core_lily_functions
set(LILY_CORE_LILY_FUNCTIONS_SRC src/core/lily/functions/builtin.c
                                 src/core/lily/functions/sys.c)

add_library(lily_core_lily_functions STATIC ${LILY_CORE_LILY_FUNCTIONS_SRC})
target_link_libraries(lily_core_lily_functions PUBLIC lily_core_lily_analysis)
target_include_directories(lily_core_lily_functions PRIVATE ${LILY_INCLUDE})

# lily_core_lily_jit

# lily_core_lily_mir
set(LILY_CORE_LILY_MIR_SRC
    src/core/lily/mir/generator/constant.c
    src/core/lily/mir/generator/dt.c
    src/core/lily/mir/generator/enum.c
    src/core/lily/mir/generator/expr/assignable.c
    src/core/lily/mir/generator/expr/binary.c
    src/core/lily/mir/generator/expr/call.c
    src/core/lily/mir/generator/expr/unary.c
    src/core/lily/mir/generator/expr.c
    src/core/lily/mir/generator/fun.c
    src/core/lily/mir/generator/record.c
    src/core/lily/mir/generator/stmt.c
    src/core/lily/mir/generator/val.c
    src/core/lily/mir/generator/stmt/variable.c
    src/core/lily/mir/dt.c
    src/core/lily/mir/instruction.c
    src/core/lily/mir/linkage.c
    src/core/lily/mir/generator.c
    src/core/lily/mir/mir.c
    src/core/lily/mir/scope.c)

add_library(lily_core_lily_mir STATIC ${LILY_CORE_LILY_MIR_SRC})
target_link_libraries(lily_core_lily_mir PUBLIC lily_base
                                                lily_core_lily_analysis)
target_include_directories(lily_core_lily_mir PRIVATE ${LILY_INCLUDE})

# lily_core_lily_parser
set(LILY_CORE_LILY_PARSER_SRC
    src/core/lily/parser/ast/body/class.c
    src/core/lily/parser/ast/body/enum_object.c
    src/core/lily/parser/ast/body/fun.c
    src/core/lily/parser/ast/body/record_object.c
    src/core/lily/parser/ast/body/trait.c
    src/core/lily/parser/ast/decl/alias.c
    src/core/lily/parser/ast/decl/attribute.c
    src/core/lily/parser/ast/decl/class.c
    src/core/lily/parser/ast/decl/constant.c
    src/core/lily/parser/ast/decl/enum.c
    src/core/lily/parser/ast/decl/enum_object.c
    src/core/lily/parser/ast/decl/error.c
    src/core/lily/parser/ast/decl/fun.c
    src/core/lily/parser/ast/decl/method.c
    src/core/lily/parser/ast/decl/module.c
    src/core/lily/parser/ast/decl/object.c
    src/core/lily/parser/ast/decl/prototype.c
    src/core/lily/parser/ast/decl/record.c
    src/core/lily/parser/ast/decl/record_object.c
    src/core/lily/parser/ast/decl/trait.c
    src/core/lily/parser/ast/decl/type.c
    src/core/lily/parser/ast/expr/access.c
    src/core/lily/parser/ast/expr/array.c
    src/core/lily/parser/ast/expr/binary.c
    src/core/lily/parser/ast/expr/call.c
    src/core/lily/parser/ast/expr/cast.c
    src/core/lily/parser/ast/expr/identifier.c
    src/core/lily/parser/ast/expr/lambda.c
    src/core/lily/parser/ast/expr/list.c
    src/core/lily/parser/ast/expr/literal.c
    src/core/lily/parser/ast/expr/try.c
    src/core/lily/parser/ast/expr/tuple.c
    src/core/lily/parser/ast/expr/unary.c
    src/core/lily/parser/ast/pattern/array.c
    src/core/lily/parser/ast/pattern/as.c
    src/core/lily/parser/ast/pattern/error.c
    src/core/lily/parser/ast/pattern/list.c
    src/core/lily/parser/ast/pattern/list_head.c
    src/core/lily/parser/ast/pattern/list_tail.c
    src/core/lily/parser/ast/pattern/literal.c
    src/core/lily/parser/ast/pattern/name.c
    src/core/lily/parser/ast/pattern/range.c
    src/core/lily/parser/ast/pattern/record_call.c
    src/core/lily/parser/ast/pattern/tuple.c
    src/core/lily/parser/ast/pattern/variant_call.c
    src/core/lily/parser/ast/stmt/asm.c
    src/core/lily/parser/ast/stmt/await.c
    src/core/lily/parser/ast/stmt/block.c
    src/core/lily/parser/ast/stmt/break.c
    src/core/lily/parser/ast/stmt/defer.c
    src/core/lily/parser/ast/stmt/drop.c
    src/core/lily/parser/ast/stmt/for.c
    src/core/lily/parser/ast/stmt/if.c
    src/core/lily/parser/ast/stmt/match.c
    src/core/lily/parser/ast/stmt/next.c
    src/core/lily/parser/ast/stmt/raise.c
    src/core/lily/parser/ast/stmt/return.c
    src/core/lily/parser/ast/stmt/try.c
    src/core/lily/parser/ast/stmt/unsafe.c
    src/core/lily/parser/ast/stmt/variable.c
    src/core/lily/parser/ast/stmt/while.c
    src/core/lily/parser/ast/data_type.c
    src/core/lily/parser/ast/decl.c
    src/core/lily/parser/ast/expr.c
    src/core/lily/parser/ast/field.c
    src/core/lily/parser/ast/field_object.c
    src/core/lily/parser/ast/generic_param.c
    src/core/lily/parser/ast/impl_param.c
    src/core/lily/parser/ast/inherit_param.c
    src/core/lily/parser/ast/pattern.c
    src/core/lily/parser/ast/stmt.c
    src/core/lily/parser/ast/variant.c
    src/core/lily/parser/parser.c)

add_library(lily_core_lily_parser STATIC ${LILY_CORE_LILY_PARSER_SRC})
target_link_libraries(lily_core_lily_parser PUBLIC lily_core_lily_preparser
                                                   lily_core_lily_shared)
target_include_directories(lily_core_lily_parser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_precompiler
set(LILY_CORE_LILY_PRECOMPILER_SRC src/core/lily/precompiler/precompiler.c)

add_library(lily_core_lily_precompiler STATIC ${LILY_CORE_LILY_PRECOMPILER_SRC})
target_link_libraries(lily_core_lily_precompiler
                      PUBLIC lily_core_lily_compiler_package)
target_include_directories(lily_core_lily_precompiler PRIVATE ${LILY_INCLUDE})

# lily_core_lily_preparser
set(LILY_CORE_LILY_PREPARSER_SRC
    src/core/lily/preparser/preprocess/allow.c
    src/core/lily/preparser/preprocess/arch.c
    src/core/lily/preparser/preprocess/deny.c
    src/core/lily/preparser/preprocess/doc.c
    src/core/lily/preparser/preprocess/forbid.c
    src/core/lily/preparser/preprocess/link_info.c
    src/core/lily/preparser/preprocess/os.c
    src/core/lily/preparser/preprocess/repr.c
    src/core/lily/preparser/preprocess/warn.c
    src/core/lily/preparser/preparser.c
    src/core/lily/preparser/preprocess.c)

add_library(lily_core_lily_preparser STATIC ${LILY_CORE_LILY_PREPARSER_SRC})
target_link_libraries(lily_core_lily_preparser PUBLIC lily_core_lily_scanner)
target_include_directories(lily_core_lily_preparser PRIVATE ${LILY_INCLUDE})

# lily_core_lily_scanner
set(LILY_CORE_LILY_SCANNER_SRC src/core/lily/scanner/scanner.c
                               src/core/lily/scanner/token.c)

add_library(lily_core_lily_scanner STATIC ${LILY_CORE_LILY_SCANNER_SRC})
target_link_libraries(lily_core_lily_scanner PUBLIC lily_base lily_core_shared)
target_include_directories(lily_core_lily_scanner PRIVATE ${LILY_INCLUDE})

# lily_core_lily_shared
set(LILY_CORE_LILY_SHARED_SRC src/core/lily/shared/visibility.c)

add_library(lily_core_lily_shared STATIC ${LILY_CORE_LILY_SHARED_SRC})
target_include_directories(lily_core_lily_shared PRIVATE ${LILY_INCLUDE})

# lily_core_shared
set(LILY_CORE_SHARED
    src/core/shared/target/arch.c src/core/shared/target/os.c
    src/core/shared/cursor.c src/core/shared/diagnostic.c
    src/core/shared/location.c src/core/shared/source.c)

add_library(lily_core_shared STATIC ${LILY_CORE_SHARED})
target_link_libraries(lily_core_shared lily_base lily_core_lily_diagnostic
                      lily_core_cc_diagnostic lily_core_cpp_diagnostic)
target_include_directories(lily_core_shared PRIVATE ${LILY_INCLUDE})

add_executable(lily src/bin/lily/main.c)
target_link_libraries(lily lily_cli lily_command)
target_include_directories(lily PRIVATE ${LILY_INCLUDE})

add_executable(lilyc src/bin/lilyc/main.c)
target_link_libraries(lilyc lilyc_cli lilyc_command)
target_include_directories(lilyc PRIVATE ${LILY_INCLUDE})

# lily_builtin
set(BUILTIN_SRC lib/builtin/alloc.c lib/builtin/cstr.c lib/builtin/math.c
                lib/builtin/pointer.c)

add_library(lily_builtin SHARED ${BUILTIN_SRC})
target_include_directories(lily_builtin PUBLIC lib include)

# lily_sys
set(SYS_SRC lib/sys/sys.c)

add_library(lily_sys SHARED ${SYS_SRC})
target_include_directories(lily_sys PUBLIC lib include)

function(move_tests_dir)
  file(COPY tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  message("Move `tests` directory in build directory")
endfunction()

if(LILY_DEBUG)
  if(EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tests)
    file(REMOVE tests)
    move_tests_dir()
  else()
    move_tests_dir()
  endif()

  enable_testing()

  add_executable(test_base tests/base/base.c)
  target_link_libraries(test_base lily_base)
  target_include_directories(test_base PRIVATE include)

  add_test(NAME test_base COMMAND test_base)

  add_executable(test_core_scanner tests/core/lily/scanner/scanner.c)
  target_link_libraries(test_core_scanner lily_base lily_core_lily_scanner)
  target_include_directories(test_core_scanner PRIVATE include)

  add_test(NAME test_core_scanner COMMAND test_core_scanner)

  add_executable(test_core_preparser tests/core/lily/preparser/preparser.c)
  target_link_libraries(test_core_preparser lily_base lily_core_lily_preparser)
  target_include_directories(test_core_preparser PRIVATE include)

  add_test(NAME test_core_preparser COMMAND test_core_preparser)

  add_executable(test_core_precompile tests/core/lily/precompile/precompile.c)
  target_link_libraries(test_core_precompile lily_base
                        lily_core_lily_precompiler)
  target_include_directories(test_core_precompile PRIVATE include)

  add_test(NAME test_core_precompile COMMAND test_core_precompile)

  add_executable(test_core_parser tests/core/lily/parser/parser.c)
  target_link_libraries(
    test_core_parser lily_base lily_core_lily_parser lily_core_lily_preparser
    lily_core_lily_compiler_package)
  target_include_directories(test_core_parser PRIVATE include)

  add_test(NAME test_core_parser COMMAND test_core_parser)

  add_executable(bench_vec benchmarks/base/vec.c)
  target_link_libraries(bench_vec lily_base)
  target_include_directories(bench_vec PRIVATE include)

  add_executable(bench_string benchmarks/base/string.c)
  target_link_libraries(bench_string lily_base)
  target_include_directories(bench_string PRIVATE include)

  add_executable(bench_hash_map benchmarks/base/hash_map.c)
  target_link_libraries(bench_hash_map lily_base)
  target_include_directories(bench_hash_map PRIVATE include)
endif()
