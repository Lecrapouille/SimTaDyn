# Introduction to SimTaDyn

SimTaDyn: a dynamical spreadsheet for geographic simulation (SIMulateur TAbleur DYNamique in french).

_The following figure is a screenshot of the legacy version of SimTaDyn._
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/SimTaDyn.jpg)

## State of Art
SimTaDyn is a proof of concept for a geographic information system
(GIS) allowing to draw geographic maps, to edit their informations like
done in a spreadsheet pages, and finally run dynamical computations on them
like if they were dynamical systems.

The idea of SimTaDyn is to couple in a single tool a GIS with a
spreadsheet but extends to more general cases:

* A spreadsheet, like Excel, edits pages of regular and rectangular
pavement. Cells contain either data or a computation function which
perform actions on the cell itself or on the content of other cells.

* A Geographic Information System (GIS) is an editor manipulating
geographic databases and draw them. Datum are usually static (in the
way modifying one set over time will dynamically change other datum).

1. The idea of the classic spreadsheet is extended. In SimTaDyn the
pavement is more general and contains different kind of editable
cells (tiles):

* Polygonal cells (like a geographic area) extend the regular and
  rectangular pavement of classic spreadsheets.
* Edge cells (like roads) extend the pavement of classic spreadsheets
  by allowing edges to be interactive as well polygonal cells.
* Node cells (like road crossings) extend the pavement of classic
  spreadsheets by allowing corners to be interactive.

This idea is summarized by the following picture:
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/pavement.png)

2. Like classic spreadsheets, SimTaDyn has a script language for
"gluing" cells together by formulas. When once has its value changed,
other cells are notified through their relation given by the
formula. Instead of using a Visual Basic, SimTaDyn uses the [Forth
language](https://en.wikipedia.org/wiki/Forth_(programming_language))
which is a stack language (reverse polish notation), embedding its
virtual machine (managing it's own memory, dictionary, byte code) and
its own interpreter (compiler). Forth is very powerful because it's a
self-evoluting language: the interpreter evolves dynamically and can
modify its own syntax.

3. The Forth editor of SimTaDyn allows to write and run two kind of scripts:
* Local scripts are stored inside the cell like the classic spreadsheet.
* Global scripts are applied to the map (launching Forth script of cells),
modifying it (its layout, informations, ...).

4. The idea of GIS is also extended. Classic GIS manipulates two kind of geographic maps.
* [Rasterized map](http://desktop.arcgis.com/en/arcmap/10.3/manage-data/raster-and-images/what-is-raster-data.htm)
  used for computations with a [map algebra](https://www.nrem.iastate.edu/files/w11-MapAlgebra_presentation5.pdf),
* [Vectorized map](http://gisgeography.com/spatial-data-types-vector-raster/) used
  for displaying static informations from databases.

SimTaDyn can be seen as a vectorized map manipulated like a rasterized map. Each
cell is a either a polygon, a polyline or dots but are at the same time a script forth and an element of the database.

* Because SimTaDyn uses OpenGL and a evolutive scripting language, we can display results of a script in 3D instead of 2D.

## Typical SimTaDyn applications
Application with this SimTaDyn is not limited to GIS application, we can image the
following applications:

* A pure classic spreadsheet where nodes and arcs have no effects, zones are
  rectangular.

* A game engine for making a clone of the SimCity game. Or more serious for resolving problem of traffic jam/congestion and [Wardrop's equilibrium](https://en.wikipedia.org/wiki/John_Glen_Wardrop).

* Solving partial differential equation like the video showing [SpaceX simulations](https://www.youtube.com/watch?v=vYA0f6R5KAI).

* A flowchart in the same idea of Lego Mindstorms: nodes are Forth code
  and edges the order sequences. Or blocks schematics for flow
  control like Simulink or Scicos.

As said previously, SimTaDyn is still a proof of concept and is still looking for its way.

## Origin of the project
SimTaDyn was at this origin a free-subject school project made in 2003-2004 for students to learn coding. The name comes from a joke mixing words SIM (for SimCity, SIMulation), TA (for TABleur: spreadsheet in French), DYN (for DYNamic) and sounding close to the project Sytadin a traffic jam application where the name is also a joke to the French word citadine (meaning city-dweller).

For frenchmen, you can read the [french PDF manual](https://github.com/Lecrapouille/SimTaDyn/blob/Original-Version-EPITA-2004/doc/Simtadyn-Manuel.pdf)
written in 2004 as support for our teachers (badly written due to miss of time).

The initial code is available on the git branch [Original-Version-EPITA-2004](https://github.com/Lecrapouille/SimTaDyn/tree/Original-Version-EPITA-2004). The project was not finalized (from what written in its document) but functional. To compile it, you will need to install lib like MySQL, OpenGL (the legacy version), GTK+2, gtkglext, ncurses and worked for OSX with X11 and still working on actual Linux.

The project reborn in 2016, after a talk with a co-worker and I decided to develop "from scratch" in C++ with cleaner code (I hope) because the original code is messy. You can get it on the master branch. Note: this project is still in development and still is a proof of concept.

