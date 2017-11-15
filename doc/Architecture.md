# Architecture

Here is the simplified SimTaDyn code architecture. For more details, please read:
* README.md on each sub-directories for detailed algorithms and strategies.
* header files or code documentation (generated with doxygen) for implementation details.

## SimTaDyn Forth

A Forth context is composed of the Forth interpreter, its dictionary
(storing the byte code) and several stacks (at least the data stack
and memory stack for class forth interpreter). You can read
[this page](https://github.com/Lecrapouille/SimTaDyn/blob/master/src/forth/doc/forth.md)
for more details about the standard Forth made for SimTaDyn.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/simforth.png)

Some methods of the Forth interpreter class are virtual. This is made
to specialized it for the SimTaDyn project, while keeping intact the
behavior of the standard Forth. For example the interpreter can
directly understand spreadsheet cell names. As second example, add more
primitive words calling class methods used by SimTaDyn (for example OpenGL).

## Classic spreadsheet

A classic spreadsheet is a matrix of cells. A cell either contains a
formulae or a value. A formulae is a mix of literal values,
references to other cell and operators. An interpreter, for executing
the formulae. The result is stored in the cell. When the user enters a
value or a formulae in a cell, this one notifies to others cells that
its content has changed to force them to refresh their result. The
design pattern observer is used for notifications.

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/spreadsheet.png)

In the case of SimTaDyn, we use the Forth interpreter. The formulae
shall leave the result in the data stack of the Forth interpreter
which will poped it and store it in the Cell value. SimTaDyn Cell
values are linked to a database by a key index.

A SimTaDyn spreadsheet is a pavement of three types of cells: nodes,
arcs and polygons. This complex structure will be describe in coming
sections.

## Graph as a spreadsheet

A classic graph contains nodes (aka vertices) and arcs (aka
edges). They are many graph structure for storing these data
(adjacency list, matrix, sparse matrix). Graph theory and their
different data structures is heavily described in algorithmic
books.

From a graph we can implement some basic algorithms like
the [depth first search](https://en.wikipedia.org/wiki/Depth-first_search).

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/graph.png)

A SimTaDyn map is a particular graph: it also stores polygons (aka
areas, aka zones).

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/uml/simgraph.png)

We decided to define the graph structure by the relation "has borders"
and "is a border of" of its elements:

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

A node knows indirectly which zones which contains it through its list
of neighboring arcs. A zone knows indirectly its nodes.

From a basic (classic) graph and its nodes, arcs. We use template class for easier usage.
We use inheritance to make a ForthNode a graph node to be used as a spreadsheet cell.
We use inheritance to make a ForthArc a graph arc to be used as a spreadsheet cell.
A ForthZone is directly a spreadsheet cell.

A spreadsheet contain at least one pages (workbook). This is also the case for SimTaDyn.

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
