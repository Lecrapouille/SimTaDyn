### SimForth Standalone ###

SimForth, used for the SimTaDyn project, can be compiled apart as a basic Forth.
SimForth is developped in C++ in SimTaDyn in the aim to have either a basic Forth
or extended for the GIS thanks to the power of the inheritance. This project does
propose the GIS extension but simply the classic Forth.

## Compilation ##

```sh
cd SimTaDyn/src/forth/standalone/
make
./build/SimForth -h
```

## Usage ##

```sh
Usage:   SimForth [-option] [argument]
option:  -h              Show help information
option:  -u              Show help information
         -l dico         Load a SimForth dictionary file and smash the current dictionary
         -a dico         load a SimForth dictionary file and append to the current dictionary
         -d dico         Dump the current dictionary into a binary file
         -f file         Interprete a SimForth script file (ascii)
         -e string       Interprete a SimForth script string (ascii)
         -p              Pretty print the dictionary with or without color (depending on option -x)
         -i              Interactive mode (not yet implemented)
         -x              Do not use color when displaying dictionary
```

More explanations on arguments:
* SimForth can interpete a script Forth passed either as file (-f) or as command line (-e).
* SimForth does not yet have an interactive mode. In the future the SimTaDyn Forth editor GUI will be separated and therefore
will be directly included inside the code source of the standalone SimForth.
* SimForth dictionnary can be displayed on the console with colors (-p) This is inspired by Charles Moore's ColorForth.
* A dictionary file is a binary file containing the "dump" of the Forth dictionary (-d). Dictionaries can be loaded (-l) or concatenated (-a).

## Example ##

```sh
./build/SimForth -f ../core/system.fs -f../core/tester.fs -e "T{ 1 2 3 -> 1 2 3 }T" -p
```

## For the moment ##

SimForth has to be improved a basic core system is not yet loaded automaticaly. 
For the moment add this line as argument to SimForth to have a more complex system:
```sh
./build/SimForth -f ../core/system.fs
```
