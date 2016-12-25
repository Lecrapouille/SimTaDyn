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
         -l dico         Load a SimForth dictionary file and replace the old dictionary
         -a dico         Load and append a SimForth dictionary file
         -d dico         Dump the dictionary as a binary file
         -f file         Run a SimForth text file
         -e string       Execute a string as SimForth script
         -p              Pretty print the dictionary
         -i              Interactive mode
```

For the moment, to have a more complete Forth dictionary add this line as argument to SimForth:
```sh
./build/SimForth -f ../core/system.fs
```

### Example ###
```sh
./build/SimForth -f ../core/system.fs -f../core/tester.fs -e "T{ 1 2 3 -> 1 2 3 }T"
```
