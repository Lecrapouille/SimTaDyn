#ifndef FORTH_HPP_
#  define FORTH_HPP_

#  include "simtypes.hpp"
#  include <iostream>
#  include <stdexcept>
#  include <string>

typedef uint32_t Cell;
#define STACK_SIZE     (1024U)
#define DATASPACE_SIZE (16U * 1024U * sizeof (Cell))

class Forth
{
public:
  Forth();
  bool toInt(const std::string& word, Cell& number);
  bool toToken(const std::string& word, Cell& token);
  inline bool isPrimitive(const Cell token);
  void eval(const Cell tx);
  bool interprete(const std::string& word);
  bool readString(const std::string& txt);

protected:
  Cell dictionnary[DATASPACE_SIZE];
  Cell data_stack[STACK_SIZE];
  Cell *return_stack[STACK_SIZE];

  // Registers
  Cell  tos;   // Top of (data) Stack
  Cell *dsp;   // Data stack pointer
  Cell **rsp;   // Return stack pointer
  Cell *here;  // Address of the next available dictionary location.
  Cell *ip;    // Instruction pointer (CFA of the next word to be executed)

  // Dictionnary
  int32_t *last;
};

#endif /* FORTH_HPP_ */
