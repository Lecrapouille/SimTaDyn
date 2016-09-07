#ifndef FORTH_HPP_
#  define FORTH_HPP_

#  include "ForthHelper.hpp"
#  include "ForthPrimitives.hpp"
#  include "ForthExceptions.hpp"
#  include "ForthReader.hpp"

class Forth
{
public:
  Forth();
  void boot();
  inline void changeDisplayBase(const uint8_t newbase);
  virtual bool eatFile(std::string const& filename);
  virtual bool eatString(std::string const& code_forth);
  virtual bool dumpDico(std::string const& filename);
  virtual void displayDico();

protected:
  inline int32_t RStackSize() const;
  inline bool isPrimitive(const Cell16 id) const;
  void execPrimitive(const Cell16 idPrimitive);
  void execToken(const Cell16 tx);
  void checkDicoBoundaries(Cell16* const ip, std::string const& funcName) const;
  bool toNumber(std::string const& word, Cell32& number);
  virtual void eat(std::string const& word);
  bool createDicoEntry(const Cell16 token, std::string const& word, const bool immediate);
  void dicoAppendCell16(Cell16 value);
  Cell16 readCell16at(const Cell8 *addr);
  Cell16 readCell16at(const Cell16 *addr);
  void writeCell16at(Cell8 *addr, Cell16 data);
  bool toToken(std::string const& wordName, Cell16& token, Cell16& immediate);

protected:
  Cell8   dictionary[DATASPACE_SIZE];
  Cell32  data_stack[STACK_SIZE];
  Cell16 *return_stack[STACK_SIZE];

  // Registers
  Cell32  tos, tos1, tos2, tos3, tos4;   // Top of (data) Stack
  Cell32 *dsp;   // Data stack pointer
  Cell16 **rsp;  // Return stack pointer
  Cell16 *ip;    // Instruction pointer (CFA of the next word to be executed)

  // Dictionnary
  Cell16 last;  // NFA of the last dictionnary entry.
  Cell16 here;  // Address of the next available dictionary location.

  // Base (octal, decimal, hexa) when displaying numbers
  int32_t  base;

  Cell32  state; // compile/execution
  //Cell32  mode;  // deep compile/execution

  // Eat ascii code
  ForthTxtReader reader;
};

#endif /* FORTH_HPP_ */
