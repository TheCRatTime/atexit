# atexit
**atexit** - forget about the forgotten free()

# Description
This is a small project for disable double free and
memory leak.

# Features
- C89/ANSI C code standard.
- Minimal memory usage.

# Needed
- C Compiler with support C89.
- CMake 3.10 and greater

# Building
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

CMake have options:
**USE_SHARED** - use shared library instead of static.
Default: OFF

**BUILD_EX** - build examples in `examples/` directory.
Default: OFF

# Examples
You can find examples in `examples/` directory

# License
Project is under Apache 2.0 license.

# Authors
TheCRatTime.

# Also
See this readme in manual -> `README.1`
