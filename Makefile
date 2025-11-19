CLANG_FORMAT = @clang-format -i
CMAKE_FORMAT = @cmake-format -i

hooks:
	./scripts/patches/enable_local.sh
	cd .git/hooks && ln -sf ../../scripts/git/pre-commit . && ln -sf ../../scripts/git/post-commit .

build:
	cmake --build  build/
	cmake --build  build/Debug

configure:
	mkdir -p build && cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release

configure_with_llvm:
	mkdir -p build && cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DLILY_BUILD_LLVM=ON -DLLVM_ENABLE_PROJECTS="lld"

debug:
	mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -sf Debug/compile_commands.json .

debug_with_llvm:
	mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DLILY_BUILD_LLVM=ON -DLLVM_ENABLE_PROJECTS="lld" -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -sf Debug/compile_commands.json .

llvm_submodule:
	git submodule init
	git submodule update lib/local/src/llvm-project
	mkdir -p lib/local/include
	cd lib/local/include && ln -sf ../src/llvm-project/llvm/include/llvm . && ln -sf ../src/llvm-project/llvm/include/llvm-c .

libyaml_submodule:
	git submodule init
	git submodule update lib/local/src/libyaml
	cd lib/local/src/libyaml && ./bootstrap && ./configure && mv include/config.h src

submodules_without_llvm: libyaml_submodule
submodules: llvm_submodule libyaml_submodule

test:
	cmake --build build/Debug -j 4
	cd build/Debug && ctest --verbose

format:
	./scripts/format.sh

clean:
	@rm -rf build
	@rm -rf bin
