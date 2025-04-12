# ram

##

This is a cache and memory simulator for a custom ISA nicknamed "RISC V[ECTOR]". It uses a writeback and write allocate on a miss scheme, and supports a configurable number of cache levels and ways, with a least-recently used replacement policy.

## Dependencies

g++, CMake, and the following libraries are required to compile:

- cmake (tested with v3.30.3)
- g++ (tested with v11.4.0)
- catch2 (tested with v3.5.3)

## To run

Generate the build directory with

`cmake -S . -B build`

then compile both the simulator and tests with

`cmake --build build`

# about

Created at the University of Massachusetts, Amherst

CS535 -- Computer Architecture and ISA Design
