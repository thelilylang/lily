# Build

## Foreword

> [!TIP]
> You can also build `Lily` on a docker image instead of doing it
> directly on your computer. Please follow this
> [documentation](docker/dev/README.md) to be able to run the
> docker image.

## Introduction

First, you need to load the git submodules.

```
make submodules
```

> [!TIP]
> If you already have LLVM installed, you don't need to download the `llvm-project` submodule.
> You can run the `make submodules_without_llvm` command instead.

If you only want to use `Lily` locally to try it out, run:<br>

```bash
./scripts/patches/enable_local.sh
```

If you want to install Lily on your machine, you'll be able to do so when you finish building in release mode (configure with `make configure`):

> [!CAUTION]
> For the moment, the script only supports Unix machines and the Bash, Zsh and Fish shells.
> The script will also modify your shell configuration file.

```bash
./scripts/install.sh
```

## Linux

Make sure you are using `Clang` 18 as your default compiler, if not, run:

```bash
export CC=clang # or clang-18
export CXX=clang++ # or clang++-18
```

After launching the CMake configuration:

```bash
make configure_with_llvm # We use Ninja
# If LLVM is already installed:
make configure # We use Ninja
```

Then, the build:

```bash
# If you don't have a very powerful machine or are already using a
# lot of resources, it's worth trying to add a restriction on the
# number of possible tasks, with the -j option.
ninja -C build -j <n_jobs>
```

Launch `lilyc`:<br>
NOTE: This is the command to use the Lily compiler.

```bash
./bin/lilyc -h # local version
lilyc -h # installed version
```

Launch `lily`:<br>
NOTE: This is the command to use some utility tools for the Lily language (build, init, new, ...).

```bash
./bin/lily -h  # local version
lily -h # installed version
```

## MacOS

Make sure you are using `Clang` 18 as your default compiler, if not, run:

```bash
# Brew installation
export CC=$(brew --prefix llvm@18)/bin/clang 
export CXX=$(brew --prefix llvm@18)/bin/clang++ 

# No-brew installation
export CC=clang
export CXX=clang++ 
```

After launching the CMake configuration:

```bash
make configure_with_llvm # We use Ninja
# If LLVM is already installed:
make configure # We use Ninja
```

If LLVM is not found by CMake run:

```bash
# Brew installation
export LLVM_DIR=$(brew --prefix llvm@18)
```

Then, the build:

```bash
# If you don't have a very powerful machine or are already using a
# lot of resources, it's worth trying to add a restriction on the
# number of possible tasks, with the -j option.
ninja -C build -j <n_jobs>
```

Launch `lilyc`:<br>
NOTE: This is the command to use the Lily compiler.

```bash
./bin/lilyc -h # local version
lilyc -h # installed version
```

Launch `lily`:<br>
NOTE: This is the command to use some utility tools for the Lily language (build, init, new, ...).

```bash
./bin/lily -h  # local version
lily -h # installed version
```

## Windows

...

## BSD

### FreeBSD

The steps for building on FreeBSD are exactly the same as those described for Linux.
