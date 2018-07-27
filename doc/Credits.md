# Credits

SimTaDyn code source gots its inspiration from the following documents and code source:

## GIS as Spreadsheet

* [Excel GIS](http://scholarworks.umb.edu/cgi/viewcontent.cgi?article=1004&context=management_wp)
The project with the closest idea to my project but here used as rasterized map.

* [Map Algebra](https://www.nrem.iastate.edu/files/w11-MapAlgebra_presentation5.pdf)
The algebra used in rasterized maps.

## Spreadsheet

* https://github.com/rohitsinha54/Spreadsheet-Calculator
A very basic textual spreadsheet calculator made in Java with circular references detection.
Formuleas use the reverse polish notation.

* http://www.cs.sjsu.edu/~pearce/modules/cases/ss/ss.htm
Explain how cells use the pattern observer for notifying other their value changed.

* https://github.com/tiluser/Creole-Forth-For-Excel
A scripting language built on top of VBA for Excel based on the Forth language.

## Forth

* [pforth](https://github.com/philburk/pforth):
Portable Forth in C. SimTaDyn Forth mainly inspires of its code.

* [reforth](https://github.com/seanpringle/reforth):
Lot of good ideas for upgrading the Forth syntax.

* [4th](https://thebeez.home.xs4all.nl/4tH/)
The crashproof Forth

* [JonesForth](http://git.annexia.org/?p=jonesforth.git;a=blob;f=jonesforth.S;h=45e6e854a5d2a4c3f26af264dfce56379d401425;hb=HEAD):
Tutorial explaining how a (ligth) Forth is implemented with better explanations than the book Walid P. Salman, Olivier Tisserand, Bruno Toulout.

* [wikiversity](https://fr.wikiversity.org/wiki/Forth/Conserver_des_donn%C3%A9es)
(french only) Clear explanations conserning the CREATE word. Reused examples for my unit tests.

* Book [Forth](https://www.amazon.fr/Forth-Walid-P-Salman/dp/B0007CEVYM):
Walid P. Salman, Olivier Tisserand, Bruno Toulout.
Edition: Eyrolles 1984 (french),
Edition: Macmillan 1983 (english).
Excellent book describing how a Forth is implemented with lot of Forth words coded in Forth.

* [ForthFreak](https://github.com/ForthHub/ForthFreak/wiki/Forthfreak-wiki)

## Graph structure and Graph-Theory IDE

* [rocs](https://github.com/KDE/rocs)
Rocs is the closest graph IDE to SimTaDyn. Written in C++ and QT mixing a drawn interface with a scripting module manipulating the drawn graph. The script is QtScript Java Script.

* [The Stanford slib](http://stanford.edu/~stepp/cppdoc/BasicGraph-class.html)
A complete class for graph written in C++.

* [VivaGraphJS](https://github.com/anvaka/VivaGraphJS)
Powerful graph vizualisation in JS and WebGL.

* [Gephi](https://gephi.org/)
The Open Graph Viz Platform.

* [graphosaurus](https://github.com/frewsxcv/graphosaurus)
A graph viewer powered by WebGL with three.js

## RTree

* [RTree](http://web.archive.org/web/20030625161805/http:/www.es.ucsc.edu/~tonig/)
The inventor's page (C code and PDF).

* RTree explanation in [postgis tutorial](http://www.postgis.fr/chrome/site/docs/workshop-foss4g/doc/introduction.html)

## GIS

* [OpenStreetMap](https://github.com/openstreetmap/iD/blob/master/ARCHITECTURE.md)
OpenStreetMap is built by a community of mappers that contribute and maintain data about roads, trails, cafés, railway stations, and much more, all over the world.

* [PostGIS](http://www.postgis.fr/chrome/site/docs/workshop-foss4g/doc/introduction.html)
spatial database extender for PostgreSQL object-relational database. It adds support for geographic objects allowing location queries to be run in SQL.

* [QGIS](http://www.qgis.org/en/site/)
A Free and Open Source Geographic Information System

## GIS file format

* [ESRI shapefile](https://www.esri.com/library/whitepapers/pdfs/shapefile.pdf)
ESRI shapefiles technical description

* [OpenStreetMap](http://wiki.openstreetmap.org/wiki/Elements)
OpenStreetMap formalism

## Traffic and transportation

* [Ciudadsim](https://www.rocq.inria.fr/metalau/ciudadsim/)
Scilab Traffic Assignment Toolboxes. [PDF](https://www.rocq.inria.fr/metalau/ciudadsim/ftp/CS5/manual/manual.pdf)

* [AequilibraE](http://www.aequilibrae.com)
A transportation modeling toolbox for QGIS

* [Data Representation of Road Networks](http://vterrain.org/Culture/Roads/)

* [TRAFFIC_RECON](http://gamma.cs.unc.edu/TRAFFIC_RECON)
Virtualized Traffic: Reconstructing Traffic Flows from Discrete Spatio-Temporal Data

* [Traffic Simulation, Reconstruction, and Route Planning](https://www.youtube.com/playlist?list=PL20C69E9F07FF4E20&app=desktop)

* [SUMO](http://www.dlr.de/ts/en/desktopdefault.aspx/tabid-9883/16931_read-41000/)
Simulation of Urban MObility

* [volkhin](https://github.com/volkhin/RoadTrafficSimulator)
3D Traffic demo

* [lo-th](https://github.com/lo-th/root/tree/gh-pages/traffic)
Fork of the previous link (better look)

## Drawing cities, maps and streets

* [City Building](https://www.binpress.com/tutorial/creating-a-city-building-game-with-sfml/137) and the [github](https://github.com/dbMansfield/citybuilder)
Creating a City Building Game with SFML

* [glosm](https://github.com/Dimitri1/glosm)
C++ and OpenGL legacy for displaying in 3D Openstreet map files with french pdf docs.

* [Off-road game engine](http://www.milantimko.info/index.php?page=articles&category=tutorials&article=01)

* [Curved Path](http://www.redblobgames.com/articles/curved-paths/)

* [Smooth thick lines using geometry shader](https://forum.libcinder.org/topic/smooth-thick-lines-using-geometry-shader)

* https://github.com/tangrams/tangram
Real-Time WebGL Maps

* https://github.com/mapbox/mapbox-gl-js
Interactive, thoroughly customizable maps in the browser, powered by vector tiles and WebGL

* https://www.mapbox.com/blog/labels-for-navigation/

## OpenGL tutorials and 2D libraries

* [glumpy](https://github.com/glumpy/glumpy)
Python + Numpy + modern OpenGL: fast, scalable and beautiful scientific visualization.
SimTaDyn mainly inspired of its class hiearchy, which is in my opinion the best and simplest OpenGL wrapper.

* [Programming for Games](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/)
Tutorials from the module on Programming for Games of the Newcastle
University. SimTaDyn uses their implementation of Scene graph.

* [SFML](http://www.sfml-dev.org/index-fr.php)
Simple and Fast Multimedia Library.

* [ThinMatrix](https://www.youtube.com/user/ThinMatrix)
Modern OpenGL tutorials on Youtube.

* http://www.tomdalling.com/blog/modern-opengl/01-getting-started-in-xcode-and-visual-cpp/
* http://opengl.developpez.com/tutoriels/open_gl/
* https://learnopengl.com/
* http://www.opengl-tutorial.org/
Modern OpenGL tutorials.

## Forth and OpenGL, GTK, SQL

* PDF [Forth and GTK](http://www.complang.tuwien.ac.at/anton/euroforth/ef10/papers/mahlow.pdf)

* Forth and Legacy OpenGL [tutorials](https://groups.google.com/forum/#!topic/comp.lang.forth/NzgOg9ljXQQ)

* PDF [Forth and SQL](http://www.complang.tuwien.ac.at/anton/euroforth/ef13/papers/nelson.pdf) and [sources](https://groups.google.com/forum/#!topic/comp.lang.forth/EsC4u6cNX-I)

## GTK

* [Ganv](http://drobilla.net/software/ganv)
Gtk widget for interactive graph-like environments.

* [jucipp](https://github.com/cppit/jucipp)
Code source Editor with gtksourceviewmm.

## Libraries

### General lib

* [Moteur3D](http://loulou.developpez.com/tutoriels/moteur3d/)
More than a tutorial on 3D game engine, it explains manager classes in C++ which can used for a more general projects.

* [POCO](https://pocoproject.org/)
Modern, powerful open source C++ class libraries for building network- and internet-based applications that run on desktop, server, mobile and embedded systems.

### Shared library

* [RuntimeCompiledCPlusPlus](https://github.com/RuntimeCompiledCPlusPlus/RuntimeCompiledCPlusPlus)
Make a C code to mimic the behavior of an interpreted language (hot compile and load C code).

### Trace function calls when segfault

* [Backward-cpp](https://github.com/bombela/backward-cpp)
Abeautiful stack trace pretty printer for C++. Used with success for this project.

* [Stacktrace](https://panthema.net/2008/0901-stacktrace-demangled/)
A light tool made in few line.

## Design and coding

### Observer / Event handler

* Mutual Registration: A Pattern for Ensuring Referential Integrity in Bidirectional Object Relationships:
(http://hillside.net/europlop/HillsideEurope/Papers/EuroPLoP1999/1999_Henney_MutualRegistration.pdf)

* GameDev:
(http://www.gamedev.net/page/resources/_/technical/game-programming/effective-event-handling-in-c-r2459)

* Link:
(http://codereview.stackexchange.com/questions/92199/c-observer-design-pattern-implementation)

* Link:
(https://juanchopanzacpp.wordpress.com/2013/02/24/simple-observer-pattern-implementation-c11/)

* Link:
(http://codereview.stackexchange.com/questions/92199/c-observer-design-pattern-implementation)

* Link:
(http://stackoverflow.com/questions/26833920/observer-pattern-with-different-notifications)

### Counting Instances

* DrDobb's:
(http://www.drdobbs.com/cpp/counting-objects-in-c/)

### Memory

* (https://www.ibm.com/developerworks/aix/tutorials/au-memorymanager/)

### cout redirection

* (http://gtk.10911.n7.nabble.com/Redirecting-cout-to-a-TextBuffer-td42437.html)
