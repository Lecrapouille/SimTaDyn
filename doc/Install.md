# SimTaDyn Installation Guide

- Installation
  - Linux
    - [Ubuntu](#ubuntu)
  - OS X
    - Not yet in developpement
  - Windows
    - Not yet in developpement
- [Run](#run)

## Ubuntu

Install dependencies:
```sh
sudo apt-get install freeglut3 freeglut3-dev mesa-common-dev libgtkglextmm-x11-1.2-dev libgtkmm-2.4-dev libgtksourceviewmm-3.0-dev libglu1-mesa-dev libglew-dev libcppunit-dev gcovr libdw-dev
```

It's recommanded to install the gtkmm version 3.22 (I'm not totally sure but this version is given with Ubuntu 16.10) and not the gtkm-3.18 (present with Ubuntu 16.04) due to this [bug](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Bugs.md). 

If needed, for fixing some disgracious messages Gtk-Message: Failed to load module "overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

Get SimTaDyn source, compile, install:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
cd SimTaDyn/src
make
make install-data
```

Explainations:
* the make command will create the SimTaDyn/build directory and compile the executable on here.
* the make install is not yet implemented. So, for the moment call SimTaDyn from the sources.
* thee make install-data will create a hidden directory at $HOME/.SimTaDyn and copy some needed files.

## Other architecture

Not yet done. No developement scheduled yet.

## Run

To launch SimTaDyn once compiled:
```sh
cd SimTaDyn/build
./SimTaDyn
```
Note:
* Be sure you type make install-data before launching the excutable. SimTaDyn needs these files installed.

## Bugs

See [here](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Bugs.md).
