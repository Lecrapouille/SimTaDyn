#ifndef FORTH_INNER_HPP_
#  define FORTH_INNER_HPP_

#  include "ForthReader.hpp"
#  include "ForthDico.hpp"

class Forth
{
public:
  Forth();
  void boot();
  void interprete(std::string const& word);
  std::pair<bool, std::string> eatString(std::string const& code_forth);
  std::pair<bool, std::string> eatString(const char* const code_forth);
  std::pair<bool, std::string> eatFile(std::string const& filename);
  // TODO: std::pair<bool, std::string> eatString(); std::cin
  const ForthDico& dictionary() const;
  // TODO: charge un fichier dico et ecrase le dico ou le charge a la fin
  // loadCompilFile(std::string const& filename, bool append) ou alors boot(std::string const& filename, bool append)

protected:
  std::pair<bool, std::string> parseStream();
  bool toNumber(std::string const& word, Cell32& number);
  void execPrimitive(const Cell16 idPrimitive);
  void execToken(const Cell16 tx);
  inline int32_t RStackDepth() const;
  inline int32_t DStackDepth() const;
  inline bool isPrimitive(const Cell16 id) const;
  inline void changeDisplayBase(const uint8_t newbase);

protected:
  // Stacks
  Cell32  data_stack[STACK_SIZE];
  Cell16 *return_stack[STACK_SIZE];

  // Top of (data) Stack
  Cell32  m_tos, m_tos1, m_tos2, m_tos3, m_tos4;

  // Registers
  Cell32 *m_dsp;   // Data stack pointer
  Cell16 **m_rsp;  // Return stack pointer
  Cell16 *m_ip;    // Instruction pointer (CFA of the next word to be executed)
  int32_t  m_base; // Base (octal, decimal, hexa) when displaying numbers
  Cell32  m_state; // compile/execution
  Cell32  m_saved_state;

  // Ascii reader
  ForthReader m_reader;

  // Dictionary
  ForthDico m_dico;
};

#endif /* FORTH_INNER_HPP_ */
