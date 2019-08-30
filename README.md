# SimTaDyn

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://github.com/Lecrapouille/SimTaDyn/blob/master/LICENSE)
[![Coverity scan](https://scan.coverity.com/projects/13000/badge.svg)](https://scan.coverity.com/projects/lecrapouille-simtadyn)
[![Coverage Status](https://coveralls.io/repos/github/Lecrapouille/SimTaDyn/badge.svg?branch=development)](https://coveralls.io/github/Lecrapouille/SimTaDyn?branch=development)

|Branch     | **`Linux/Mac OS`** | **`Windows`** | **`openSUSE Build`** |
|-----------|------------------|-------------|----------------|
|master     |[![Build Status](https://travis-ci.org/Lecrapouille/SimTaDyn.svg?branch=master)](https://travis-ci.org/Lecrapouille/SimTaDyn)|[![Build status](https://ci.appveyor.com/api/projects/status/github/lecrapouille/simtadyn?svg=true)](https://ci.appveyor.com/project/Lecrapouille/SimTaDyn)|[compiled versions](https://build.opensuse.org/repositories/home:Lecrapouille/SimTaDyn)|
|development|[![Build Status](https://travis-ci.org/Lecrapouille/SimTaDyn.svg?branch=development)](https://travis-ci.org/Lecrapouille/SimTaDyn)|||

SimTaDyn is a proof of concept project for a geographic information
system (GIS) allowing to draw geographic maps and manipulate them like
a spreadsheet (Excel). The goal is to add formulas in cells and design dynamical geographic simulations (like SimCity game). Please, read this [introduction](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.md) to understand better the idea of this project.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.jpg)
*This screenshot comes from the legacy version of SimTaDyn*

#### Table of contents:

* [Introduction to SimTaDyn](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.md)
* [SimTaDyn code architecture](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Architecture.md)
* [Compile and install SimTaDyn](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Install.md)
* [SimTaDyn known bugs](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Bugs.md)
* [Report bugs](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Bugs.md)
* [SimTaDyn GUI shortcuts](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Shortcuts.md)
* [SimTaDyn Forth scripting](https://github.com/Lecrapouille/SimTaDyn/blob/development/src/forth/doc/forth.md)
* [Forth standalone](https://github.com/Lecrapouille/SimTaDyn/blob/development/src/forth/doc/standalone.md)
* [Credits](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Credits.md)

#### Note concerning git branches

SimTaDyn was originally a one-year student project made in 2004 [release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004). In 2017 I decided to reborn it and rewrite it from scratch
[master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master) and 
[development branch](https://github.com/Lecrapouille/SimTaDyn/tree/development).

* If you just want to see what the project can do, use the
[release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004). This branch is no longer developed but is
enough functional and show more functionalities than the master branch.
You will have a better idea of the idea of SimTaDyn. This version crashes if the map is not currently bind to a mySQL table (create one in local). It would have been better to catch this "no connection error" and display instead an error ! You cannot save maps or develop concrete GIS applications.

* The [development branch](https://github.com/Lecrapouille/SimTaDyn/tree/development) is a total rework (and still in gestation) of the branch
release-EPITA-2004 branch but currently there is less features than the 2004 branch. When you git clone the project, you will be in the development branch (not on the git master!). This branch is the default for developers. With this branch, the build may be broken due temporary experiments and miss of time. I prefer pushing partial code in temporary `dev-` branches and pick them in the development branch.

* The [master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master) peaks stable releases from the development git branch (2017+). Code source of releases can also be [downloaded directly](https://github.com/Lecrapouille/SimTaDyn/releases). With [OpenSUSE Build](https://build.opensuse.org/repositories/home:Lecrapouille/SimTaDyn)
I'm trying to compile this project into different distribution packages (for the moment only deb packages, not yet RPM packages, so feel free to contribute).

#### What is have been done ?

* Basic utilities for the project (container, log, maths, patterns ...).
* Basic Forth interpreter (can runs very basic scripts).
* Basic spreadsheet parser (compute simple sheets with basic Forth formulas).
* Graph structure (node, arcs, zones), Map, sheet internal structures.
* Basic loader and resource managers.
* Scene graph and Basic OpenGL wrapper allowing to write in few line 3D OpenGL-Core scenes (like glumpy).
* Basic GTKmm GUI (OpenGL view, Forth editor, map tools, inspectors).
* Continuous integration (Travis-CI).

#### What is currently in progress ?

GUI integrating OpenGL, Forth and Maps:
* Add GUI tools for map edition: add, remove nodes, arcs, edit cells, undo, redo.
* OpenGL wrapper: add routines for drawing maps and graphes.
* Map renderering.

#### What is has to be done ?

* Complete Forth core system (high level words, GIS and spreadsheet manipulation
* Link the map with a database (each cells can have several fields).
* 2D/3D view of the map (2d for edition, 3d for the simulation).
* Create a basic application.
