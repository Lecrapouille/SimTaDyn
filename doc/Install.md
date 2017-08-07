# SimTaDyn Compilation Guide

Table of contents:
- [Steps for compiling SimTaDyn](#steps-for-compiling-simtadyn)
- [Installing packages that SimTaDyn depends on.](#install-packages-that-simtadyn-depends-on)
  - [Ubuntu](#ubuntu)
  - [OS X](#mac-os-x)
  - [Debian](#other-architecture)
  - [Windows](#other-architecture)
- [Docker](#docker)
- [Compiling SimTaDyn unit tests](#compiling-simtadyn-unit-tests)
- [Building a debian package](#building-a-debian-package)
- [PPA](#ppa)

## Steps for compiling SimTaDyn

This section explains how to compile SimTaDyn code source taken from the git [master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master). For other SimTaDyn branches (specially for the [Original-Version-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/Original-Version-EPITA-2004)), read the install file associated to the branch.

##### Step 1: Install packages that SimTaDyn depends on:

SimTaDyn is based on C++11, modern OpenGL (>= 3.3), gtkmm >= 3.22 and gtksourceviewmm (which are C++ interfaces for GTK+ and GtkSourceView a boosted version of the GTK+ text editor) and other small libraries like libSOIL (for loading image files). Click on the link matching your operating system:
* [Ubuntu](#ubuntu)
* [OS X](#mac-os-x)
* [Docker](#docker)
* [Debian](#other-architecture)
* [Windows](#other-architecture)

##### Step 2: Clone the SimTaDyn code source:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
```
A SimTaDyn folder should have been created.

##### Step 3: Clone libraries code source that SimTaDyn depends on:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
cd SimTaDyn/external
./gitclone.sh
```
This will clone and compile libs.

##### Step 4: Compile SimTaDyn code source:
```sh
cd ..
make -j4
```
Note: SimTaDyn does use ./configure script. Call directly the Makefile. -j4 is optional and is used for compiling faster the code source. It refers to the desired number of threads (usually 2 * number of cores of your computer).

##### Step 5: Install data that SimTaDyn depends on:
```sh
sudo make install
```
By default, the runable will be placed in /usr/bin and data in /usr/share/SimTaDyn. If you do not want these locations, please edit the file
```sh
SimTaDyn/.makefile/Makefile.helper
```

##### Step 6: getting fun with SimTaDyn:
```sh
SimTaDyn
```

Note: SimTaDyn does have command line options for the moment. Be sure you typed ``make install`` like described in the previous step before launching the executable. SimTaDyn needs these files installed to run correctly.

## Installing packages that SimTaDyn depends on

Currently, gtkmm >= 3.22 is mandatory to get SimTaDyn worked correctly with OpenGL due to a bug in gtkmm solved in this version. Not all Ubuntu versions contains this version. In addition OpenGL is hardly supported by GTK+, a shame !

### Ubuntu

#### Ubuntu >= 17.04

For Ubuntu >= 17.04, gtkmm >= 3.22 is available. Install the following packages:
```sh
sudo apt-get update && apt-get install libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev libsoil-dev binutils-dev git
```
Note: this will take you around 1 GB in your disk space.

If needed and for fixing some Gtk-Message like: "Failed to load module overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

#### Ubuntu < 17.04

For Ubuntu versions < 17.04, gtkmm >= 3.22 is not available. If you have a 64-bits machine you could install [Docker](https://en.wikipedia.org/wiki/Docker_(software)), pull an image of Ubuntu 17.04 and inside the docker, follow instructions [described in below section](#docker).

If you have a 32-bits (which is my case), unfortunately Docker does not support 32-bits architecture, so you will have to compile by yourself, GTK+ and gtkmm source code. [Link here](http://www.linuxfromscratch.org/blfs/view/svn/x/gtk3.html).

### Mac OS X

SimTaDyn compiles on a MacBook Air and Mac OS X (tested on version 10.9.5). To install gtkmm and other dependencies use the tool [homebrew](https://brew.sh/index_fr.html). The current gtkmm version is 3.22 and no problem occurred during the compilation of these libs. However the Gtk::GlArea is not implemented on this architecture and by consequence the OpenGL context is not created !

### Other architecture

Debian: not tested but shall be closed to Ubuntu.

Window: Not done and no development scheduled yet. Indeed Cygwin stopped being ported on Windows XP and so you need higher Window version, which it's not my case currently.
Feel free to help me on this point.

## Docker

This section is for people having an Ubuntu < 17.04 or preferring to test SimTaDyn inside a docker. If you have a 64-bits machine you can install [Docker](https://en.wikipedia.org/wiki/Docker_(software)), pull an image of Ubuntu 17.04 and inside the docker, follow instructions [described in below section](#docker).

If you have a 32-bits (which is my case, I have to use a computer not owned by me, so I cannot help a lot on docker subject), unfortunately Docker does not support 32-bits architecture and you have to find another way for compling SimTaDyn.

##### Step 1: Install docker image:

If you have not a docker installed, you can [follow these steps](https://www.digitalocean.com/community/tutorials/how-to-install-and-use-docker-on-ubuntu-16-04).

Then download an official image of Ubuntu 17.04:
```sh
docker pull ubuntu:17.04
```

##### Step 2: Clone SimTaDyn:

And go inside SimTaDyn folder, this will be the entry point for your docker:
```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
cd SimTaDyn
```

##### Step 3: Launch the docker:

Launch the docker with an interactive bash where the root of the docker will contain the root of the SimTaDyn project:

```sh
docker run --rm -e DISPLAY=$DISPLAY -v "$(pwd):/SimTaDyn" -e LOCAL_USER_ID=`id -u $USER` -it ubuntu:17.04 /bin/bash
```

You should see something like:
```sh
root@524a5c276943:/#
```

Note that you are a root user located at the / directory (root) in your docker. 7471cd1db3fa is the unique ID referring to your current image. Your value shall differ from this example. You can get unique ID of all your images with the following command:
```sh
docker ps -a
```

##### Step 4: Installing packages:

Now that you are inside the docker, if you type the ls command, you'll see the SimTaDyn directory. Go inside it. Now, still inside the docker, follow:
- [these steps](#ubuntu) for installing libraries for SimTaDyn. Note that do not have to type the sudo command because you are already the root user.
- Compile and run SimTaDyn like described in next sections.
- [Read this article](https://linuxmeerkat.wordpress.com/2014/10/17/running-a-gui-application-in-a-docker-container/) for getting the graphic display. Here its summary:
- Install the Xvfb package and configure it:
```sh
apt-get install xvfb
Xvfb :1 -screen 0 1024x768x16 -nolisten tcp &
export DISPLAY=":1.0"
```
- do not yet exit the docker but read the next step !

Note: An alternative to Xvfb (not working well in my case)
```sh
docker run --rm -e DISPLAY=$DISPLAY -v "$(pwd):/SimTaDyn" -v /tmp/.X11-unix:/tmp/.X11-unix -e DISPLAY=unix$DISPLAY -e LOCAL_USER_ID=`id -u $USER` -it ubuntu:17.04 /bin/bash
```

##### Step 5: Save your docker image:

You have to know that Docker creates a dropable copy of the image (here Ubuntu 17.04), so your current image will be destroyed when leaving the docker (command exit). If you did that you will have to install all packages that SimTaDyn depends on.

To avoid that, save your docker image (around 1 Go).
```sh
docker commit 524a5c276943 test/simtadyn:0.2
```

Where 524a5c276943 shall be your image unique ID and where test/simtadyn:0.2 is an example of name (give a better name in your case).

##### Step 6: Call your SimTaDyn image:

You can exit your docker image and run up the container but this time with your new image.
```sh
docker run --rm -e DISPLAY=$DISPLAY -v "$(pwd):/SimTaDyn" -e LOCAL_USER_ID=`id -u $USER` -it test/simtadyn:0.2 /bin/bash
```

## Compiling SimTaDyn unit tests

This section is optional for users but can be useful for developers. Install the following packages:
```sh
sudo apt-get update && apt-get install libcppunit-dev gcovr libglm-dev
```

And optionally, for generating some expected unit test results, install the Scilab fork: [ScicosLab](http://www.scicoslab.org/)
```sh
sudo apt-get update && apt-get install scicoslab-gtk
```

The compilation of unit tests is made by the following command:
```sh
cd SimTaDyn/tests
make -j4
```

For launching tests:
```sh
./build/UnitTest
```

If you have installed gcovr, you can compile code, launch tests and generate html showing code coverage in a single Makefile command:
```sh
cd SimTaDyn/tests
make coverage -j4
```

The HTML files are generated in SimTaDyn/doc/coverage. Open the index.html with any kind of browser (like Firefox).

## Building a debian package.

Install the checkinstall tool which will create a package from source and binary:
```sh
sudo apt-get update && apt-get install checkinstall
```

Launch the makefile rule:
```sh
cd SimTaDyn
sudo make package
```

A .deb file shall have been created.

## PPA

SimTaDyn cannot yet been installed with Ubuntu or Debian package managers while packages can be generated.
