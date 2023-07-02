cmake_minimum_required(VERSION 3.20)
project(lily
	VERSION 0.0.0
	HOMEPAGE_URL "https://github.com/ArthurPV/lily"
	LANGUAGES C CXX)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_subdirectory(src/core/cc/comptime_gen)

if (NOT WIN32)
	add_compile_options(-Wall -O3 -fdiagnostics-color=always)
endif()

if (CMAKE_C_COMPILER_ID STREQUAL "MSVC")
  	set(CMAKE_C_FLAGS "/wd4710 /wd4711 /wd4255") 
endif()

if(UNIX AND NOT APPLE)
	set(LILY_LINUX 1)
endif()

find_package(LLVM 15 CONFIG REQUIRED)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")

set(BASE_SRC
	src/base/alloc.c
	src/base/atof.c
	src/base/atoi.c
	src/base/cli/command.c
	src/base/cli/default_action.c
	src/base/cli/diagnostic.c
	src/base/cli/option.c
	src/base/cli/result/command.c
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

set(CLI_SRC
	src/cli/option.c
	src/cli/parse_command.c
	src/cli/parse_config.c
	src/cli/option/build.c
	src/cli/option/cc.c
	src/cli/option/compile.c
	src/cli/option/cpp.c
	src/cli/option/init.c
	src/cli/option/new.c
	src/cli/option/run.c
	src/cli/option/test.c
	src/cli/option/to.c)

set(COMMAND_SRC
	src/command/build/build.c
	src/command/cc/cc.c
	src/command/compile/compile.c
	src/command/cpp/cpp.c
	src/command/init/init.c
	src/command/new/new.c
	src/command/run/run.c
	src/command/test/test.c
	src/command/to/to.c)

set(CORE_SRC
	src/core/cc/error.c
	src/core/cc/token.c
	src/core/cc/warning.c
	src/core/cpp/error.c
	src/core/cpp/warning.c
	src/core/lily/ast/body/class.c
	src/core/lily/ast/body/enum_object.c
	src/core/lily/ast/body/fun.c
	src/core/lily/ast/body/record_object.c
	src/core/lily/ast/body/trait.c
	src/core/lily/ast/decl/alias.c
	src/core/lily/ast/decl/attribute.c
	src/core/lily/ast/decl/class.c
	src/core/lily/ast/decl/constant.c
	src/core/lily/ast/decl/enum.c
	src/core/lily/ast/decl/enum_object.c
	src/core/lily/ast/decl/error.c
	src/core/lily/ast/decl/fun.c
	src/core/lily/ast/decl/method.c
	src/core/lily/ast/decl/module.c
	src/core/lily/ast/decl/object.c
	src/core/lily/ast/decl/prototype.c
	src/core/lily/ast/decl/record.c
	src/core/lily/ast/decl/record_object.c
	src/core/lily/ast/decl/trait.c
	src/core/lily/ast/decl/type.c
	src/core/lily/ast/expr/access.c
	src/core/lily/ast/expr/array.c
	src/core/lily/ast/expr/binary.c
	src/core/lily/ast/expr/call.c
	src/core/lily/ast/expr/cast.c
	src/core/lily/ast/expr/identifier.c
	src/core/lily/ast/expr/lambda.c
	src/core/lily/ast/expr/list.c
	src/core/lily/ast/expr/literal.c
	src/core/lily/ast/expr/try.c
	src/core/lily/ast/expr/tuple.c
	src/core/lily/ast/expr/unary.c
	src/core/lily/ast/pattern/array.c
	src/core/lily/ast/pattern/as.c
	src/core/lily/ast/pattern/exception.c
	src/core/lily/ast/pattern/list.c
	src/core/lily/ast/pattern/list_head.c
	src/core/lily/ast/pattern/list_tail.c
	src/core/lily/ast/pattern/literal.c
	src/core/lily/ast/pattern/name.c
	src/core/lily/ast/pattern/range.c
	src/core/lily/ast/pattern/record_call.c
	src/core/lily/ast/pattern/tuple.c
	src/core/lily/ast/pattern/variant_call.c
	src/core/lily/ast/stmt/asm.c
	src/core/lily/ast/stmt/await.c
	src/core/lily/ast/stmt/block.c
	src/core/lily/ast/stmt/break.c
	src/core/lily/ast/stmt/defer.c
	src/core/lily/ast/stmt/drop.c
	src/core/lily/ast/stmt/for.c
	src/core/lily/ast/stmt/if.c
	src/core/lily/ast/stmt/match.c
	src/core/lily/ast/stmt/next.c
	src/core/lily/ast/stmt/raise.c
	src/core/lily/ast/stmt/return.c
	src/core/lily/ast/stmt/try.c
	src/core/lily/ast/stmt/unsafe.c
	src/core/lily/ast/stmt/variable.c
	src/core/lily/ast/stmt/while.c
	src/core/lily/ast/data_type.c
	src/core/lily/ast/decl.c
	src/core/lily/ast/expr.c
	src/core/lily/ast/field.c
	src/core/lily/ast/field_object.c
	src/core/lily/ast/generic_param.c
	src/core/lily/ast/impl_param.c
	src/core/lily/ast/inherit_param.c
	src/core/lily/ast/pattern.c
	src/core/lily/ast/stmt.c
	src/core/lily/ast/variant.c
	src/core/lily/checked/body/class.c
	src/core/lily/checked/body/enum_object.c
	src/core/lily/checked/body/fun.c
	src/core/lily/checked/body/record_object.c
	src/core/lily/checked/body/trait.c
	src/core/lily/checked/decl/alias.c
	src/core/lily/checked/decl/attribute.c
	src/core/lily/checked/decl/class.c
	src/core/lily/checked/decl/constant.c
	src/core/lily/checked/decl/enum.c
	src/core/lily/checked/decl/enum_object.c
	src/core/lily/checked/decl/error.c
	src/core/lily/checked/decl/fun.c
	src/core/lily/checked/decl/method.c
	src/core/lily/checked/decl/module.c
	src/core/lily/checked/decl/object.c
	src/core/lily/checked/decl/prototype.c
	src/core/lily/checked/decl/record.c
	src/core/lily/checked/decl/record_object.c
	src/core/lily/checked/decl/trait.c
	src/core/lily/checked/decl/type.c
	src/core/lily/checked/expr/array.c
	src/core/lily/checked/expr/binary.c
	src/core/lily/checked/expr/call.c
	src/core/lily/checked/expr/cast.c
	src/core/lily/checked/expr/lambda.c
	src/core/lily/checked/expr/list.c
	src/core/lily/checked/expr/literal.c
	src/core/lily/checked/expr/tuple.c
	src/core/lily/checked/expr/unary.c
	src/core/lily/checked/pattern/array.c
	src/core/lily/checked/pattern/as.c
	src/core/lily/checked/pattern/exception.c
	src/core/lily/checked/pattern/list_head.c
	src/core/lily/checked/pattern/list_tail.c
	src/core/lily/checked/pattern/list.c
	src/core/lily/checked/pattern/literal.c
	src/core/lily/checked/pattern/name.c
	src/core/lily/checked/pattern/range.c
	src/core/lily/checked/pattern/record_call.c
	src/core/lily/checked/pattern/tuple.c
	src/core/lily/checked/pattern/variant_call.c
	src/core/lily/checked/stmt/asm.c
	src/core/lily/checked/stmt/await.c
	src/core/lily/checked/stmt/block.c
	src/core/lily/checked/stmt/break.c
	src/core/lily/checked/stmt/drop.c
	src/core/lily/checked/stmt/for.c
	src/core/lily/checked/stmt/if.c
	src/core/lily/checked/stmt/match.c
	src/core/lily/checked/stmt/next.c
	src/core/lily/checked/stmt/raise.c
	src/core/lily/checked/stmt/return.c
	src/core/lily/checked/stmt/try.c
	src/core/lily/checked/stmt/unsafe.c
	src/core/lily/checked/stmt/variable.c
	src/core/lily/checked/stmt/while.c
	src/core/lily/checked/access.c
	src/core/lily/checked/compiler_generic.c
	src/core/lily/checked/data_type.c
	src/core/lily/checked/decl.c
	src/core/lily/checked/drop_table.c
	src/core/lily/checked/expr.c
	src/core/lily/checked/field.c
	src/core/lily/checked/field_object.c
	src/core/lily/checked/generic_param.c
	src/core/lily/checked/global_name.c
	src/core/lily/checked/history.c
	src/core/lily/checked/impl_param.c
	src/core/lily/checked/inherit_param.c
	src/core/lily/checked/operator.c
	src/core/lily/checked/operator_register.c
	src/core/lily/checked/parent.c
	src/core/lily/checked/pattern.c
	src/core/lily/checked/safety_mode.c
	src/core/lily/checked/scope.c
	src/core/lily/checked/scope_container.c
	src/core/lily/checked/scope_decls.c
	src/core/lily/checked/scope_response.c
	src/core/lily/checked/signature.c
	src/core/lily/checked/stmt.c
	src/core/lily/checked/variant.c
	src/core/lily/checked/virtual_scope.c
	src/core/lily/ir/cc/builder/constant.c
	src/core/lily/ir/cc/builder/enum.c
	src/core/lily/ir/cc/builder/function/condition.c
	src/core/lily/ir/cc/builder/function/function.c
	src/core/lily/ir/cc/builder/function/loop.c
	src/core/lily/ir/cc/builder/function/switch.c
	src/core/lily/ir/cc/builder/function/variable.c
	src/core/lily/ir/cc/builder/macro.c
	src/core/lily/ir/cc/builder/struct.c
	src/core/lily/ir/cc/builder/typedef.c
	src/core/lily/ir/cc/builder/union.c
	src/core/lily/ir/cc/builder.c
	src/core/lily/ir/cc/generator/alias.c
	src/core/lily/ir/cc/generator/class.c
	src/core/lily/ir/cc/generator/constant.c
	src/core/lily/ir/cc/generator/enum.c
	src/core/lily/ir/cc/generator/enum_object.c
	src/core/lily/ir/cc/generator/error.c
	src/core/lily/ir/cc/generator/function.c
	src/core/lily/ir/cc/generator/macro.c
	src/core/lily/ir/cc/generator/record.c
	src/core/lily/ir/cc/generator/record_object.c
	src/core/lily/ir/cc/generator.c
	src/core/lily/ir/cpp/builder/class/attribute.c
	src/core/lily/ir/cpp/builder/class/class.c
	src/core/lily/ir/cpp/builder/class/method.c
	src/core/lily/ir/cpp/builder/constant.c
	src/core/lily/ir/cpp/builder/function/condition.c
	src/core/lily/ir/cpp/builder/function/function.c
	src/core/lily/ir/cpp/builder/function/loop.c
	src/core/lily/ir/cpp/builder/function/switch.c
	src/core/lily/ir/cpp/builder/function/variable.c
	src/core/lily/ir/cpp/builder/namespace.c
	src/core/lily/ir/cpp/builder/struct.c
	src/core/lily/ir/cpp/builder/typedef.c
	src/core/lily/ir/cpp/builder/union.c
	src/core/lily/ir/cpp/builder.c
	src/core/lily/ir/cpp/generator/alias.c
	src/core/lily/ir/cpp/generator/class.c
	src/core/lily/ir/cpp/generator/constant.c
	src/core/lily/ir/cpp/generator/enum.c
	src/core/lily/ir/cpp/generator/enum_object.c
	src/core/lily/ir/cpp/generator/error.c
	src/core/lily/ir/cpp/generator/function.c
	src/core/lily/ir/cpp/generator/macro.c
	src/core/lily/ir/cpp/generator/record.c
	src/core/lily/ir/cpp/generator/record_object.c
	src/core/lily/ir/cpp/generator.c
	src/core/lily/ir/llvm/generator/builtin.c
	src/core/lily/ir/llvm/generator/constant.c
	src/core/lily/ir/llvm/generator/data_type.c
	src/core/lily/ir/llvm/generator/error.c
	src/core/lily/ir/llvm/generator/expr/array.c
	src/core/lily/ir/llvm/generator/expr/binary.c
	src/core/lily/ir/llvm/generator/expr/call.c
	src/core/lily/ir/llvm/generator/expr/cast.c
	src/core/lily/ir/llvm/generator/expr/lambda.c
	src/core/lily/ir/llvm/generator/expr/list.c
	src/core/lily/ir/llvm/generator/expr/literal.c
	src/core/lily/ir/llvm/generator/expr/tuple.c
	src/core/lily/ir/llvm/generator/expr/unary.c
	src/core/lily/ir/llvm/generator/expr.c
	src/core/lily/ir/llvm/generator/function.c
	src/core/lily/ir/llvm/generator/main.c
	src/core/lily/ir/llvm/generator/object/class.c
	src/core/lily/ir/llvm/generator/object/enum_object.c
	src/core/lily/ir/llvm/generator/object/record_object.c
	src/core/lily/ir/llvm/generator/stmt/asm.c
	src/core/lily/ir/llvm/generator/stmt/await.c
	src/core/lily/ir/llvm/generator/stmt/block.c
	src/core/lily/ir/llvm/generator/stmt/break.c
	src/core/lily/ir/llvm/generator/stmt/drop.c
	src/core/lily/ir/llvm/generator/stmt/for.c
	src/core/lily/ir/llvm/generator/stmt/if.c
	src/core/lily/ir/llvm/generator/stmt/match.c
	src/core/lily/ir/llvm/generator/stmt/next.c
	src/core/lily/ir/llvm/generator/stmt/raise.c
	src/core/lily/ir/llvm/generator/stmt/return.c
	src/core/lily/ir/llvm/generator/stmt/try.c
	src/core/lily/ir/llvm/generator/stmt/unsafe.c
	src/core/lily/ir/llvm/generator/stmt/variable.c
	src/core/lily/ir/llvm/generator/stmt/while.c
	src/core/lily/ir/llvm/generator/stmt.c
	src/core/lily/ir/llvm/generator/sys.c
	src/core/lily/ir/llvm/generator/type/enum.c
	src/core/lily/ir/llvm/generator/type/record.c
	src/core/lily/ir/llvm/generator.c
	src/core/lily/ir/llvm/linkage.c
	src/core/lily/ir/llvm/linker.c
	src/core/lily/ir/llvm/scope.c
	src/core/lily/ir/llvm/store.c
	src/core/lily/ir/llvm/types.c
	src/core/lily/ir/cc.c
	src/core/lily/ir/cpp.c
	src/core/lily/ir/js.c
	src/core/lily/ir/llvm.c
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
	src/core/lily/mir/scope.c
	src/core/lily/package/config.c
	src/core/lily/package/default_path.c
	src/core/lily/package/dependency_tree.c
	src/core/lily/package/library.c
	src/core/lily/package/package.c
	src/core/lily/preprocess/allow.c
	src/core/lily/preprocess/arch.c
	src/core/lily/preprocess/deny.c
	src/core/lily/preprocess/doc.c
	src/core/lily/preprocess/forbid.c
	src/core/lily/preprocess/link_info.c
	src/core/lily/preprocess/os.c
	src/core/lily/preprocess/repr.c
	src/core/lily/preprocess/warn.c
	src/core/lily/analysis.c
	src/core/lily/ast.c
	src/core/lily/builtin.c
	src/core/lily/checked.c
	src/core/lily/error.c
	src/core/lily/file.c
	src/core/lily/ir.c
	src/core/lily/linker.c
	src/core/lily/mir.c
	src/core/lily/package.c
	src/core/lily/parser.c
	src/core/lily/precompile.c
	src/core/lily/preparser.c
	src/core/lily/preprocess.c
	src/core/lily/program.c
	src/core/lily/scanner.c
	src/core/lily/sys.c
	src/core/lily/token.c
	src/core/lily/visibility.c
	src/core/lily/warning.c
	src/core/shared/cursor.c
	src/core/shared/diagnostic.c
	src/core/shared/location.c
	src/core/shared/target/arch.c
	src/core/shared/target/os.c
	src/core/shared/source.c)

set(BUILTIN_SRC
	lib/builtin/alloc.c
	lib/builtin/cstr.c
	lib/builtin/math.c
	lib/builtin/pointer.c)

set(SYS_SRC
	lib/sys/sys.c)

add_library(lily_base STATIC ${BASE_SRC})
target_include_directories(lily_base PUBLIC include)

add_library(lily_cli STATIC ${CLI_SRC})
target_link_libraries(lily_cli lily_base)

add_library(lily_command STATIC ${COMMAND_SRC})
target_link_libraries(lily_command lily_base lily_cli lily_core)

add_library(lily_core STATIC ${CORE_SRC})
target_link_libraries(lily_core lily_base LLVM)
target_include_directories(lily_core PUBLIC lib/local)

add_library(lily_builtin SHARED ${BUILTIN_SRC})
target_include_directories(lily_builtin PUBLIC lib include)

add_library(lily_sys SHARED ${SYS_SRC})
target_include_directories(lily_sys PUBLIC lib include)

add_executable(lily src/bin/lily/main.c)
target_link_libraries(lily lily_base lily_cli lily_command)

add_executable(lilyc src/bin/lilyc/main.c)
target_link_libraries(lilyc lily_base lily_cli)

function(move_tests_dir)
	file(COPY tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
	message("Move `tests` directory in build directory")
endfunction()

if (LILY_DEBUG)
	if (EXISTS ${CMAKE_CURRENT_BINARY_DIR}/tests)
		file(REMOVE tests)
		move_tests_dir()
	else()
		move_tests_dir()
	endif()

	enable_testing()

	add_executable(test_base tests/base/base.c)
	target_link_libraries(test_base lily_base)

	add_test(NAME test_base COMMAND test_base)

	add_executable(test_core_scanner tests/core/lily/scanner/scanner.c)
	target_link_libraries(test_core_scanner lily_base lily_core)

	add_test(NAME test_core_scanner COMMAND test_core_scanner)

	add_executable(test_core_preparser tests/core/lily/preparser/preparser.c)
	target_link_libraries(test_core_preparser lily_base lily_core)

	add_test(NAME test_core_preparser COMMAND test_core_preparser)

	add_executable(test_core_precompile tests/core/lily/precompile/precompile.c)
	target_link_libraries(test_core_precompile lily_base lily_core)

	add_test(NAME test_core_precompile COMMAND test_core_precompile)

	add_executable(test_core_parser tests/core/lily/parser/parser.c)
	target_link_libraries(test_core_parser lily_base lily_core)

	add_test(NAME test_core_parser COMMAND test_core_parser)

	add_executable(bench_vec benchmarks/base/vec.c)
	target_link_libraries(bench_vec lily_base)

	add_executable(bench_string benchmarks/base/string.c)
	target_link_libraries(bench_string lily_base)

	add_executable(bench_hash_map benchmarks/base/hash_map.c)
	target_link_libraries(bench_hash_map lily_base)
endif()
