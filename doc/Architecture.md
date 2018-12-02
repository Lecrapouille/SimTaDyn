# Architecture

This documents is a general architectural description of principal
parts of the SimTaDyn code source. This document is supposed to be
updated at the same time the code source evolves but algorithms in
code source can be a little different from this document (updates,
method names, etc). For more architecture details, please, refer to
following documents:

* README.md located on each sub-directories. These files give more
details on implemented algorithms.
* Commentaries in the header files where you can generate the html
files with Doxygen (command to type: `make doc`).

Note concerning class diagram symbols for the following figures on this document:
* C for C++ Class.
* I for C++ Interface class (containing only pure virtual methods).
* A for C++ Abstract class (containing a mixture of pure virtual methods and concrete methods).
* I personally make not difference between an Interface class and Abstract class. In this document
they are synonyms.
* white array: inheritance.
* filled array: composition of class.
* dot array: interaction between classes.

## Classic spreadsheet and SimTaDyn spreadsheet

A classic spreadsheet is a rectangular pavement of cells. Internally
it's a matrix (2D array) of cells where the dimension is given by the
number of rows and columns.

A cell contains a formulae (given as a string) and the result of the
formulae once interpreted. A formulae is a mixture of literal values,
references to other cell and operators. An interpreter computes the
formulae and saves the result. The interpreter has to solve references
to other cells. A reference to a cell refers to the result of the
computed formulae. The cell is referred by the concatenation of its
column (expressed in base 26) and its row (given in base 10). For
example: AZ42 means row AZ and column 42.

When a cell is solved its result is used for solving formulae on other
cells. By iteration the whole spreadsheet is computed. When the user
enters a value or a formulae in a cell, this one notifies to others
cells that its content has changed to force them to refresh the result
of their formulae. The design pattern observer is used for
notifications like shown on the next figure.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/spreadsheet.png)

In the case of SimTaDyn, we use a Forth interpreter, named SimForth,
as replacement of the classic Excel VBA language. The formulae shall
leave the result in the data stack of the Forth interpreter which will
"pop" it and store its content into the cell.

In the future, SimTaDyn cells will store several results instead of a
single like done in Excel. Indeed values will be linked to a database
through a key index.

The SpreadSheet class is an abstract class containing virtual methods
like resetCellIterator(), hasCell(), nextCell() used for iterating on
cells while keeping independent on the implementation of the container
storing cells. For example SimTaDyn does not use a matrix but a
special graph. A SimTaDyn spreadsheet is an implementation of the
SpreadSheet class: the sheet pavement contains three types of cells:
nodes, arcs and polygons. The structure of the SimTaDyn spreadsheet is
an extended graph. This complex structure will be describe in the
coming section. A class named GraphAlgorithm is used for traversing
the graph. The next figure shows how are interfaced these methods
between the spreadsheet and the graph.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/simsheet.png)

A classic spreadsheet contain at least one pages (workbook). This is
also the case for SimTaDyn: A SimMap is a class containing several
SimSheet. For drawing all sheets in OpenGL, we use an other graph:
a [Scene graph](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/)
for storing all sheets. A scene graph stores on its node a 4x4 matrix and a 3D object.
The matrix represents the relative positions between them (rotation translation,
and scaling). This allows easier 3D manipulations of objects individually.

## Extended graph structure as a spreadsheet

A classic graph contains nodes (aka vertices) and arcs (aka edges).
A SimTaDyn graph is a particular graph: it also stores polygons (aka
areas, aka zones) delimited by the arcs. Zones can be see like
geographic areas, or Excel cells or polygons of a 2D/3D mesh.

In the graph theory literature many graph structures are described
for storing nodes and arcs (adjacency list, matrix, sparse
matrix) like shown in the following figure.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/graph.png)

In SimTaDyn, we decided to define the graph structure by the relation
"has borders" and "neighbors" (aka "is a border of") of its elements:

* A node is a border of an arc.
* An arc is a border of a zone.
* A zone is a border of 3D zone.
* A 3D zone is a border of ... etc, etc.
* A zone has N borders (N arcs).
* A arc has two borders (two nodes).
* A node has no border.

The inverse relation is also defined:
* A node knows arcs as neighbors.
* An arc knows zones as neighbors.
* A zone contains several (at least 3) arcs.
* An arc contains two nodes.

The follow TODO A refaire
![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/simgraph.png)

A node knows indirectly which zones is near him through the arcs
neighbors list. A zone knows indirectly its nodes.

From aclassic graph and its nodes, arcs. We use template class for easier usage.
We use inheritance to make a ForthNode a graph node to be used as a spreadsheet cell.
We use inheritance to make a ForthArc a graph arc to be used as a spreadsheet cell.
A ForthZone is directly a spreadsheet cell.

With a graph we can implement some basic algorithms like the [depth-first search](https://en.wikipedia.org/wiki/Depth-first_search),
Breadth-first search, ... We made an abstract class GraphAlgorithm for iterating on cells of the
spreadsheet when the Forth interpreter is interpreting the sheet.

## SimTaDyn Forth Interpreter

Forth is computer language. For SimTaDyn it replaces the Excel VBA
language. If you do not what Forth is, you should read [this
page](https://github.com/Lecrapouille/SimTaDyn/blob/master/src/forth/doc/forth.md)
which explains what it is, how it's working and how it's interpreter
is implemented.

As summary, Forth is composed of an interpreter, a dictionary (aka
virtual machine) for storing the byte code (compiled script), and
several stacks (at least two): the data stack and the return
stack. The fist stack is used for storing operands (which are consumed
by Forth operators aka Forth words) and the second, used internally by
the forth interpreter, for storing the call of operators.

The SimTaDyn Forth is a special Forth which understand the spreadsheet
cell names and which contains a stack for storing cells references.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/simforth.png)

Some methods of the Forth interpreter class are virtual. This is made
for specializing a classic Forth interpreter for the SimTaDyn project,
while keeping intact the behavior of the standard Forth. For example
the interpreter can directly understand spreadsheet cell names. As
second example, add more primitive words calling class methods used by
SimTaDyn (for example OpenGL).

## Resources

SimTaDyn has to manage many resources like project files, maps, forth
scripts, textures, ... Resources come from files so SimTaDyn has to
load and save them. It also has to import and export files from
proprietary software (shapefile, ...). Resources are shareable by
several clients: they dispose of them and release them.

To simplify the code. Here is the class diagram managing, resources
and loaders.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/loader.png)

A resource manager take care of the life of resources and release the
ones that nobody uses. Resources are identified by their unique ID. A
loader manager manages a collection of loaders indexed by the file
extension. A loader creates a new resource from a file (loading) and
saves a resource in file. For example the texture loader creates
OpenGL objects from jpeg, png, bmp files.

## Renderer

### OpenGL Wrapper

Wraps OpenGL 3.3 API into a C++ class. More informations are given in [this page](https://github.com/Lecrapouille/SimTaDyn/blob/dev-new-opengl/doc/OpenGL.md)

### Renderer

Use an OpenGL Wrapper.
SimTaDyn maps are stored withing a [SceneGraph](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf)
and displayed with the OpenGL wrapper.
