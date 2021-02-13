# bompiler
bompiler is a compiler that currently only analyzes tokens (for now).

## Building bompiler
bomiler requires [CMake](https://cmake.org/install/) for building.

1. `cmake .`
2. `make`

The executable `lexdriver` will be located in `./bin/`

## Usage
`./bin/lexdriver <path-to-.src>` where the path to the .src file is either relative or absolute.