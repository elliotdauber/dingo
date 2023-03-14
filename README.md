# dingo
A Lingo for Design

Run `make clean && make compiler && make` to make all binaries.

1. dingo: run `bin/dingo -h` for help. Note: must create a `compile_commands.json` for your project using `bear` or a similar tool, and this must be in the folder that you run dingo from.
2. compiler: run `bin/compiler <dingofile.dingo>`
3. cppparser: run `bin/cppparser <cpp_file.cpp>`
