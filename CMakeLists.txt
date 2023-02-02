cmake_minimum_required(VERSION 3.10)
project(lily)

set(DEBUG 1) # 0 for release, 1 for debug

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_compile_options(-Wall -O3)

set(BASE_SRC
	src/base/atoi.c
	src/base/color.c
	src/base/file.c
	src/base/format.c
	src/base/hash_map.c
	src/base/hash_set.c
	src/base/itoa.c
	src/base/string.c
  	src/base/vec.c)

set(CLI_SRC
	src/cli/command.c
	src/cli/emit.c
	src/cli/parse_command.c
	src/cli/parse_config.c
	src/cli/config/build.c
	src/cli/config/cc.c
	src/cli/config/compile.c
	src/cli/config/cpp.c
	src/cli/config/init.c
	src/cli/config/new.c
	src/cli/config/run.c
	src/cli/config/test.c
	src/cli/config/to.c
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

add_library(lily_base STATIC ${BASE_SRC})
target_include_directories(lily_base PUBLIC include)

add_library(lily_cli STATIC ${CLI_SRC})
target_link_libraries(lily_cli lily_base)

add_library(lily_command STATIC ${COMMAND_SRC})
target_link_libraries(lily_command lily_base)

add_executable(lily src/bin/main.c)
target_link_libraries(lily lily_base)

if (DEBUG)
	add_executable(test_base tests/base/base.c)
	target_link_libraries(test_base lily_base)

	add_test(NAME test_base COMMAND test_base)

	add_executable(bench_vec benchmarks/base/vec.c)
	target_link_libraries(bench_vec lily_base)

	add_executable(bench_string benchmarks/base/string.c)
	target_link_libraries(bench_string lily_base)

	add_executable(bench_hash_map benchmarks/base/hash_map.c)
	target_link_libraries(bench_hash_map lily_base)
endif()
