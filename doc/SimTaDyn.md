# Introduction to SimTaDyn

SimTaDyn: a dynamical spreadsheet for geographic simulation (SIMulateur TAbleur DYNamique in french).

_The following figure is a screenshot of the legacy version of SimTaDyn._
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/SimTaDyn.jpg)

## State of Art

SimTaDyn is a proof of concept for a geographic information system
(GIS) allowing to draw geographic maps, to edit their information and
formulas like done in a spreadsheet editor, and finally, execute the
map dynamically like dynamical systems.

A spreadsheet, like Excel, edits pages of regular and rectangular
pavement. Cells (aka tile) contain either a literal value or a
formula which performs an action on the cell itself with the content
of other cells.

A Geographic Information System (GIS) is an editor manipulating
geographic databases and draw their information. Datum are usually
not dynamic (in the way modifying one set over time will dynamically
change other datum).

The idea of SimTaDyn is to couple in a single tool a GIS with a
spreadsheet but the goal is to extend their ideas to a more general
case:

1. __The idea of spreadsheet is extended:__ In SimTaDyn the pavement
is more general and contains different kind of editable tiles (cells):

* Polygonal cells (like a geographic area) extend the regular and
  rectangular pavement of classic Excel spreadsheets.

* Edge cells (like roads or points of interest) extend the pavement of
  classic spreadsheets by allowing edges to be interactive as well
  polygonal cells (which is not the case on Excel).

* Node cells (like road crossings) extend the pavement of classic
  spreadsheets by allowing corners to be interactive (which
  is not the case on Excel).

This idea is summarized by the following picture:

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/pavement.png)

2. __The idea of spreadsheet scripting is extended:__ Like classic
spreadsheets, SimTaDyn has a script language for "gluing" cells
together by formulas. SimTaDyn extends with self-evolving scripting
language: the [Forth
language](https://en.wikipedia.org/wiki/Forth_(programming_language))
instead of a static language like Visual Basic.

Forth is a stack language (reverse polish notation), embedding its own
virtual machine (managing it's own memory, dictionary, byte code) and
its own interpreter (compiler). Forth is very powerful because because
it's a self-evolving language: the interpreter evolves dynamically
and can modify its own syntax.

3. __The idea of spreadsheet scripting is extended:__ A classic
spreadsheet only manages one literal value or a formulae. SimTaDyn
wants to work on a database: each cell can have more than one data and
at most one forth script as formulae. And thanks to Forth you can
write complex formulas like discrete time systems.

3. __The idea of spreadsheet scripting is extended:__ The Forth editor
of SimTaDyn allows to write and run two kind of scripts:

* Local scripts are stored inside cells like a classic spreadsheet.

* Global scripts are applied to the whole map (launching Forth script
on all cells), modifying its layout or information, ...

4.  __The idea of GIS is also extended:__ classic GIS manipulates two
kind of geographic maps.

* [Rasterized map](http://desktop.arcgis.com/en/arcmap/10.3/manage-data/raster-and-images/what-is-raster-data.htm)
  used for computations with a [map algebra](https://www.nrem.iastate.edu/files/w11-MapAlgebra_presentation5.pdf),

* [Vectorized map](http://gisgeography.com/spatial-data-types-vector-raster/) used
  for displaying static informations from databases.

SimTaDyn can be seen as a vectorized map manipulated like a rasterized
map. Each cell is a either a polygon, a polyline or dots but are at
the same time a cell with a script forth and an data from the
database.

5. __The idea of GIS is also extended:__ Because SimTaDyn uses OpenGL
and Forth has an evolving scripting language, we can display results
of a script in 3D instead of 2D.

## Typical SimTaDyn applications

Application with this SimTaDyn is not limited to GIS application, we
can imagine using SimTaDyn for the following applications:

* A pure classic spreadsheet, Excel-like, where nodes and arcs have no
  effects, zones are rectangular.

* A game engine for making a clone of the SimCity game. Or more
  seriously for resolving problem of traffic jam/congestion and
  [Wardrop's
  equilibrium](https://en.wikipedia.org/wiki/John_Glen_Wardrop).

* Solving partial differential equation like the video showing [SpaceX
  simulations](https://www.youtube.com/watch?v=vYA0f6R5KAI).

* A flowchart in the same idea of Lego Mindstorms: nodes are Forth
  code and edges the order sequences. Or blocks schematics for flow
  control like Simulink or Scicos.

As said previously, SimTaDyn is still a proof of concept and is still
looking for its way and GIS is one of the possible way.

## Origin of the project

SimTaDyn was at this origin a one-year free-subject school project
made in 2003-2004 at EPITA school for learning coding and working in
team. The project reborn in 2017, after a talk with a co-worker and I
decided to give this project a second life. The original code was so
messy that I decide to develop it again but "from scratch" and in C++
with cleaner code (I hope).

The SimTaDyn name comes from a joke mixing words SIM (for SimCity,
SIMulation), TA (for TABleur: spreadsheet in French), DYN (for
DYNamic) and sounding close to the project Sytadin a traffic jam
application where the name is also a joke to the French word citadine
(meaning city-dweller).

For frenchmen, you can read the [french PDF
manual](https://github.com/Lecrapouille/SimTaDyn/blob/release-EPITA-2004/doc/Simtadyn-Manuel.pdf)
written in 2004 as support for our teachers (badly written due to a
miss of time).

The initial code is available on the git branch [release-EPITA-2004 branch](https://github.com/Lecrapouille/SimTaDyn/tree/release-EPITA-2004).
The project was not fully finalized but functional. To compile it, you
will need to install libraries like MySQL, OpenGL (the legacy
version), GTK+2, gtkglext (OpenGL for GTK+2), ncurses and worked for
OSX with X11 and still working on actual Linux (but no longer on
recent OSX versions because GTK+/Opengl is no longer maintained).
