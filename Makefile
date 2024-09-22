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
	mkdir -p build && cd build && cmake .. -G Ninja -DCMAKE_BUILD_TYPE=Release -DLILY_BUILD_LLVM=ON

debug:
	mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -sf Debug/compile_commands.json .

debug_with_llvm:
	mkdir -p build && cd build && cmake -H. -BDebug -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DLILY_BUILD_LLVM=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=YES .. -G Ninja && ln -sf Debug/compile_commands.json .

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

format:
	${CMAKE_FORMAT} ./cmake/*.cmake
	${CLANG_FORMAT} ./include/base/*.h	
	${CLANG_FORMAT} ./include/base/cli/*.h	
	${CLANG_FORMAT} ./include/base/cli/result/*.h	
	${CLANG_FORMAT} ./include/base/hash/*.h
	${CLANG_FORMAT} ./include/base/memory/*.h
	${CLANG_FORMAT} ./include/base/object/*.h
	${CLANG_FORMAT} ./include/base/object/value/*.h
	${CLANG_FORMAT} ./include/cli/*.h
	${CLANG_FORMAT} ./include/cli/ci/*.h
	${CLANG_FORMAT} ./include/cli/lily/*.h
	${CLANG_FORMAT} ./include/cli/lily/config/*.h
	${CLANG_FORMAT} ./include/cli/lilyc/*.h
	${CLANG_FORMAT} ./include/command/ci/*.h
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
	${CLANG_FORMAT} ./include/command/lilyc/*.h
	${CLANG_FORMAT} ./include/core/cc/*.h
	${CLANG_FORMAT} ./include/core/cc/ci/*.h
	${CLANG_FORMAT} ./include/core/cc/ci/diagnostic/*.h
	${CLANG_FORMAT} ./include/core/cc/ci/extensions/*.h
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
	${CLANG_FORMAT} ./include/core/lily/compiler/ar/*.h
	${CLANG_FORMAT} ./include/core/lily/compiler/driver/*.h
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
	${CLANG_FORMAT} ./include/core/lily/interpreter/package/*.h
	${CLANG_FORMAT} ./include/core/lily/interpreter/vm/*.h
	${CLANG_FORMAT} ./include/core/lily/interpreter/vm/runtime/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/expr/*.h
	${CLANG_FORMAT} ./include/core/lily/mir/generator/stmt/*.h 
	${CLANG_FORMAT} ./include/core/lily/compiler/package/*.h
	${CLANG_FORMAT} ./include/core/lily/package/*.h
	${CLANG_FORMAT} ./include/core/lily/package/compiler/*.h
	${CLANG_FORMAT} ./include/core/lily/package/interpreter/*.h
	${CLANG_FORMAT} ./include/core/lily/preparser/*.h
	${CLANG_FORMAT} ./include/core/lily/preparser/preprocess/*.h
	${CLANG_FORMAT} ./include/core/lily/precompiler/*.h
	${CLANG_FORMAT} ./include/core/lily/scanner/*.h
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
	${CLANG_FORMAT} ./src/base/hash/*.c
	${CLANG_FORMAT} ./src/base/memory/*.c
	${CLANG_FORMAT} ./src/base/object/*.c
	${CLANG_FORMAT} ./src/base/object/value/*.c
	${CLANG_FORMAT} ./src/bin/ci/*.c
	${CLANG_FORMAT} ./src/bin/lily/*.c
	${CLANG_FORMAT} ./src/bin/lilyc/*.c
	${CLANG_FORMAT} ./src/cli/ci/*.c
	${CLANG_FORMAT} ./src/cli/lily/*.c
	${CLANG_FORMAT} ./src/cli/lilyc/*.c
	${CLANG_FORMAT} ./src/command/ci/*.c
	${CLANG_FORMAT} ./src/command/lily/build/*.c
	${CLANG_FORMAT} ./src/command/lily/cc/*.c
	${CLANG_FORMAT} ./src/command/lily/compile/*.c
	${CLANG_FORMAT} ./src/command/lily/cpp/*.c
	${CLANG_FORMAT} ./src/command/lily/init/*.c
	${CLANG_FORMAT} ./src/command/lily/new/*.c
	${CLANG_FORMAT} ./src/command/lily/run/*.c
	${CLANG_FORMAT} ./src/command/lily/test/*.c
	${CLANG_FORMAT} ./src/command/lily/to/*.c
	${CLANG_FORMAT} ./src/command/lilyc/*.c
	${CLANG_FORMAT} ./src/core/cc/ci/*.c
	${CLANG_FORMAT} ./src/core/cc/ci/diagnostic/*.c
	${CLANG_FORMAT} ./src/core/cc/ci/extensions/*.c
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
	${CLANG_FORMAT} ./src/core/lily/compiler/ar/*.c
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
	${CLANG_FORMAT} ./src/core/lily/compiler/ir/*.cpp
	${CLANG_FORMAT} ./src/core/lily/compiler/linker/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/output/*.c
	${CLANG_FORMAT} ./src/core/lily/diagnostic/*.c
	${CLANG_FORMAT} ./src/core/lily/functions/*.c
	${CLANG_FORMAT} ./src/core/lily/interpreter/package/*.c
	${CLANG_FORMAT} ./src/core/lily/interpreter/vm/*.c
	${CLANG_FORMAT} ./src/core/lily/interpreter/vm/runtime/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/expr/*.c
	${CLANG_FORMAT} ./src/core/lily/mir/generator/stmt/*.c
	${CLANG_FORMAT} ./src/core/lily/compiler/package/*.c
	${CLANG_FORMAT} ./src/core/lily/package/*.c
	${CLANG_FORMAT} ./src/core/lily/package/compiler/*.c
	${CLANG_FORMAT} ./src/core/lily/package/interpreter/*.c
	${CLANG_FORMAT} ./src/core/lily/preparser/*.c
	${CLANG_FORMAT} ./src/core/lily/preparser/preprocess/*.c
	${CLANG_FORMAT} ./src/core/lily/precompiler/*.c
	${CLANG_FORMAT} ./src/core/lily/scanner/*.c
	${CLANG_FORMAT} ./src/core/shared/*.c
	${CLANG_FORMAT} ./src/core/shared/target/*.c
	${CLANG_FORMAT} ./src/ex/bin/*.c
	${CLANG_FORMAT} ./src/ex/lib/*.c
	${CLANG_FORMAT} ./benchmarks/base/*.c
	${CLANG_FORMAT} ./tests/base/*.c
	${CLANG_FORMAT} ./tests/base/memory/*.c
	${CLANG_FORMAT} ./tests/core/lily/parser/*.c
	${CLANG_FORMAT} ./tests/core/lily/precompiler/*.c
	${CLANG_FORMAT} ./tests/core/lily/preparser/*.c
	${CLANG_FORMAT} ./tests/core/lily/scanner/*.c
	${CMAKE_FORMAT} ./CMakeLists.txt
	${CMAKE_FORMAT} ./src/core/cc/ci/CMakeLists.txt

clean:
	@rm -rf build
