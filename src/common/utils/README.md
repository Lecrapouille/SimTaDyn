Files:

* Exception.[ch]pp: Class extending std::exceptions. All project will use this class and not the std::

* AnsiColor.hpp: Class for writing text in console (xterm) with or without color. Mainly used for the Forth interpretor. FIXME: move it to forth directory.

* File.[ch]pp: Routines for manangig file and directory names. Not for managing file contents.

* Path.[ch]pp: Class mimic the behavior of Linux variable environment $PATH.

* ILogger.[ch]pp: Interface for logs.

* Logger.[ch]pp: File logger implementation. FIXME rename it to FileLogger.[ch]pp
