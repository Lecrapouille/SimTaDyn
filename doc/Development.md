# SimTaDyn developementment guidelines

## File extensions

* c++ code: file extensions are .cpp and .hpp for code and headers. I use .tpp for template classes and .ipp for including code source (template implementation for example).
* c code: file extensions are .c and .h for code and headers.
* forth code: file extensions are .fs or .fth for standard code. .sfs or .sfth for SimTaDyn forth (which is a non-standard forth because using map cells).
* glade code: .glade a xml for building at runtime gtk+ or gtkmm application.
* OpenGL code: .fragment for fragment shaders, .vertex for vertex shader.
* PlantUML code: .plantuml for UML code source (then generated int png files).
* SimTaDyn files: .smap for SimTaDyn map. .sfth or .sfs for SimTaDyn forth scripts. .spak for SimTaDyn package file (a zip file containing maps, forth scripts and resources).

## Coding rules

* Indentation: I always used gnu c style with Yoda style. If people prefer 4 spaces instead of 2 or aligning { with if rather addition space: it's ok for me.
* I hate Google and Kernel style with { at the end of the line. It's totally unreadable particularly with if. So always carriage return them.
* Even for a single line after a if or a while, use brackets. Please do not use kernel style is unreadable again !
* Separate langage tokens by a single space. I do not use tabulating. They are converted into spaces. Erase trailing white spaces.
* Avoid mixing a += (++i)++;
* Split huge code into plenty of small methods doing a single task. It's better to unit test them.
* Never use literals. They always look magic numbers for other readers. Give them a name (#define or const or constexpr) !
* Add plenty of comments and because a code never self-document itself. Describe files and complex algorithms in README.md files.
* Add unit tests with code. They are not useless. They helped a lot.
* I use Emacs, feel free to use another development IDE and import their. See my [Emacs config](https://github.com/Lecrapouille/DotEmacs).

## Code documentation

Launch Doxywizard or Doxygen against the Doxyfile file placed at the
root of the project for generating the code documentation.
```sh
doxygen Doxyfile
```

Concerning the documentation, I try to impose to myself the following convention:
* For hpp file: I write the brief description of methods, functions. I do not describe parameters.
* For cpp file: I detail the algorithm and I described parameters of the method.
* For tpp file (template file): Describe everything.

And because computer language will never be as powerful as the human
language, markdown files (.md) have their utility in each
sub-directories for detailing the utility of files or to describe
subjects like architecture or algorithms.

Note: please do not believe people saying that a code itself is
self-documented. A 10-lines of human script will always describe
better something than 1000-lines of C++.
