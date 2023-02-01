configure:
	@mkdir -p build && cd build && cmake .. -G Ninja

debug:
	@mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -s Debug/compile_commands.json .

format:
	@clang-format -i ./include/base/*.h
	@clang-format -i ./src/base/*.c
	@clang-format -i ./src/bin/*.c
