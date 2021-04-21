# bompiler
bompiler is a compiler that tokenizes a file, creates an AST from these tokens, creates symbolic tables, makes sure symbolic tables are okay (very partially done) and generates MOON processor code for execution.

## Building bompiler
bomiler requires [CMake](https://cmake.org/install/) for building.

1. `cmake .`
2. `make`

The executables will be located in `./bin/`

## Usage
`./bin/<executable> <path-to-.src>` where the path to the .src file is either relative or absolute.

Alternatively, you can use the test script with the name of the .src file (without .src, e.g., `./test.sh test`) which will do everything, namely:
- check if the provided .src file exists (should be under `data/testing/4/`)
- compile the main driver (called compilerdriver)
- run compilerdriver using the provided source file
- run the generated moon file
