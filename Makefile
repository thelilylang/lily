CLANG_FORMAT = @clang-format -i
CMAKE_FORMAT = @cmake-format -i
BUILDER_GENERATOR ?= Ninja
JOBS ?= 4

# Resolved rather than named, because make runs a recipe of its own accord when
# it holds no shell metacharacter, and make before 4.4 execs whatever the PATH
# search lands on without checking that it is a file. The emsdk directory the
# wasm job puts on the PATH holds a `cmake` directory, which is enough to turn
# that into `make: cmake: Permission denied`. `command -v` skips it.
CMAKE := $(or $(shell command -v cmake),cmake)
CTEST := $(or $(shell command -v ctest),ctest)

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
INNER_BUILD ?= 0
LINK_DYNAMIC ?= 0

CMAKE_OPTIONS = \
	-DLILY_BUILD_LLVM=$(BUILD_LLVM) \
	-DLILY_BUILD_WASM=$(BUILD_WASM) \
	-DLILY_COMPILE_LIB_CC_STD=$(COMPILE_LIB_CC_STD) \
	-DLILY_DEBUG_MEM=$(DEBUG_MEM) \
	-DLILY_FULL_DEBUG=$(FULL_DEBUG) \
	-DLILY_LINK_DYNAMIC=$(LINK_DYNAMIC)

# The compilers to build with. `make` gives `CC` and `CXX` a default of its own,
# which says nothing about what the caller wants, so only a value that came from
# the command line or the environment is handed over.
ifneq ($(origin CC),default)
CMAKE_OPTIONS += -DCMAKE_C_COMPILER=$(CC)
endif

ifneq ($(origin CXX),default)
CMAKE_OPTIONS += -DCMAKE_CXX_COMPILER=$(CXX)
endif

# Where `find_package` is to look, for a caller with more than one LLVM
# installed. Unset, it looks wherever it usually would.
ifneq ($(LLVM_DIR),)
CMAKE_OPTIONS += -DLLVM_DIR=$(LLVM_DIR)
endif

ifneq ($(LLD_DIR),)
CMAKE_OPTIONS += -DLLD_DIR=$(LLD_DIR)
endif

ifneq ($(CLANG_DIR),)
CMAKE_OPTIONS += -DClang_DIR=$(CLANG_DIR)
endif

# The Emscripten toolchain file ships next to `emcc`, wherever emsdk was
# installed, and it is what turns `LILY_WASM` on.
#
# emcc also caches the sysroot libraries it builds, and it links nothing until
# it can lock that cache. A system-wide emsdk (`/usr/lib/emsdk`, say) is not
# writable, so fall back to a cache of our own inside the build directory. Both
# are set whether or not the wasm build is the one asked for, since `build` has
# no way of knowing which one it is finishing.
EMCC := $(shell command -v emcc)

ifneq ($(EMCC),)
EMSCRIPTEN_TOOLCHAIN := $(dir $(EMCC))cmake/Modules/Platform/Emscripten.cmake
EM_CACHE ?= $(shell test -w $(dir $(EMCC))cache \
	&& echo $(dir $(EMCC))cache \
	|| echo $(CURDIR)/build/em-cache)
export EM_CACHE
endif

# Building LLVM from the submodule only gives us a linker if `lld` is among the
# projects it is configured with.
ifeq ($(BUILD_LLVM),1)
CMAKE_OPTIONS += -DLLVM_ENABLE_PROJECTS="lld"
endif

# Configure one toolchain on its own, rather than the superbuild driver that
# spawns them all. `BUILD_WASM` then no longer says "the wasm build too", it
# says which toolchain this single build is for:
#
#   make debug INNER_BUILD=1                the native build, alone
#   make debug INNER_BUILD=1 BUILD_WASM=1   the wasm build, alone
#
# The build directory is the one named on the command line (`build/Debug`),
# with no `native`/`wasm` subdirectory below it, so `make test` does not apply.
ifeq ($(INNER_BUILD),1)
# `LILY_BUILD_WASM` is the driver's own option, and CMake warns about the ones
# it is handed but never reads.
CMAKE_OPTIONS := \
	$(filter-out -DLILY_BUILD_WASM=%,$(CMAKE_OPTIONS)) -DLILY_INNER_BUILD=1

ifeq ($(BUILD_WASM),1)
ifeq ($(EMCC),)
$(error `emcc` is not on the PATH, so the wasm toolchain cannot be found)
endif

# LLVM and the freestanding libc are both left out of the wasm build, and so
# are the options that only those two read.
CMAKE_OPTIONS := \
	$(filter-out -DLILY_COMPILE_LIB_CC_STD=% -DLILY_LINK_DYNAMIC=%,$(CMAKE_OPTIONS)) \
	-DCMAKE_TOOLCHAIN_FILE=$(EMSCRIPTEN_TOOLCHAIN)
endif
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
	@echo '  INNER_BUILD          configure a single toolchain, rather than the'
	@echo '                       superbuild driver; BUILD_WASM then names which'
	@echo '  LINK_DYNAMIC         link against the shared LLD libraries'
	@echo ''
	@echo '  CC, CXX              compilers to build with'
	@echo '  LLVM_DIR, LLD_DIR, CLANG_DIR'
	@echo '                       where `find_package` is to look for each'
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
		$(CMAKE) --build build -j $(JOBS); \
	fi
	@if [ -f build/Debug/CMakeCache.txt ]; then \
		$(CMAKE) --build build/Debug -j $(JOBS); \
	fi

configure:
	$(CMAKE) -S . -B build -G $(BUILDER_GENERATOR) -DCMAKE_BUILD_TYPE=Release $(CMAKE_OPTIONS)

debug:
	$(CMAKE) -S . -B build/Debug -G $(BUILDER_GENERATOR) -DCMAKE_BUILD_TYPE=Debug -DLILY_DEBUG=1 -DCMAKE_EXPORT_COMPILE_COMMANDS=YES $(CMAKE_OPTIONS)
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
# superbuild driver, so `ctest` runs from `build/Debug/native`. An
# `INNER_BUILD=1` configure has no such directory, and a wasm one registers no
# test at all, so what is there decides whether this applies.
test:
	@if [ ! -f build/Debug/CMakeCache.txt ]; then \
		echo 'Nothing to test. Please configure the project by running `make debug`.'; \
		exit 1; \
	fi
	$(CMAKE) --build build/Debug -j $(JOBS)
	@if [ ! -d build/Debug/native ]; then \
		echo '`build/Debug` was configured with INNER_BUILD=1, which registers no test. Please remove it and run `make debug` again: a configure of its own would keep what the cache holds.'; \
		exit 1; \
	fi
	cd build/Debug/native && $(CTEST) --verbose

format:
	./scripts/format.sh

clean:
	@rm -rf build
	@rm -rf bin

.PHONY: help hooks build configure debug llvm_submodule libyaml_submodule \
	submodules_without_llvm submodules test format clean
