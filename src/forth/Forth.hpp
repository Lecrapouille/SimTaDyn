#ifndef FORTH_HPP_
#  define FORTH_HPP_

#  include "simtypes.hpp"
#  include <iostream>
#  include <stdexcept>
#  include <string>
#  include <iomanip>

typedef uint32_t Cell;
#define STACK_SIZE     (1024U)
#define DATASPACE_SIZE (64U * 1024U) // reachable with a 2^16 address

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
  inline void changeBase(const uint32_t newbase);
  void execPrimitive(const Cell token);
  int32_t dstackSize();
  bool createDicoEntry(const Cell token, const std::string& word, const bool immediate);
  bool searchDicoEntry(const std::string& word, uint32_t& ptr);
  void dumpDico();
  void displayDico();

protected:
  uint8_t dictionary[DATASPACE_SIZE];
  Cell data_stack[STACK_SIZE];
  Cell *return_stack[STACK_SIZE];

  // Registers
  Cell  tos;   // Top of (data) Stack
  Cell *dsp;   // Data stack pointer
  Cell **rsp;  // Return stack pointer
  Cell *ip;    // Instruction pointer (CFA of the next word to be executed)

  // Dictionnary
  Cell last;  // NFA of the last dictionnary entry.
  Cell here;  // Address of the next available dictionary location.

  //
  int32_t  base; // Base for display
};

#endif /* FORTH_HPP_ */
