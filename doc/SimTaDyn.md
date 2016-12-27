# SimTaDyn
SIMulateur TAbleur DYNamique

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/SimTaDyn.jpg)

## English

SimTaDyn is a geographic information system (GIS) allowing to draw
geographic maps, to edit them like we do with a spreadsheet page.

A spreadsheet, like Excel, edits pages of regular and rectangular
pavement. Cells contain either data or computation function which
performing actions on itself or on the content of other cells.

A GIS is an editor manipulating geographic databases and draw
them. Datum are usually static (in the way modifying one set over time
will dynamically change other datum).

The idea of SimTaDyn is to couple GIS with a spreadsheet in a single tool:

* the map editor draws and manipulates, in 2D, different kinds of cells:
  nodes (aka vertices), arcs (aka edges) and zones (aka areas), add
  datum to them and scripts in language Forth.

* the Forth editor allows to write and run scripts. Scripts couple
  datum together dynamically. A script can run over all cells in the
  map or be stored and run inside a cell. Forth has been choose
  because it's light to implement and is very powerful: stack
  language, manage it's own memory, no syntax by using reverse polish
  notation and the syntax can evolves by itself.

* Finally we can display result of script in 3D map.

Application with this SimTaDyn is not limited to GIS, we can image the
following applications:

* A pure spreadsheet (nodes and arcs have no effects, zones are
  rectangular).

* A flowchart in the same idea of Lego Mindsorms (nodes are Forth code
  and edges the order sequences). Or blocks schematics for flow
  control like Simulink or Scicos.

* Solving partial differential equation.

* Resolving problem of traffic jam/congestion in a city (see Wardrop's
  equilibrium).

* Redo the SimCity game.

SimTaDyn is a school project made in 2003-2004 where the name is a
joke mixing words SIM (for SimCity, SIMulation), TA (for tableur or
spreadsheet in french), DYN (for dynamic) sounding close to the word
citadine (female noun for a city-dweller in french).

The initial code is available on the git branch
Original-Version-EPITA-2004. The project was not finalized (from
specification document) but functional. The master branch is empty
because in 2016 I decided to develop it back from scratch with cleaner
code (I hope).

To compile the old project you will need to install lib like mysql,
opengl (the old version), gtk, gtkglext, ncurses. That's easy on
Ubuntu. The project could run on Mac OSX.

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
