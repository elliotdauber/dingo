## Dingo's Clang Parser

This module uses the clang AST parsing library to parse C++ and lower it to a ModuleIR graph

1. Run `../bin/clangparser programs/main.cpp`
2. Run `dot -Tpng programs/output.dot > output.png`
