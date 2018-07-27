### SimForth Standalone ###

SimForth used in the SimTaDyn GUI can be compiled apart as a minimal Forth.

## Install ##

```sh
cd SimTaDyn/tree/master/src/SimForth/Forth/standalone
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

More explanations:

A dictionary file is a binary


## Example ##
```sh
./build/SimForth -f ../core/system.fs -f../core/tester.fs -e "T{ 1 2 3 -> 1 2 3 }T"
```

## For the moment ##

For the moment, to have a more complete Forth dictionary add this line as argument to SimForth:
```sh
./build/SimForth -f ../core/system.fs
```
