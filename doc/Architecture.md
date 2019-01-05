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
it's a (sparse) matrix (2D array) of spreadsheet cells. Each cells
have a name: the concatenation of its column (expressed in base 26)
and its row (given in base 10). For example: `AZ42` means row `AZ` and
column `42`. The matrix dimension is given by the number of rows and
columns of the sheet.

A cell contains a formulae and the result of this formulae:
* A formulae is a mixture of literal values, references to other cell
and operators. For example, a possible formula for the cell `A1` can
be `(5 + AZ42) * 2`.
* The result of a cell is stored since the formulae has been
interpreted with success. For example if the cell `AZ42` has 0 as
result, the intermediate formulae for `A1` will be (5 + 0) * 2 and
therefore its result will be 10.

An interpreter computes the formulae of the spreadsheet and saves the
result. Operators can be more complex that basic mathematics operators,
we can think about loops and controls. The interpreter has to solve
references to other cells. A reference to a cell refers in fact to the
result of the computed formulae. Solving a spreadsheet is based on the
fact: When a cell is solved its result is used for solving formulae on
other cells and by iteration the whole spreadsheet can be computed.

The simpler algorithm for solving a spreadsheet is the follow:
* We separate the spreadsheet into two set: a first set (named
topological list) contains cells that do not reference other cells
(for example: literals like `42` or computation with only literals
like `(5 * 2) + 4`.  A second set (named dependencies list) contains
cells with referencing to cells with or without operations (like
`AZ42` or `AZ42 + 42`).
* For each cell from the topological set, compute its result and find
its relation in the dependencies list. If a formulae can be solved,
place this cell to the topological list.
* Iterate until there no more cells in the topological list.
* The spreadsheet can reach two states: `success` when all cells have
been computed or `failed` when there is a cyclic references between
cells (for example cell `A1` has formulae `B1 + 1` and cell `B1` has
formulae `A1 + 1`). This case is detected when the algorithm has no longer
cells in the topological list but still has cells in the dependencies list.

When the user enters a new value or a new formulae in a cell of a
spreadsheet, this one notifies to others cells referring it that its
content has been changed. This force them to refresh the result of
their formulae. The design pattern observer is used for notifications
like shown on the next figure.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/spreadsheet.png)

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
storing cells. Indeed SimTaDyn does not use a matrix but a special
graph: the sheet pavement contains three types of cells: nodes, arcs
and polygons. The structure of the SimTaDyn spreadsheet is an extended
graph. This complex structure will be describe in next sections in
this document. A class named GraphAlgorithm is used for traversing the
graph. The next figure shows how are interfaced these methods between
the spreadsheet and the graph.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/simsheet.png)

A classic spreadsheet contains at least one pages. It's named
workbook. This is also the case for SimTaDyn: A SimTaDynMap is a class
contains several SimTaDynSheet. The structure of the workbook is
[Scene graph](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/).
This allows to display them with OpenGL and contains relative
transformation (rotation translation, and scaling) between maps. This
will be explained later in this document.

## Extended graph structure as a spreadsheet

A classic graph contains nodes (aka vertices) and arcs (aka edges).
In the graph theory literature many structures are described for
storing nodes and arcs (adjacency list, matrix, sparse matrix) but in
SimTaDyn, we have decided to define the graph structure with relations
"has borders" and "has neighbors" (the expression "has neighbors" is
equivalent to "is a border of"). The following figure summarizes it:

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/graph.png)

In this figure, we can see a class named GraphAlgorithm decomposed it
with virtual methods to allow visiting step by step a graph with
standard algorithms like [depth-first
search](https://en.wikipedia.org/wiki/Depth-first_search) or
breadth-first search, and get visited nodes at each step. This will
help us to visit cells of our spreadsheet like described in other part
of this document.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/simgraph.png)

A SimTaDyn graph is a particular graph: it extends a classic graph by
storing polygons (aka areas, aka zones) delimited by the arcs. Zones
can be see like geographic areas, or Excel cells or polygons of a
2D/3D mesh. We also extend relations "has borders", "is a border of"
and "has neighbors":

* A node is a border of an arc.
* An arc is a border of a zone.
* A zone is a border of 3D zone.
* A 3D zone is a border of 4D zone ... etc, etc.
* A zone has N borders: N arcs. Said differently: A node knows arcs as neighbors.
* A arc has two borders: two nodes. Said differently: An arc knows zones as neighbors.
* A node has no border.

There is indirect neighborship relations:
* A node knows indirectly zones as neighbors because of arcs.
* A zone can knows its vertices through iterating on borders of its
  arcs.

TODO: relations "has borders", "is a border of" and "has neighbors"
should be defined by global index arrays instead to be private
variable members. This can may be make access easier (like drawing).

We use inheritance to make a ForthNode a graph node to be used as a
spreadsheet cell.  We use inheritance to make a ForthArc a graph arc
to be used as a spreadsheet cell. A ForthZone is directly a
spreadsheet cell. We use inheritance of the GraphAlgorithm for
iterating on cells of the spreadsheet when the Forth interpreter is
solving a spreadsheet.

## SimTaDyn Forth Interpreter

Forth is computer language. For SimTaDyn it replaces the Excel VBA
language. If you do not what Forth is, you should read [this
page](https://github.com/Lecrapouille/SimTaDyn/blob/development/src/forth/doc/forth.md)
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

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/simforth.png)

Some methods of the Forth interpreter class are virtual. This is made
for specializing a classic Forth interpreter for the SimTaDyn project,
while keeping intact the behavior of the standard Forth. For example
the interpreter can directly understand spreadsheet cell names. As
second example, add more primitive words calling class methods used by
SimTaDyn (for example OpenGL).

## Resources

SimTaDyn has to manage many resources like project files, maps,
sheets, forth scripts, textures, ... Resources come from files on the
hard-disk (for the moment) so SimTaDyn has to load and save them. It
also has to import and export files from proprietary software
(shapefile, ...). Resources are shareable by several clients (Forth
editor, Map editor, GUI widgets): they do not have to create or
dispose of them directly. This is the job for other classes: the
resource manager and the loader manager.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/development/doc/uml/loader.png)

A resource manager takes care of the life of resources and release it
automatically when nobody no longer uses it. Resources are identified
by their unique ID (integer, enum, string ...).

A loader manager manages a collection of loaders. A loader creates a
new resource from a file (loading) ot saves it in file. We can extend
this idea to other support (memory, Ethernet ...). A loader manager is
mainly a map between a loader and file extensions. For example an
OpenGL texture can be created from jpeg or png or bmp files. In this
case we one resource (the OpenGL texture), 4 file extension referring
the same texture loader.

Sequences:
* The resource manager creates and store a new dummy resource.
* The resource is passed to the loader manager to be loaded.
* The user acquires to the resource from the resource manager for using it.
* The user ask the resource manager to dispose of the resource.
* The resource is destroyed when nobody is using of it.

## Renderer

### OpenGL Wrapper

Wraps OpenGL 3.3 API into a C++ class. More information are given in
[this page](https://github.com/Lecrapouille/SimTaDyn/blob/dev-new-opengl/doc/OpenGL.md)

### OpenGL Renderer

A SimTaDyn map uses the structure of
[SceneGraph](https://research.ncl.ac.uk/game/mastersdegree/graphicsforgames/scenegraphs/Tutorial%206%20-%20Scene%20Graphs.pdf)
where its nodes are SimTaDynSheets. SimTaDynSheets are displayed by the OpenGL wrapper.

As a fast summary of the previous link, a scene graph is a tree
structure (not a graph for this project) storing the relative
transformation between a node and its parent. In our case nodes store
a transformation matrix. This allows to easily place and move a set of
3D objects in relation. Let suppose we want to draw a car and its four
wheels.  Wheels turn with 2 degrees of liberty but when we are
translating the car we also want to apply this translation to
wheels. Scene graph will compute all matrices for us and apply it to
objects.

The reason of why using a scene graph for storing sheets of a
spreadsheet comes from the idea that it could be interesting to place
a set of geographic maps together (for example two maps with different
position and scaling) can be superposed (georeferenced) without doing
maths.
