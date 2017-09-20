Add here all external libraries that SimTaDyn depends on and which cannot be installed with the OS installer (apt-get, brew ...).

The gitclone.sh bash script will git clone them (some recurisvely) and compile them. They are not installed into your environement. Therefore SimTaDyn makefiles have to know where to find their files (includes and static/shared libraries).

gitclone.sh takes one parameter: the name of the architecture (Linux or Darwin or Windos). Better to call this script from ../Makefile with the rule `make clone`.
