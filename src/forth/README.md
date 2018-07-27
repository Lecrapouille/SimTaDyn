List of source files:
* ForthCells.hpp: define number of bytes for Forth Cells.
* ForthStack.tpp: Stack Forth class for the Forth data stack and return stack.
* ForthPrimitives.[ch]pp: Allow to create the core of the Forth interpreter by defining primitive words.
Primitives code are split in parts inside the primitives/ directory.
* ForthStream.[ch]pp: allow Forth interpreter to parse scripts from different inputs (string, files ...).
* ForthClibrary.[ch]pp: Allow Forth scripts to write C code and/or load dynamic C libraries and creat Forth words calling C functions.
* ForthUtility.[ch]pp: utility functions for the Forth interpreter.
* ForthExceptions.[ch]pp: Define all messages for triggering Forth exceptions.
* ForthColorSyntax.hpp: define syntax highlighter for coloring Forth words.
* primitives/: contains primitive words split into categories.
* core/: forth scripts for completing Forth core.
* standalone/: allow to compile a standalone Forth interpreter.
* doc/: documentation about this Forth.
