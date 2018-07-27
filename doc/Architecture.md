# SimTaDyn Architecture for developers

## Doxygen

Use Doxywizard and the Doxyfile file for generating the code documentation.

## Architecture description

Here is the simplified SimTaDyn Architecture

![alt tag](https://github.com/Lecrapouille/SimTaDyn/blob/master/doc/Architecture.jpg)


SimTaDynWindow is a Gtk window containing mainly a Forth editor and a geographic map editor.

A ForthEditor edits and manages Forth documents through a Gtk TextView and Gtk TextBuffer.

A SimForth is a Forth modified for the need of the software. Classes for a Forth interpreter
have been splitted : the dictionary, an ascii stream reader and the Forth itself.

A Map editor edits and displays in 2D or 3D geographic maps thank to the renderer. Maps are
imported using the SimTaDynMapLoader.

A Renderer is a class using an OpenGL context for displaying 2D and 3D scenes.

A SimTaDynMap is a set of SimTaDynGraph, RTree (for spatial index) and Forth documents.

A SimTaDyGraph is a classical graph extended to contain zones and where elements of it
contain Forth scripts and geographic datum. 





