Add here all external libraries that SimTaDyn depends on and which cannot be installed directly with the OS installer (apt-get, brew ...):
* backward-cpp: allows to display the stack when a segfault occures.
* SOIL: allows to read picture files (jpeg, png ...) for OpenGL texturing.
* zipper: a library for zip archives.
* YesEngine: (partial code) gives some meta programming code.

SimTaDyn does not use git submodules but two bash scripts for doing the same work.
These scripts are seperated to be shared by the ../Makefile and by continous integration scripts (../.integration/):
* download-external-libs.sh: this script will git clone (some recurisvely) needed libraries.
* compile-external-libs.sh: this script will compile these libraries.

Libraries are not installed in your environement (no sudo make install calls are made). Therefore SimTaDyn Makefile has to know explicitly
the path of compiled resources (header files, static/shared libraries, ...).

Scripts take one parameter: the name of the architecture: `Linux` or `Darwin` or `Windows`.
