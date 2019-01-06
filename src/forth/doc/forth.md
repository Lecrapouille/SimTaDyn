# SimForth

SimForth is the [Forth langage](https://en.wikipedia.org/wiki/Forth_(programming_language))
used in SimTaDyn for replacing the Visual Basic in traditional spreadsheet (I got fun when
scripting in VB for spreadsheet). SimForth can be used as standalone forth (see
[here](https://github.com/Lecrapouille/SimTaDyn/blob/master/src/forth/doc/standalone.md)
for more details how to compile, install and use it).

SimForth is based on standard Forth 1978 ... 2012 like
[Gforth](https://www.gnu.org/software/gforth/),
[pForth](http://www.softsynth.com/pforth/),
[4th](https://thebeez.home.xs4all.nl/4tH/4tHmanual.pdf),
[reforth](https://github.com/seanpringle/reforth)
but with some incompatibilites or missing words with the standard [Forth 2012](http://lars.nocrew.org/forth2012/core.html)
but with words specialized for the SimTaDyn project. SimForth has not been written in assembly langage (because it was not
created to be embedded for a real time system) but in C++ (for a more readable code than C). C++ helped to have both a
classic Forth and a specialized Forth for the SimTaDyn project.

I feeled concern by the 4th philosophy: having a Forth that never
crashes. This is currently not yet the case for SimForth but I would
to reach the same goal in the future.

SimForth, like gforth can import, C functions in the aim to call
existing code and helping adding plugins for SimTaDyn.

## Where to start learning Forth

There are several good websites and books introducing standard Forth:

* [Starting Forth](https://www.forth.com/starting-forth/) well written for discovering this language.
* [Forth](https://www.amazon.com/FORTH-W-P-SALMAN/dp/0387912568/), W.P. Salman, O. Tisserand, B. Toulout Edition Macmillan (english version) or Eyrolles (french version) 1983. An excellent book for understanding how Forth works internaly. The 1983 french version has more typo than the english version (the 2nd revision is 1984 but I do not have it). ISBN-13: 978-0387912561, ISBN-10: 0387912568
* [JonesForth](http://git.annexia.org/?p=jonesforth.git;a=blob;f=jonesforth.S;h=45e6e854a5d2a4c3f26af264dfce56379d401425;hb=HEAD) excellent tutorial on how to implement a Forth (in assembly) and how Forth is implemented.
* [Thinking Forth](http://thinking-forth.sourceforge.net/) how to well write Forth code.

The second link is my best book followed by the JonesForth tutorial: they complete themselves.

## Difference between SimForth standard forth

Standard Forths give words than can self modifying definition. These
words are great for embedded systems where RAM manipulation is
important but for GIS application this is less important and even
dangerous when badly used. For this reason some words in SimForth do
not follow the standard or are inexistant.

### No user variables

Standard Forths user variables are not present in SimForth but either used as "pure function" (as refrence to functional programming).

* `HERE` gives the address of the 1st free slot in the dictionary and
  in standard Forth, a variable `DP` is also given for changing
  `HERE`. In SimForth `DP` does not exist and the only way to deplace
  `HERE` is to use words like `,` and `ALLOT` (note `ALLOT` take into
  account negative values, will this method is not recommended). This
  avoid erroneous code like this `0 DP !` erasing the whole
  dictionary.

* Words `@`, '`!`, `,`, `ALLOT` and equivalent have safe guards to be
  sure to not leave the dictionary or smash previous definition.

### Tokens

SimForth data numbers (that can be placed in data stack) are 32-bits
length but tokens (aka addresses) are 16-bits length in the aim to
minimize the dictionary size. However a SimForth dictionary cannot be
greater than 64 kilo bytes (= 2^16) but this can be improved in
future.

Addresses are relative specially LFA (Link Field Address). The
advantage with relative addresses compared to absolute addresses is
that you can "concat" several Forth dictionaries together or translate
them into memory without effort.

SimForth dictionary is architecture independant for two reasons:
* Numbers are stored internaly in big endian and then transformed in
  the current endian of the architecture when reading.
* Dictionary datum do not need to be aligned (the word `ALIGN` is not
  needed). This simplifies the sharing of dictionaries between
  architecture.

Therefore users can share the byte code of compiled SimForth scripts
(as a single binary file) and append it in their dictionary instead of
sharing (several) ASCII code source and compile them. This makes
sharing executable extremly easy in comparaison of sharing Unix
binaries for example.

### Word size name

SimForth uses the old standard: 31 caracters max for a name. Gforth,
pForth removed this limitation. All SimForth words stored in the
dictionary are upper cased while the user can type mix of lower and
upper cases, SimForth will transform words internaly.

### No memory blocks

Forth is born as language for constrain embedded systems where the
memory was costly. A block is a chunck of 1024 bytes data for storing
and loading the source code. This is still used while in modern
computers with huge quantities memories directly managed by the
operating system. Therefore we prefer using file streams
instead. Words like `LIST`, `LOAD` are not defined in the core.

### No vocabular

Known words are stored through a linked list (from the last
inserted). Vocabular allows in standard Forth to minize research in
existing words using the principe of trees (but instead of searching
from the root) Forth search from a leaf.

SimForth will use a trie tee for fasten the research.

## SimForth guts

### Benchmark

SimForth has not been made to be the faster Forth on the market but to
try to be portable and safer as possible in execution. Forths like
Gforth, JonesForth are written in assembly to manipulate directly CPU
registers like interpreter pointer (IP) or stack pointer
(SP). SimForth get inspiration from pForth, reforth and is written in
C++ to be portable, Therefore stacks are "simulated" by an array and
registers by variable, implying more overhead than direct assembly.

### Unit tests

TODO
