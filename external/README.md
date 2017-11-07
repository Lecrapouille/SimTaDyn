Add here all external libraries that SimTaDyn depends on and which cannot be installed with the OS installer (apt-get, brew ...).

* download-external-libs.sh: bash script will git clone them (some recurisvely).
* compile-external-libs.sh: bash script compile these libraries. They are not installed into your environement (no sudo make install calls are made).
Hence SimTaDyn Makefiles has to know explicitly the path of compiled resources (header files, static/shared libraries, ...).

These scripts are seperated to be launched by the ../Makefile. They takes one parameter: the name of the architecture (Linux or Darwin or Windos).
