CLANG_FORMAT = @clang-format -i
CMAKE_FORMAT = @cmake-format -i
BUILDER_GENERATOR ?= Ninja
JOBS ?= 4

# Build options. Each one is off by default and enabled by setting it to 1 on
# the `make` command line, in any combination:
#
#   make debug BUILD_WASM=1
#   make configure BUILD_LLVM=1 LINK_DYNAMIC=1
#   make debug BUILD_WASM=1 DEBUG_MEM=1
#
BUILD_LLVM ?= 0
BUILD_WASM ?= 0
COMPILE_LIB_CC_STD ?= 0
DEBUG_MEM ?= 0
FULL_DEBUG ?= 0
LINK_DYNAMIC ?= 0

CMAKE_OPTIONS = \
	-DLILY_BUILD_LLVM=$(BUILD_LLVM) \
	-DLILY_BUILD_WASM=$(BUILD_WASM) \
	-DLILY_COMPILE_LIB_CC_STD=$(COMPILE_LIB_CC_STD) \
	-DLILY_DEBUG_MEM=$(DEBUG_MEM) \
	-DLILY_FULL_DEBUG=$(FULL_DEBUG) \
	-DLILY_LINK_DYNAMIC=$(LINK_DYNAMIC)

# Building LLVM from the submodule only gives us a linker if `lld` is among the
# projects it is configured with.
ifeq ($(BUILD_LLVM),1)
CMAKE_OPTIONS += -DLLVM_ENABLE_PROJECTS="lld"
endif

help:
	@echo 'Targets: configure (release), debug, build, test, format, clean,'
	@echo '         hooks, submodules, submodules_without_llvm'
	@echo ''
	@echo 'Options, set to 1 on the command line and combined freely:'
	@echo '  BUILD_LLVM           build LLVM from the submodule, rather than'
	@echo '                       using the one installed on the system'
	@echo '  BUILD_WASM           also build the wasm targets, with Emscripten'
	@echo '  COMPILE_LIB_CC_STD   build the experimental freestanding libc'
	@echo '  DEBUG_MEM            build with the address sanitizer'
	@echo '  FULL_DEBUG           imply DEBUG_MEM, and the debug build'
	@echo '  LINK_DYNAMIC         link against the shared LLD libraries'
	@echo ''
	@echo '  BUILDER_GENERATOR    CMake generator to use (default: Ninja)'
	@echo '  JOBS                 parallel build jobs (default: 4)'
	@echo ''
	@echo 'For example: make debug BUILD_WASM=1'

hooks:
	./scripts/patches/enable_local.sh
	cd .git/hooks && ln -sf ../../scripts/git/pre-commit . && ln -sf ../../scripts/git/post-commit .

# Build whichever of the two is configured, so that having only one of them
# does not make this fail.
#
# NOTE: `build/Debug` sits inside `build`, so the directory existing says
# nothing about a release build having been configured. The cache file does.
build:
	@if [ ! -f build/CMakeCache.txt ] && [ ! -f build/Debug/CMakeCache.txt ]; then \
		echo 'Nothing to build. Please configure the project by running `make configure` or `make debug`.'; \
		exit 1; \
	fi
	@if [ -f build/CMakeCache.txt ]; then \
		cmake --build build -j $(JOBS); \
	fi
	@if [ -f build/Debug/CMakeCache.txt ]; then \
		cmake --build build/Debug -j $(JOBS); \
	fi

configure:
	cmake -S . -B build -G $(BUILDER_GENERATOR) -DCMAKE_BUILD_TYPE=Release $(CMAKE_OPTIONS)

debug:
	cmake -S . -B build/Debug -G $(BUILDER_GENERATOR) -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=YES $(CMAKE_OPTIONS)
	cd build && ln -sf Debug/compile_commands.json .

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

# NOTE: The tests are registered by the native inner build, not by the
# superbuild driver, so `ctest` runs from `build/Debug/native`.
test:
	cmake --build build/Debug -j $(JOBS)
	cd build/Debug/native && ctest --verbose

format:
	./scripts/format.sh

clean:
	@rm -rf build
	@rm -rf bin

.PHONY: help hooks build configure debug llvm_submodule libyaml_submodule \
	submodules_without_llvm submodules test format clean
