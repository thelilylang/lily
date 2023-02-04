CLANG_FORMAT = @clang-format -i

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
	${CLANG_FORMAT} ./include/core/lily/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/decl/*.h
	${CLANG_FORMAT} ./include/core/lily/ast/expr/*.h
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
	${CLANG_FORMAT} ./src/core/lily/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/decl/*.c
	${CLANG_FORMAT} ./src/core/lily/ast/expr/*.c
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
	${CLANG_FORMAT} ./src/core/lily/package/*.c
	${CLANG_FORMAT} ./benchmarks/base/*.c
	${CLANG_FORMAT} ./tests/base/*.c

clean:
	@rm -rf build
