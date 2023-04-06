CLANG_FORMAT = @clang-format -i

build:
	ninja -C build
	ninja -C build/Debug

configure:
	@mkdir -p build && cd build && cmake .. -G Ninja

debug:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json .

format:
	${CLANG_FORMAT} ./include/base/*.h
	${CLANG_FORMAT} ./include/cli/*.h
	${CLANG_FORMAT} ./include/cli/config/*.h
	${CLANG_FORMAT} ./include/cli/option/*.h
	${CLANG_FORMAT} ./include/command/*.h
	${CLANG_FORMAT} ./include/command/build/*.h
	${CLANG_FORMAT} ./include/command/cc/*.h
	${CLANG_FORMAT} ./include/command/compile/*.h
	${CLANG_FORMAT} ./include/command/cpp/*.h
	${CLANG_FORMAT} ./include/command/init/*.h
	${CLANG_FORMAT} ./include/command/new/*.h
	${CLANG_FORMAT} ./include/command/run/*.h
	${CLANG_FORMAT} ./include/command/test/*.h
	${CLANG_FORMAT} ./include/command/to/*.h
	${CLANG_FORMAT} ./include/core/cc/*.h
	${CLANG_FORMAT} ./include/core/cpp/*.h
	${CLANG_FORMAT} ./include/core/lily/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/body/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/decl/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/pattern/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/stmt/*.h
	${CLANG_FORMAT} ./include/core/lily/checked/*.h
	${CLANG_FORMAT} ./include/core/lily/checked/decl/*.h
	${CLANG_FORMAT} ./include/core/lily/checked/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/checked/stmt/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cc/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cc/builder/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cc/builder/function/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cc/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cpp/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cpp/builder/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cpp/builder/class/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cpp/builder/function/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/cpp/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/ir/llvm/*.h
	${CLANG_FORMAT} ./include/core/lily/package/*.h	
	${CLANG_FORMAT} ./include/core/shared/*.h
	${CLANG_FORMAT} ./src/base/*.c
	${CLANG_FORMAT} ./src/bin/*.c
	${CLANG_FORMAT} ./src/cli/*.c
	${CLANG_FORMAT} ./src/cli/option/*.c
	${CLANG_FORMAT} ./src/command/build/*.c
	${CLANG_FORMAT} ./src/command/cc/*.c
	${CLANG_FORMAT} ./src/command/compile/*.c
	${CLANG_FORMAT} ./src/command/cpp/*.c
	${CLANG_FORMAT} ./src/command/init/*.c
	${CLANG_FORMAT} ./src/command/new/*.c
	${CLANG_FORMAT} ./src/command/run/*.c
	${CLANG_FORMAT} ./src/command/test/*.c
	${CLANG_FORMAT} ./src/command/to/*.c
	${CLANG_FORMAT} ./src/core/cc/*.c
	${CLANG_FORMAT} ./src/core/cpp/*.c
	${CLANG_FORMAT} ./src/core/lily/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/body/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/decl/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/pattern/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/checked/*.c
	${CLANG_FORMAT} ./src/core/lily/checked/decl/*.c
	${CLANG_FORMAT} ./src/core/lily/checked/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/checked/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cc/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cc/builder/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cc/builder/function/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cc/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cpp/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cpp/builder/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cpp/builder/class/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cpp/builder/function/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/cpp/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/llvm/*.c
	${CLANG_FORMAT} ./src/core/lily/ir/*.c
	${CLANG_FORMAT} ./src/core/lily/package/*.c
	${CLANG_FORMAT} ./src/core/shared/*.c
	${CLANG_FORMAT} ./benchmarks/base/*.c
	${CLANG_FORMAT} ./tests/base/*.c
	${CLANG_FORMAT} ./tests/core/lily/parser/*.c
	${CLANG_FORMAT} ./tests/core/lily/precompile/*.c
	${CLANG_FORMAT} ./tests/core/lily/preparser/*.c
	${CLANG_FORMAT} ./tests/core/lily/scanner/*.c

# TODO: try to port -pg on CMake config
profile:
	@mkdir -p build && cd build && cmake .. -G Ninja && ninja
	@mkdir -p build/profile
	@gcc -Wall -O3 -pg -lLLVM -L build/ -llily_base -llily_cli -llily_command -I include -o build/profile/lily \
		src/bin/main.c \
		src/base/*.c \
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
		src/core/lily/checked/decl/*.c \
		src/core/lily/checked/expr/*.c \
		src/core/lily/checked/stmt/*.c \
		src/core/lily/checked/*.c \
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
		src/core/lily/package/*.c \
		src/core/lily/*.c \
		src/core/shared/*.c

clean:
	@rm -rf build
