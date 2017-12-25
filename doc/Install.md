# SimTaDyn Compilation Guide

Table of contents:
- [Steps for compiling SimTaDyn](#steps-for-compiling-simtadyn)
- [Installing packages that SimTaDyn depends on](#installing-packages-that-simtadyn-depends-on)
  - [Ubuntu](#ubuntu)
  - [OpenSUSE](#opensuse)
  - [Debian](#other-architecture)
  - [OS X](#mac-os-x)
  - [Windows](#other-architecture)
  - [Docker](#docker)
- [RPM and debian packages](#rpm-and-debian-packages)
- [Developers](#developers)
  - [Create RPM and Debian packages](#create-rpm-and-debian-packages)
  - [Compiling SimTaDyn unit tests](#compiling-simtadyn-unit-tests)
  - [Makefile options](#makefile-options)

## Steps for compiling SimTaDyn

This section explains the differents steps for compiling the SimTaDyn code source taken from the git [master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master). For other SimTaDyn branches (specially for the [Original-Version-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/Original-Version-EPITA-2004)), read the install file associated to the branch.

##### Step 1: Install packages that SimTaDyn depends on:

SimTaDyn is based on C++11, modern OpenGL (>= 3.3), gtkmm >= 3.22 and gtksourceviewmm (which are C++ interfaces for GTK+ and GtkSourceView a boosted version of the GTK+ text editor) and other small libraries like libSOIL (for loading image files) or zipper (a C++ manipulationg zip archives).

Click on the link matching your operating system, for knowing how to install these libs:
* [Ubuntu](#ubuntu)
* [OpenSUSE](#opensuse)
* [Debian](#other-architecture)
* [OS X](#mac-os-x)
* [Windows](#other-architecture)
* [Docker](#docker)

##### Step 2: Clone the SimTaDyn code source:

Download the SimTaDyn code source from github <img src="http://i.imgur.com/Cj4rMrS.gif" height="40" alt="Swimming Octocat" title="Games on GitHub">:

```sh
git clone https://github.com/Lecrapouille/SimTaDyn.git
```
A SimTaDyn folder should have been created. If not working install git before.

##### Step 3: Clone libraries code source that SimTaDyn depends on:

Not all libraries are available on your disribution (aka cannot be installed directly like shown in step 1). You have to download and compile them. This is made by two bash scripts called by the makefile:

```sh
cd SimTaDyn
make download-external-lib
make compile-external-libs
```
Note: These libraries are not install into your environnement. If you desired them, you have to go to each directories and type:

```sh
sudo make install
```

##### Step 4: Compile the SimTaDyn code source:

SimTaDyn does not use ./configure script. Call directly the Makefile:

```sh
cd ..
make -j4
```
Note: -jX is optional and is used for compiling faster the code source. X refers to the desired number of threads (usually 2 * number of cores of your computer. For example X = 2 * 2).

##### Step 5: getting fun with SimTaDyn:
```sh
./build/SimTaDyn
```

Note: SimTaDyn does take options for the moment.

##### Step 6: Install data that SimTaDyn depends on:

If you like SimTaDyn you can install it by typing:
```sh
sudo make install
```
By default:
* the binary will be placed in `/usr/bin` and named `SimTaDyn-<version>`
* its data and documentation placed in `/usr/share/SimTaDyn/<version>`.

If you do not like the default location, Give to Makefile options `DESTDIR`, `PREFIX` and `BINDIR` or directly edit the file `.makefile/Makefile.header` (note: touching a makfile file will force a whole project compilation).

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

SimTaDyn compiles on a MacBook Air and Mac OS X (tested on version 10.9.5). To install gtkmm and other dependencies use the tool [homebrew](https://brew.sh/index_fr.html). The current gtkmm version is 3.22 and no problem occurred during the compilation of these libs. __However the Gtk::GlArea is not fully implemented on this architecture and by consequence the OpenGL context is not created ! I'm sorry it does not depend on me but on gtk+ developpers (for memory, Legacy OpenGL worked perfectly on gtk+2 with gtkglext on 2003 with OSX (see [Original-Version-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/Original-Version-EPITA-2004) but again gtk+ developpers seems have made regressions because the whole gtk window is black (even butons) !)__

### OpenSUSE

Install the following packages:
```
gcc-c++ gtksourceviewmm-devel freeglut-devel glew-devel libdw-devel Mesa-devel Mesa-libGLU-devel libglfw-devel
```
I cannot personally test on SimTaDyn on this plateform.

### Other architecture

* Debian: I cannot personally test on SimTaDyn on this plateform.
* Window: Not done and no development scheduled yet. Indeed Cygwin stopped being ported on Windows XP and so you need higher Window version, which it's not my case currently.
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

## RPM and debian packages.

(in gestation) SimTaDyn uses the OpenSUSE Build Service for building the project on different distributions and architectures.
Go [here](https://build.opensuse.org/project/show/home:Lecrapouille) for downloading packages.
Not all distrubutions are yet made but will be done in the future. I cannot yet test them. So any feeback is welcome.

## Developers

These sections are optional for users but can be useful for developers.

### Create RPM and Debian packages

* `make obs` for launching RPM and debian packages compilation with OpenSUSE Build Service.

### Compiling SimTaDyn unit tests

Install the following packages:
```sh
sudo apt-get update && apt-get install libcppunit-dev gcovr libglm-dev llvm-4.0-tools osc npm
sudo npm install markdown-to-html -g
```

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

Still for developpers:
* `coverity-scan` create an archive to be downloaded into [Coverity Scan](https://scan.coverity.com/projects/lecrapouille-simtadyn) for a static analysis of the code.
* `run` run the binary with adress sanitizer (or valgrind) for dynamic analysis of the code (memory access violation).
* `make targz` create a backup of SimTaDyn code (without .git, generated doc files or buid files) with the date. If an archive already exists the first is not smashed and a second archive is created.
