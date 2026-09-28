# atexit
**atexit** - forget about the forgotten free()

# Description
This is a small project for disable double free and
memory leak.

## Features
- C89/ANSI C code standard.
- Minimal memory usage.

# Needed
- C Compiler with support C89 standard.
- CMake 3.10 and greater

### Building
```bash
# Just build
cmake -B build
cd build/
make
```

```bash
# Fast build
cmake -B build
cd build/
make -j$(nproc)
```

```bash
# Use ninja
cmake -B build -G Ninja
cd build/
ninja
```

CMake have options:
**USE_SHARED** - use shared library instead of static.
Default: OFF

**BUILD_EX** - build examples in `examples/` directory.
Default: OFF

**USE_QUOTES** - Use "quotes" instead of <this> in includes.
Default: ON

**OWN_INC_PATH** - Use own path to atexit.h
Default: ../include/atexit.h

### How add to your project
That use atexit in your project, you can:
1. Link code and library (**libatexit_do.a**)
```bash
# Example:
gcc -B/path/to/libraries/ your_code.c -latexit_do -o output
```

2. Move source to project:
```bash
mkdir -p myproject/src
cp src/atexit.c myproject/src/
cp -r include/ myproject/
# If needed, define USE_QUOTES and OWN_INC_PATH in CMake or
# INC_USE_QUOTES and ATEXIT_PATH_TO_INC in code via #define.
```

## Examples
You can find examples in `examples/` directory

## License
Project is under Apache 2.0 license.

## Authors
TheCRatTime.

## Also
See this readme in manual -> `README.1`
with this command:

```bash
man -l README.1
```
