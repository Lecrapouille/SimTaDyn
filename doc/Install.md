# SimTaDyn Installation Guide

This page explains how to compile the code source of the master branch. See other install files given on other git branches (specially the Original-Version-EPITA-2004 branch).

SimTaDyn is based on C++11, Modern OpenGL (>= 3.0), The C++ version of GTK+, the C++ version of GtkSourceView. In addition it needs the gtkmm >= 3.22 to work correctly. This choice was due to thee fact that on 2004 I enjoyed programming GTK+ and OpenGL was well integrated (with GtkGLExt) but in 2016, I get bored of C and decide to pass to C++ and after 12 years of absence, I did not realized soon that GTK+ stopped not maintaining correctly OpenGL.

- Installation
  - Linux
    - [Ubuntu](#ubuntu)
  - Mac OSX
    - [OS X](#os-x)
  - Windows
    - [Not yet in development](#other-architecture)
- [Compilation](#compilation)
- [Run](#run)

## Ubuntu

Install dependencies for the master branch:
```sh
sudo apt-get install libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev
```

Note: if you prefer compiling the legacy version of SimTaDyn, install the gtkmm-2.4 instead of gtkmm-3.0 (both versions can co-exist on your system):
```sh
sudo libgtkglextmm-x11-1.2-dev libgtkmm-2.4-dev
```

Optionally, for launching unit test, install the following packages:
```sh
libcppunit-dev gcovr libglm-dev
```

And even more optionally for generating some expected unit test results, install the Scilab fork: [ScicosLab](http://www.scicoslab.org/)
```sh
sudo apt-get install scicoslab-gtk
```

Concerning libgtkmm-3.0-dev: t's mandatory to install the gtkmm version 3.22 due to a major bug with the OpenGL widget breaking the OpenGL context when using inheritance. See this [bug](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Bugs.md). for more informations. This version is not present on Ubuntu older than 17.04. That's mean on Ubuntu version (16.04, 16.11) you’ll have to install it by compiling sources by yourself (need long time to compile but the process compiles well).

If needed, for fixing some Gtk-Message: Failed to load module "overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

## Mac OS X

SimTaDyn compiles on a MacBook Air and Mac OS X 10.9.5. To install gtkmm and other dependencies use the tool [homebrew](https://brew.sh/index_fr.html). The current gtkmm version is 3.22 and no problem occurred during the compilation of these libs. however the Gtk::GlArea is not implemented on this architecture and by consequence the OpenGL context is not created.

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
