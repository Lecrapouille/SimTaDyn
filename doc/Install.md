# SimTaDyn Installation Guide

This page explains how to compile the code source for the **master branch**. For other SimTaDyn branches (specially for the [Original-Version-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/Original-Version-EPITA-2004)), read the install file associated to the branch.

SimTaDyn is based on C++11, modern OpenGL (>= 3.0), gtkmm >= 3.22 and gtksourceviewmm (C++ interfaces for GTK+ and GtkSourceView) and other small libraries like libSOIL (for loading image files),  

Important note: gtkmm >= 3.22 is mandatory to get SimTaDyn worked correctly with OpenGL.

- Installation
  - Linux
    - [Ubuntu >= 17.04](#ubuntu-17.04)
    - [Ubuntu < 17.04](#other-ubuntu-versions)
    - [Debian](#other-architecture)
    - [Docker](#docker)
  - Mac OSX
    - [OS X](#mac-os-x)
  - Windows
    - [Not yet in development](#other-architecture)
- [Compilation](#compilation)
- [Run](#run)

## Ubuntu

Concerning libgtkmm-3.0-dev: t's mandatory to install the gtkmm version 3.22 due to a major bug with the OpenGL widget breaking the OpenGL context when using inheritance. See this [bug](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Bugs.md) for more informations. This version is not present on Ubuntu older than 17.04. That's mean on Ubuntu version 16.04, 16.11, you’ll have to install it by compiling sources by yourself (need long time to compile but the process compiles well) or use Docker with an Ubuntu 17.04 image.

### Ubuntu 17.04

For Ubuntu >= 17.04, install the following dependencies:
```sh
sudo apt-get install libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev libsoil-dev binutils-dev
```

Optionally, for launching unit tests, install the following packages:
```sh
libcppunit-dev gcovr libglm-dev
```

And even more optionally, for generating some expected unit test results, install the Scilab fork: [ScicosLab](http://www.scicoslab.org/)
```sh
sudo apt-get install scicoslab-gtk
```

If needed, for fixing some Gtk-Message: Failed to load module "overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

### Other Ubuntu versions

For Ubuntu versions < 17.04, if you have a 64-bits machine you could install [Docker](https://en.wikipedia.org/wiki/Docker_(software)), pull an image of Ubuntu 17.04 and inside the docker, follow instructions [described here](#docker). If you have a 32-bits, unfortuatly Docker does not support 32-bits architecture, so you will have to install by yourself a gtkmm version >= 3.22.

## Docker

- Install docker. For example, if you have an Ubuntu 16.04, [follow these steps](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04).
- Clone the project and go inside, this will the entry point for your docker:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
cd SimTaDyn
```
- Download the image of Ubuntu and launch the docker in an interactive bash where the root of the docker is the root of the SimTaDyn project:
```sh
docker pull ubuntu:17.04
docker run --rm -it -v "$(pwd):/SimTaDyn" -td ubuntu:17.04 /bin/bash
```
- Now you are inside the docker, follow:
- [these steps](#ubuntu-17.04) for installing libraries for SimTaDyn.
- [these steps](#Compilation) for compiling SimTaDyn.

## Mac OS X

SimTaDyn compiles on a MacBook Air and Mac OS X 10.9.5. To install gtkmm and other dependencies use the tool [homebrew](https://brew.sh/index_fr.html). The current gtkmm version is 3.22 and no problem occurred during the compilation of these libs. However the Gtk::GlArea is not implemented on this architecture and by consequence the OpenGL context is not created !

## Other architecture

Not yet tested/done. No development scheduled yet.

Because Cygwin stopped being ported on Windows XP, you need higher Window version (which it's not my case currently).

## Compilation

Get the SimTaDyn source, compile them and install resources:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
cd SimTaDyn/src
make
make install-data
```

Explanations:
* the make command will create the SimTaDyn/build directory and compile the executable on here.
* the make install is not yet implemented (because this project is still in developement). So, for the moment call SimTaDyn from the sources.
* thee make install-data will create a hidden directory at $HOME/.SimTaDyn and copy some needed files.

## Run

To launch SimTaDyn once compiled:
```sh
cd SimTaDyn/build
./SimTaDyn
```

Note:
* Be sure you typed ``make install-data`` before launching the executable. SimTaDyn needs these files installed.
