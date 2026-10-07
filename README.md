# atexit

**atexit** - forget about the forgotten free()

# Description

This is a small project to prevent double free and
memory leak. 

## Features

- C89/ANSI C code standard.
- Minimal memory usage.
- Auto-free functions
- Auto-generation wrappers

## Quick start

```c
#define ATEXIT_SOURCE
#include "atexit.h"

#define MAIN_VOID
#include "main_wrapper.h"

int Main(AtExitHeader* main_atexit) {
  char* buffer = AtExitMalloc(main_atexit, 4096);
  if (!buffer) {
    return 1;
  }

  /* do something... */

  /* No need write free() */
  return 0;
}
```

## Needed

- C Compiler with support C89 standard.
- CMake 3.10 and greater

## Building

```bash
cmake -B build
cd build/
make
```

CMake have options:

**BUILD_EX** - build examples in `examples/` directory.
Default: ON

**USE_TESTS** - build tests.
Default: ON

### How add to your project
To use atexit in your project, you just move `include/*` to project:

```bash
mkdir -p myproject/
cp -r include/ myproject/
```

## Examples
You can find examples in `examples/` directory

## License
Project is under Apache 2.0 license.

## Authors
TheCRatTime.

## Also
See this readme in manual (`README.1`) with this command:

```bash
man -l README.1
```
