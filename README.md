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
a spreadsheet. The goal is to design dynamical geographic simulations.
Please, read this [introduction](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.md)
to understand better the idea of this project.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.jpg)
*This screenshot comes from the legacy version of SimTaDyn*

##### Table of contents:

* [Introduction to SimTaDyn](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/SimTaDyn.md)
* [SimTaDyn code architecture](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Architecture.md)
* [Compile and install SimTaDyn](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Install.md)
* [SimTaDyn known bugs](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Bugs.md)
* [Report bugs](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Bugs.md)
* [SimTaDyn GUI shortcuts](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Shortcuts.md)
* [SimTaDyn Forth scripting](https://github.com/Lecrapouille/SimTaDyn/blob/development/src/forth/doc/forth.md)
* [Forth standalone](https://github.com/Lecrapouille/SimTaDyn/blob/development/src/forth/doc/standalone.md)
* [Credits](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/Credits.md)

##### Note concerning git branches

SimTaDyn was originally a one-year student project made in 2004 but in
2017 I decided to reborn it and rewrite it from scratch:
* With [OpenSUSE Build](https://build.opensuse.org/repositories/home:Lecrapouille/SimTaDyn)
I'm trying to compile this project into different distribution packages (for the moment only deb
packages, not yet RPM packages, feel free to contribute).
* The [master branch](https://github.com/Lecrapouille/SimTaDyn/tree/master) peaks
stable releases from the development git branch (2017+). Code source of releases can also be
[downloaded directly](https://github.com/Lecrapouille/SimTaDyn/releases).
* The development branch is a total rework (and still in gestation) of the branch
[release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004)
but currently there is less features than the 2004 branch.
* When you git clone the project, you will be in the development branch
(not on the git master !). This branch is the default for developers. With this branch, the
build may be broken due temporary experiments.
* Because of miss of time (I working after working hours) I prefer pushing partial code in temporary
`dev-` branches and pick them in the developement branch.
* If you just want to see what the project can do, use the
[release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004)
* This version crashes if the map is not currently bind to a mySQL
table (create one in local). It would have been better to catch this
"no connection error" and display instead an error ! But this branch
will never be developed anymore and you currently cannot save maps.

##### What is have been done ?
* Basic utilities for the project (container, log, maths, patterns ...).
* Basic Forth interpreter (can runs simple code).
* Basic spreadsheet parser (compute simple sheets with basic Forth formulas).
* Graph structure (node, arcs, zones).
* Basic OpenGL wrapper (scene graph).
* Basic GTKmm GUI (OpenGL view, Forth editor, map tools).
* Continuous integration (Travis-CI).

##### What is currently in progress ?
* OpenGL wrapper version 2: allows to write in few line 3D scenes (like glumpy).
* Forth interpreter version 2: using computed goto (like gforth).
* SimTaDyn file loader (SimTaDyn file format) and resource manager
(a document has several maps).
* GUI tools for map edition (add, remove nodes, arcs, edit cells).
* Map renderer.

##### What is has to be done ?
* Got a complete Forth core system (high level words),
* Upgrade Forth for GIS edition and spreadsheet manipulation (got a basic application).
* Got a basic but functional GUI with principal tools (creating a complete map).
* Link the map with a database (each cells can have several fields).
* 2D/3D view of the map (2d for edition, 3d for the simulation).
* Can edit and run large maps.
