# SimTaDyn

SIMulateur TAbleur DYNamique

Compilation (Linux or OS X):
cd src/ and call one of the script shell depending on your OS:
./build-osx.sh
or:
./build-ubuntu.sh

Note concerning OSX:
* if an error message "ld: library not found for -lssl clang: error: linker command failed with exit code 1 (use -v to see invocation)" try to type in your console:
xcode-select --install
* OpenGL is now badly supported with GTK+ (while working well in 2004): the current bug is the OpenGL background color covering all GTK+ widgets :(
