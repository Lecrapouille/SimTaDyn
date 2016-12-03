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
sudo apt-get install freeglut3 freeglut3-dev mesa-common-dev libgtkglextmm-x11-1.2-dev libgtkmm-2.4-dev libgtksourceviewmm-3.0-dev libglu1-mesa-dev libcppunit-dev gcov
```

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
```

Note: the install is not yet implemented.

## Run

To launch SimTaDyn:
```sh
cd SimTaDyn/src
./build/SimTaDyn
```



