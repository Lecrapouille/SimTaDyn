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
sudo apt-get install freeglut3 freeglut3-dev mesa-common-dev libgtkglextmm-x11-1.2-dev libgtkmm-2.4-dev libgtksourceviewmm-3.0-dev libglu1-mesa-dev libglew-dev libcppunit-dev gcovr
```

It's highly recommanded to have the gtkmm-3.22 version and not the gtkm-3.18 due to a bug.
I'm not totally sure but Ubuntu 16.10 has this version of gtkmm.
See [here](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Bugs.md)

If needed:
For fixing some disgracious messages Gtk-Message: Failed to load module "overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

Get SimTaDyn source, compile, install:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
mkdir SimTaDyn/src
make
make install-data
```

Note:
* the install is not yet implemented. For the moment call SimTaDyn from its sources.
* install-data will create a .SimTaDyn directory at your $HOME and copy important elements on it.

## Other architecture

Not yet in developement.

## Run

To launch SimTaDyn once installed:
```sh
cd SimTaDyn/build
./SimTaDyn
```

## Bugs
