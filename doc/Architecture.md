# SimTaDyn for developers

## File extensions

* c++: file extensions are .cpp and .hpp for code and headers. I use .tpp for templated classes and .ipp for including code source (template implementation for example).
* forth: file extensions are .fs or .fth for standard code. .sfs or .sfth for SimTaDyn forth (which is a non-standard forth because using map cells).
* glade: .glade a xml for building at runtime gtk+ or gtkmm application.
* OpenGL: .fragment for fragment shaders, .vertex for vertex shader.
* SimTaDyn: .smap for SimTaDyn map. .sfth or .sfs for SimTaDyn forth scripts. .spak for SimTaDyn package file (a zip file containing maps, forth scripts and resources).

## Coding rules

* Indentation: I always used gnu c style with Yoda style. If people prefer 4 spaces instead of 2 or aligning { with if rather addition space: it's ok for me.
* I hate Google and Kernel style with { at the end of the line. It's totaly unreadable particularly with if ! So always cariage return them.
* Even for a single line after a if or a while, use brackets. Please do not use kernel style is unreadable again !
* Separate langage tokens by a single space. I do not use tabulars. They are converted into spaces. Erase trailing whitespaces.
* Avoid mixing a += (++i)++;
* Split huge code into plenty of small methods doing a single task. It's better to unit test them.
* Never use literals. They always look magic numbers for other readers. Give them a name (#define or const or constexpr) !
* Add plenty of comments. A code never self-document itself !
* Add unit tests with code. They are not useless. They helped a lot.
* I use Emacs, feel free to use another development IDE. See my [Emacs config](https://github.com/Lecrapouille/DotEmacs).

## Code documentation

Launch Doxywizard or Doxygen against the Doxyfile file placed at the root of the project for generating the code documentation.
```sh
doxygen Doxyfile
```

Concerning the documentation, I try to impose to myself the following convention:
* For hpp file: I write the brief description of methods, functions. I do not describe parameters.
* For cpp file: I write the detail description of the algorithm and I described parameters.
* For tpp file (templated file): Describe all stuffs from cpp and hpp.

And because compture language will never be as powerful as the human langage, markdown files (.md) have their utility in each subdirectories for detailing the utility of files or to describe subjects like architecture or algorithms.

Note: please do not believe people saying that a code itself is self-documented. A 10-lines of human script will always describe better something than 1000-lines of C++.

## Architecture

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





