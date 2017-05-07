# Introduction to SimTaDyn

SimTaDyn: a dynamical spreadsheet for geographic simulation
(SIMulateur TAbleur DYNamique in french). This page has is translated
in the following languages:

* [English version](#English)
* [French version](#French)

See also the [french PDF manual](https://github.com/Lecrapouille/SimTaDyn/blob/Original-Version-EPITA-2004/doc/Simtadyn-Manuel.pdf)
made in 2007 (badly written because this project was at the origin a
student engineering school project).

_The following figure is a screenshot of the legacy version of SimTaDyn._
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/SimTaDyn.jpg)

## English

### State of Art
SimTaDyn is a proof of concept for a geographic information system
(GIS) allowing to draw geographic maps, to edit their information like
if they were spreadsheet pages, and run computations on them.

A spreadsheet, like Excel, edits pages of regular and rectangular
pavement. Cells contain either data or a computation function which
perform actions on the cell itself or on the content of other cells.

A Geographic Information System (GIS) is an editor manipulating
geographic databases and draw them. Datum are usually static (in the
way modifying one set over time will dynamically change other datum).

The idea of SimTaDyn is to couple in a single tool a GIS with a
spreadsheet :

1. The idea of the classic spreadsheet is extended. In SimTaDyn the
pavement is more general and contains different kind of editable
cells :

* Polygonal cells (like a geographic area) extend the regular and
  rectangular pavement of classic spreadsheets.
* Edge cells (like roads) extend the pavement of classic spreadsheets
  by allowing edges to be interactive as well polygonal cells.
* Node cells (like road crossings) extend the pavement of classic
  spreadsheets by allowing corners to be interactive.

This idea is summarize by the following picture:
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/pavement.png)

2. Like classic spreadsheet, SimTaDyn has a script language for
"gluing" cells together by formulas. When once has its value changed,
other cells are notified through their relation given by the
formula. Instead of using a Visual Basic, SimTaDyn uses the [Forth
language](https://en.wikipedia.org/wiki/Forth_(programming_language))
which is a stack language (reverse polish notation), embedding its
virtual machine (managing it's own memory, dictionary, byte code) and
its own interpreter (compiler). Forth is very powerful because it's a
self-evoluting language: the interpreter evolves dynamically and can
modify its own syntax.

3. The Forth editor allows to write and run two kind of scripts:
* Global scripts are applied to the map, modifying it and it cells.
* Local scripts are stored inside the cell like the classic spreadsheet.

4. The idea of GIS is also extended. Classic GIS manipulates two kind of geographic maps.
* [Rasterized map](http://desktop.arcgis.com/en/arcmap/10.3/manage-data/raster-and-images/what-is-raster-data.htm)
  used for computations with a [map algebra](https://www.nrem.iastate.edu/files/w11-MapAlgebra_presentation5.pdf),
* [Vectorized map](http://gisgeography.com/spatial-data-types-vector-raster/) used
  for displaying static informations from databases.  SimTaDyn can be
  seen as a vectorized map manipulated like a rasterized map. Each
  cell is a either a polygon, a polyline or dots but are at the same
  time a script forth and an element of the database.

* Because SimTaDyn uses OpenGL and a evolutive scripting language, we can display results of a script in 3D instead of 2D.

### Typical SimTaDyn applications
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

### Origin of the project
SimTaDyn was at this origin a school project made in 2003-2004 for learning coding. Its name comes from a
joke mixing words SIM (for SimCity, SIMulation), TA (for TABleur:
spreadsheet in French), DYN (for DYNamic) and sounding close to the project Sytadin
a traffic jam application where the name is also a joke to the French word citadine (meaning city-dweller).

The initial code is available on the git branch
Original-Version-EPITA-2004. The project was not finalized (from
specification document) but functional. To compile it you will need to install lib like MySQL,
OpenGL (the old version), GTK, gtkglext, ncurses and worked for OSX with X11 and still working on Linux.

The project reborn in 2016, after a talk with a co-worker and I decided to develop "from scratch" in C++ with cleaner
code (I hope) because the original code was coded very messy. You can get it on the master branch.
 Note: this project is
still in development and still is a proof of concept.

## French

SimTaDyn est un système d'information géographique (SIG) permettant de
dessiner des cartes géographiques et des les éditer comme on éditerait
une feuille de calcul de tableur.

Un tableur (comme Excel) est un éditeur de feuilles de calcul, à
savoir des tables de cellules formant un pavage rectangulaire. Chaque
cellule contient des données de type divers y compris des fonctions
susceptibles de faire du calcul sur son contenu et celui des autres
cellules (Visual Basic par exemple).

Un SIG est un autre type d'éditeur manipulant des bases de données
géographiques et les affiche. Les données sont en générale statiques
(dans le sens où modifier certaines va changer dynamiquement
d'autres).

L'idée de SimTaDyn est de lier tableur et SIG en un unique
outil:

* L'éditeur de carte permet de créer une carte en 2D en dessinant
  différentes types cellules: noeuds, arcs et zones et de leur ajouter
  des informations géographiques et un script en langage Forth.

* L'éditeur Forth permet d'écrire des scripts et de les exécuter. Il
  sert donc de liant à une manipulation dynamique des informations
  géographiques. Un script Forth peut s'exécuter sur l'ensemble des
  cellules de la carte ou bien être stocké dans des cellules. Forth a
  été choisi car c'est un langage très puissant (langage à pile et
  système de notation en polonaise inversée et la syntaxe est
  auto-évolutive).

* Enfin la carte peut être visualisée en 3D (par exemple comme étant
  le résultat d'un script).

Les applications de SimTaDyn ne se limitent pas uniquement aux cartes
géographiques. On peut penser à:

* refaire un tableur Excel (où les noeuds et arcs n'ont pas d'effet et
  où les zones sont un pavage).

* Un flowchar façon Lego Mindstorm: les noeuds sont du code Forth et
  les arcs l'ordre d'exécution.

* Résoudre numériquement des équation aux dérivées partielles
  (maillage).

* Résoudre des problèmes de congestions de trafic (équilibre de
  Wardrop).

* Refaire le jeu SimCity.

SimTaDyn est initialement un projet d'étudiant à EPITA en 2003. Le nom
SimTaDyn vient d'un jeu de mot entre SimCity, Tableur, Dynamique et la
consonance du nom citadine.

Le code issu de la dernière soutenance est disponible dans la branche
Original-Version-EPITA-2004. A cette époque le projet était non
finalisé (par rapport au cahier des charges) mais fonctionnel.

En 2016 j'ai décidé de continuer ce projet et de le reprendre depuis
le début et de le recoder en C++, c'est pour cette raison que la
branche master est actuellement vide.

Pour compiler le vieux projet vous aurez besoin d'un gcc et des
bibliothèques gtk, glib, une vieille version d'opengl, gtkglext,
mysql, ncurses que l'on peut facilement installer avec Ubuntu (et à
l'époque sur un Mac OSX).


