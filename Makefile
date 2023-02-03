configure:
	@mkdir -p build && cd build && cmake .. -G Ninja

debug:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json .

format:
	@clang-format -i ./include/base/*.h
	@clang-format -i ./include/cli/*.h
	@clang-format -i ./include/cli/config/*.h
	@clang-format -i ./include/cli/option/*.h
	@clang-format -i ./src/base/*.c
	@clang-format -i ./src/bin/*.c
	@clang-format -i ./src/cli/*.c
	@clang-format -i ./src/cli/config/*.c
	@clang-format -i ./src/cli/option/*.c
	@clang-format -i ./benchmarks/base/*.c
	@clang-format -i ./tests/base/*.c
