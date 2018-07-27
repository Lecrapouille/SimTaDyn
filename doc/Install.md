# SimTaDyn Compilation Guide

Table of contents:
- [Steps for compiling SimTaDyn](#steps-for-compiling-simtadyn)
- [Installing packages that SimTaDyn depends on](#installing-packages-that-simtadyn-depends-on)
  - [Ubuntu](#ubuntu)
  - [OpenSUSE](#opensuse)
  - [Debian](#debian)
  - [OS X](#mac-os-x)
  - [Windows](#other-architecture)
  - [Docker](#docker)
- [Developers](#developers)
  - [RPM and debian packages](#rpm-and-debian-packages)
  - [Create RPM and Debian packages](#create-rpm-and-debian-packages)
  - [Compiling SimTaDyn unit tests](#compiling-simtadyn-unit-tests)
  - [Makefile options](#makefile-options)

## Steps for compiling SimTaDyn

This document explains the different steps for getting, compiling the SimTaDyn code source take from the git
[master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master) and install it.

For other SimTaDyn branches, specially for the [release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004),
please read the INSTALL file associated to this branch.

##### Step 1: Install packages that SimTaDyn depends on:

SimTaDyn code is based on C++11, modern OpenGL (>= 3.3), gtkmm >= 3.22 and gtksourceviewmm
(which are C++ interfaces for GTK+ and GtkSourceView a boosted version of the GTK+ text editor)
and other small libraries like libSOIL (for loading image files) or zipper (a C++ manipulationg zip archives).

Click on the link matching your operating system, for knowing how to install these libraries:
* [Ubuntu](#ubuntu)
* [OpenSUSE](#opensuse)
* [Debian](#other-architecture)
* [OS X](#mac-os-x)
* [Windows](#other-architecture)
* [Docker](#docker)

##### Step 2: Clone the SimTaDyn code source and its external parts:

Step 2.1: download the SimTaDyn code source from github <img src="http://i.imgur.com/Cj4rMrS.gif" height="40" alt="Swimming Octocat" title="Games on GitHub">:

```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
```
A `SimTaDyn/` folder should have been created and will be refered as the root directory of your project.

Because not all libraries can be installed from disribution packages as explained in step 1,
you will have to download and compile them from github. For some reasons which will not be explained here,
SimTaDyn does use git submodule and therefore special options of the Makefile has to be called like
explained in step 2.20

Step 2.2: download third parts. This is made by two bash scripts which can be called by the main Makefile:
```sh
cd SimTaDyn/
make download-external-libs
make compile-external-libs
```

Note: As alternative, you can do this:
```sh
cd SimTaDyn/external/
./download-external-libs.sh Linux
./compile-external-libs.sh Linux
```

* If you are not working on Linux, but OSX or Windows: replace `Linux` by `Darwin` or `Windows`.
* These libraries are not installed in your environnement. In the case of you desire them, you have to go to each directories and type:
```sh
sudo make install
```

##### Step 3: Compile the SimTaDyn code source:

SimTaDyn does not use ./configure script. Call directly the Makefile:

```sh
cd SimTaDyn/
make -j4
```
Note:
* -jX is optional and is used for compiling faster the code source. X refers to the desired number of threads (usually 2 * number of cores of your computer. For example X = 2 * 2).

If you want to compile whith a different compilator use `CXX` like:
```sh
make CXX=clang++-3.9 -j4
```

##### Step 4: getting fun with SimTaDyn:
From code source:
```sh
cd SimTaDyn/
./build/SimTaDyn
```
or directly if SimTaDyn has been installed on your system:
```sh
SimTaDyn
```

Note: for the moment, SimTaDyn does not take options but will in the future.

##### Step 5: Install data that SimTaDyn depends on:

If you like SimTaDyn you can install it by typing:
```sh
sudo make install
```
By default:
* the binary will be placed in `/usr/bin` and named `SimTaDyn-<version>`
* its data and documentation placed in `/usr/share/SimTaDyn/<version>/`.
* <version> refers to the content of the file SimTaDyn/VERSION.
* External libraires needed by SimTaDyn will not be installed.

If you do not like the default location, Pass to Makefile options `DESTDIR`, `PREFIX` and `BINDIR` or directly edit the file `.makefile/Makefile.header` (note: touching a makfile file will force a whole project compilation).

## Installing packages that SimTaDyn depends on

Currently, gtkmm >= 3.22 is mandatory to get SimTaDyn worked correctly with OpenGL due to a bug in gtkmm solved in this version. Not all Ubuntu versions contains this version. In addition OpenGL is hardly supported by GTK+, a shame !

### Ubuntu

#### Ubuntu >= 17.04

For Ubuntu >= 17.04, gtkmm >= 3.22 is available. Install directly the following packages (this will take you around 1 GB in your disk space):
```sh
sudo apt-get update && apt-get install cmake libgtksourceviewmm-3.0-dev freeglut3 freeglut3-dev mesa-common-dev libglu1-mesa-dev libglew-dev libglfw3-dev libdw-dev
```
If needed and for fixing some Gtk-Message like: "Failed to load module overlay-scrollbar":
```sh
sudo apt-get install overlay-scrollbar*
```

#### Ubuntu < 17.04

For Ubuntu versions < 17.04, gtkmm >= 3.22 is not available. If you have a 64-bits machine you could install [Docker](https://en.wikipedia.org/wiki/Docker_(software)), pull an image of Ubuntu 17.04 and inside the docker, follow instructions [described in below section](#docker).

If you have a 32-bits (which is my case), unfortunately Docker does not support 32-bits architecture, so you will have to compile by yourself, GTK+ and gtkmm source code. [Link here](http://www.linuxfromscratch.org/blfs/view/svn/x/gtk3.html).

### Mac OS X

SimTaDyn compiles on a MacBook Air and Mac OS X (tested on version 10.9.5). To install gtkmm and other dependencies use the tool [homebrew](https://brew.sh/index_fr.html). The current gtkmm version is 3.22 and no problem occurred during the compilation of these libs. __However the Gtk::GlArea is not fully implemented on this architecture and by consequence the OpenGL context is not created ! I'm sorry it does not depend on me but on gtk+ developpers (for memory, Legacy OpenGL worked perfectly on gtk+2 with gtkglext on 2003 with OSX (see [release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004) but again gtk+ developpers seems have made regressions because the whole gtk window is black (even butons) !)__

### Debian

Is similar to Ubuntu >= 17.04, so please refer to Ubuntu section.
The recommended Debian version is >= 9.3 which contains the correct Gtk version.

### OpenSUSE

Install the following packages:
```
gcc-c++ gtksourceviewmm-devel freeglut-devel glew-devel libdw-devel Mesa-devel Mesa-libGLU-devel libglfw-devel
```

Note: I cannot personally test SimTaDyn on this plateform.

### Other architecture

* I cannot yet personally test on SimTaDyn on distribution like OpenSUSE, Red Hat, Slackware, Gentoo.
* Window: Not done and no development scheduled yet. Indeed, I only have a XP and Cygwin stopped being ported for Windows XP and so I need higher Window version, which it's not my case currently.
Feel free to help me on this point (I have an Appveyor script for my continous inegration but this script fails during the compilation of the external lib: zipper).

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
- Compile and run SimTaDyn like described in previous sections.
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

## Developers

These sections are optional for users but can be useful for developers. Install the following packages:
```sh
sudo apt-get update && apt-get install libcppunit-dev gcovr libglm-dev llvm-4.0-tools osc npm
sudo npm install markdown-to-html -g
```

### RPM and debian packages.

(in gestation) SimTaDyn uses the OpenSUSE Build Service for building the project on different distributions and architectures.
Go [here](https://build.opensuse.org/project/show/home:Lecrapouille) for downloading packages.
Not all distrubutions are yet made but will be done in the future. I cannot test all of them. So any feeback is welcome.

### Create RPM and Debian packages

* `make obs` for launching RPM and debian packages compilation with OpenSUSE Build Service. Because you'll need my password (that I'll do not give)
you should adapt the script `SimTaDyn/.integration/opensuse-build-service.sh` for your own repo.

### Compiling SimTaDyn unit tests

Address Sanitizer (aka ASAN) displays the stack trace with addresses
instead of file names and file lines. For human readable traces ASAN
needs llvm-symbolizer. The drawback with llvm-symbolizer in Ubuntu
does not create a symbolic link to the current version. Therefore
you'll have to do by yourself. For example, in Ubuntu:
```
sudo ln -s /usr/bin/llvm-symbolizer-4.0 /usr/bin/llvm-symbolizer
```
Where `/usr/bin/` should be adapted to your environment and be set to your `PATH`. For example
```
echo $PATH
export PATH=/usr/bin:$PATH
echo $PATH
```

And optionally, for generating some expected unit test results, install the Scilab fork: [ScicosLab](http://www.scicoslab.org/)
```sh
sudo apt-get update && apt-get install scicoslab-gtk
```

To compile and launch unit tests, type the following commands:
```sh
cd SimTaDyn/tests
make -j4
./build/UnitTest
```
Or directly:
```
make unit-test
```
If you have installed gcovr, generate html displaying the code coverage in a single Makefile command:
```sh
make coverage
```
The HTML files are generated in SimTaDyn/doc/coverage. Open the index.html with any kind of browser (like Firefox).

Note 1: Because Travis-CI does not allow OpenGL context, OpenGL unit tests are disable for continous intrgation.
Note 2: With Travis-CI, code coverage is also made (without OpenGL unit tests). See [here](https://coveralls.io/github/Lecrapouille/SimTaDyn?branch=master)

### Makefile options

Summary for developpers !

* Type `make help` for displaying makefile options. Here main options:
* `download-external-libs` Download external projects that SimTaDyn needs.
* `compile-external-libs`  Compile external projects that SimTaDyn needs.
* `coverity-scan` Clean and compile again the project for static analysis of the code.
An archive `SimTaDyn.tgz` is created and can be uploaded into the service [Coverity Scan](https://scan.coverity.com/projects/lecrapouille-simtadyn).
* `unit-tests`Compile and launch unit tests. Then the generate code coverage report.
* `coverage` Generate the code coverage html rapport.
* `run` Launch the SimTaDyn executable with address sanitizer (if enabled) for dynamic analysis of the code (memory access violation).
* `doc` Generate the code source documentation with doxygen.
* `targz` Compress the code source into a unique tarball for backup (meaning if a backup tarball already exists it will stay intact).
.git/, build/, generated documentation and other backup tarballs are ignored. The tarball name is SimTaDyn-<VERSION>-<YEAR>-<MONTH>-<DAY>-v<RELEASE>.
* `obs` Create and upload a tarball to the OBS [OpenSuse Build Service](https://build.opensuse.org/project/show/home:Lecrapouille).
* `install` Install SimTaDyn project and its data in your system. You need to be root user.
* `uninstall` Uninstall the project. You need to be root. Please be care of potential errors because it will call `rm` command.
* `clean` Clean the build/ directory.
* `veryclean` Clean the whole project.
* `which-gcc` Display the compilator version and informations.

Note: Modifying a makefile will automaticly force a complete project compilation.
