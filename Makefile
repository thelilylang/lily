CLANG_FORMAT = @clang-format -i
CMAKE_FORMAT = @cmake-format -i
RUSTFMT = @rustfmt

setup:
	cd .git/hooks && ln -s ../../scripts/git/pre-commit .

build:
	ninja -C build
	ninja -C build/Debug

configure:
	@mkdir -p build && cd build && cmake .. -G Ninja

debug:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json .

format:
	${CLANG_FORMAT} ./include/base/*.h	
	${CLANG_FORMAT} ./include/base/cli/*.h	
	${CLANG_FORMAT} ./include/base/cli/result/*.h	
	${CLANG_FORMAT} ./include/base/hash/*.h
	${CLANG_FORMAT} ./include/base/memory/*.h
	${CLANG_FORMAT} ./include/cli/*.h
	${CLANG_FORMAT} ./include/cli/lily/*.h
	${CLANG_FORMAT} ./include/cli/lily/config/*.h
	${CLANG_FORMAT} ./include/cli/lilyc/*.h
	${CLANG_FORMAT} ./include/command/lily/*.h
	${CLANG_FORMAT} ./include/command/lily/build/*.h
	${CLANG_FORMAT} ./include/command/lily/cc/*.h
	${CLANG_FORMAT} ./include/command/lily/compile/*.h
	${CLANG_FORMAT} ./include/command/lily/cpp/*.h
	${CLANG_FORMAT} ./include/command/lily/init/*.h
	${CLANG_FORMAT} ./include/command/lily/new/*.h
	${CLANG_FORMAT} ./include/command/lily/run/*.h
	${CLANG_FORMAT} ./include/command/lily/test/*.h
	${CLANG_FORMAT} ./include/command/lily/to/*.h
	${CLANG_FORMAT} ./include/core/cc/*.h
	${CLANG_FORMAT} ./include/core/cc/diagnostic/*.h
	${CLANG_FORMAT} ./include/core/cpp/diagnostic/*.h
	${CLANG_FORMAT} ./include/core/lily/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/body/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/decl/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/pattern/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/ast/stmt/*.h
	${CLANG_FORMAT} ./include/core/lily/parser/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/body/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/decl/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/pattern/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/checked/stmt/*.h
	${CLANG_FORMAT} ./include/core/lily/analysis/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cc/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cc/builder/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cc/builder/function/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cc/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cpp/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cpp/builder/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cpp/builder/class/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cpp/builder/function/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/cpp/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/ir/llvm/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/linker/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/output/*.h
	${CLANG_FORMAT} ./include/core/lily/diagnostic/*.h
	${CLANG_FORMAT} ./include/core/lily/functions/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/stmt/*.h 
	${CLANG_FORMAT} ./include/core/lily/compiler/package/*.h
	${CLANG_FORMAT} ./include/core/lily/preparser/*.h
	${CLANG_FORMAT} ./include/core/lily/preparser/preprocess/*.h
	${CLANG_FORMAT} ./include/core/lily/precompiler/*.h
	${CLANG_FORMAT} ./include/core/lsp/*.h
	${CLANG_FORMAT} ./include/core/shared/*.h
	${CLANG_FORMAT} ./include/core/shared/target/*.h
	${CLANG_FORMAT} ./lib/*.h
	${CLANG_FORMAT} ./lib/builtin/*.h
	${CLANG_FORMAT} ./lib/builtin/*.c
	${CLANG_FORMAT} ./lib/sys/*.h
	${CLANG_FORMAT} ./lib/sys/*.c
	${CLANG_FORMAT} ./src/base/*.c
	${CLANG_FORMAT} ./src/base/cli/*.c
	${CLANG_FORMAT} ./src/base/cli/result/*.c
	${CLANG_FORMAT} ./src/base/memory/*.c
	${CLANG_FORMAT} ./src/base/hash/*.c
	${CLANG_FORMAT} ./src/bin/lily/*.c
	${CLANG_FORMAT} ./src/bin/lilyc/*.c
	${CLANG_FORMAT} ./src/cli/lily/*.c
	${CLANG_FORMAT} ./src/cli/lilyc/*.c
	${CLANG_FORMAT} ./src/command/lily/build/*.c
	${CLANG_FORMAT} ./src/command/lily/cc/*.c
	${CLANG_FORMAT} ./src/command/lily/compile/*.c
	${CLANG_FORMAT} ./src/command/lily/cpp/*.c
	${CLANG_FORMAT} ./src/command/lily/init/*.c
	${CLANG_FORMAT} ./src/command/lily/new/*.c
	${CLANG_FORMAT} ./src/command/lily/run/*.c
	${CLANG_FORMAT} ./src/command/lily/test/*.c
	${CLANG_FORMAT} ./src/command/lily/to/*.c
	${CLANG_FORMAT} ./src/core/cc/diagnostic/*.c
	${CLANG_FORMAT} ./src/core/cc/*.c
	${CLANG_FORMAT} ./src/core/cpp/diagnostic/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/body/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/decl/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/pattern/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/ast/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/parser/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/body/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/decl/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/pattern/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/checked/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/analysis/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cc/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cc/builder/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cc/builder/function/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cc/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cpp/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cpp/builder/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cpp/builder/class/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cpp/builder/function/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/cpp/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/llvm/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/llvm/*.cpp
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/linker/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/output/*.c
	${CLANG_FORMAT} ./src/core/lily/diagnostic/*.c
	${CLANG_FORMAT} ./src/core/lily/functions/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/package/*.c
	${CLANG_FORMAT} ./src/core/lily/preparser/*.c
	${CLANG_FORMAT} ./src/core/lily/preparser/preprocess/*.c
	${CLANG_FORMAT} ./src/core/lily/precompiler/*.c
	${CLANG_FORMAT} ./src/core/shared/*.c
	${CLANG_FORMAT} ./src/core/shared/target/*.c
	${CLANG_FORMAT} ./benchmarks/base/*.c
	${CLANG_FORMAT} ./tests/base/*.c
	${CLANG_FORMAT} ./tests/base/memory/*.c
	${CLANG_FORMAT} ./tests/core/lily/parser/*.c
	${CLANG_FORMAT} ./tests/core/lily/precompile/*.c
	${CLANG_FORMAT} ./tests/core/lily/preparser/*.c
	${CLANG_FORMAT} ./tests/core/lily/scanner/*.c
	${CMAKE_FORMAT} ./CMakeLists.txt
	${RUSTFMT} ./src/core/cc/comptime_gen/src/*.rs
	

# TODO: try to port -pg on CMake config
profile:
	@mkdir -p build && cd build && cmake .. -G Ninja && ninja
	@mkdir -p build/profile
	@clang -Wall -O3 -pg -lLLVM -L build/ -llily_base -llily_cli -llily_command -llily_core -I include -I lib/local -o build/profile/lily \
		src/bin/main.c \
		src/base/*.c \
		src/base/cli/*.c \
		src/base/cli/result/*.c \
		src/base/hash/*.c \
		src/cli/option/*.c \
		src/cli/*.c \
		src/command/build/*.c \
		src/command/cc/*.c \
		src/command/compile/*.c \
		src/command/cpp/*.c \
		src/command/init/*.c \
		src/command/new/*.c \
		src/command/run/*.c \
		src/command/test/*.c \
		src/command/to/*.c \
		src/core/cc/*.c \
		src/core/cpp/*.c \
		src/core/lily/ast/body/*.c \
		src/core/lily/ast/decl/*.c \
		src/core/lily/ast/expr/*.c \
		src/core/lily/ast/pattern/*.c \
		src/core/lily/ast/stmt/*.c \
		src/core/lily/ast/*.c \
		src/core/lily/checked/*.c \
		src/core/lily/checked/body/*.c \
		src/core/lily/checked/decl/*.c \
		src/core/lily/checked/expr/*.c \
		src/core/lily/checked/stmt/*.c \
		src/core/lily/checked/pattern/*.c \
		src/core/lily/ir/cc/builder/function/*.c \
		src/core/lily/ir/cc/builder/*.c \
		src/core/lily/ir/cc/generator/*.c \
		src/core/lily/ir/cc/*.c \
		src/core/lily/ir/cpp/builder/*.c \
		src/core/lily/ir/cpp/builder/class/*.c \
		src/core/lily/ir/cpp/builder/function/*.c \
		src/core/lily/ir/cpp/generator/*.c \
		src/core/lily/ir/cpp/*.c \
		src/core/lily/ir/llvm/*.c \
		src/core/lily/ir/llvm/generator/*.c \
		src/core/lily/ir/llvm/generator/expr/*.c \
		src/core/lily/ir/llvm/generator/object/*.c \
		src/core/lily/ir/llvm/generator/type/*.c
		src/core/lily/ir/*.c \
		src/core/lily/mir/*.c \
		src/core/lily/mir/generator/expr/*.c \
		src/core/lily/mir/generator/stmt/*.c \
		src/core/lily/mir/generator/*.c \
		src/core/lily/package/*.c \
		src/core/lily/preprocess/*.c \
		src/core/lily/*.c \
		src/core/shared/*.c \
		src/core/shared/target/*.c

clean:
	@rm -rf build
