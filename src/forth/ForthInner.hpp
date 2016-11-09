#ifndef FORTH_INNER_HPP_
#  define FORTH_INNER_HPP_

#  include "ForthReader.hpp"
#  include "ForthDico.hpp"

// **************************************************************
// When Forth reads a number which does not fit in the expected
// number of bits, what is the expected strategy ? Ignore it as
// a number which probably produce an undefined dictionary word
// or truncated it with a warning message.
// **************************************************************
#define FORTH_OUT_OF_RANGE_NUMBERS_ARE_WORDS 0
#define FORTH_TRUNCATE_OUT_OF_RANGE_NUMBERS  1
#ifndef FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE
#  define FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE FORTH_TRUNCATE_OUT_OF_RANGE_NUMBERS
#endif

class Forth
{
public:
  Forth();
  void boot();
  void interprete(std::string const& word);
  std::pair<bool, std::string> eatString(std::string const& code_forth);
  std::pair<bool, std::string> eatString(const char* const code_forth);
  std::pair<bool, std::string> eatFile(std::string const& filename);
  virtual void ok(std::pair<bool, std::string> const& res);
  const ForthDico& dictionary() const;
  // TODO: charge un fichier dico et ecrase le dico ou le charge a la fin
  // loadCompilFile(std::string const& filename, bool append) ou alors boot(std::string const& filename, bool append)

protected:
  std::pair<bool, std::string> parseStream();
  bool toNumber(std::string const& word, Cell32& number);
  void execPrimitive(const Cell16 idPrimitive);
  void execToken(const Cell16 tx);
  int32_t RStackDepth() const;
  int32_t DStackDepth() const;
  inline bool isPrimitive(const Cell16 id) const;
  inline void changeDisplayBase(const uint8_t newbase);
  inline void restore();
  std::string getWord();

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

  // Backup during the definition of a new word
  int32_t  m_depth_at_colon;
  Cell16  m_last_at_colon;
  Cell16  m_here_at_colon;
  std::string m_creating_word;

  // Backup Forth state when entering in a commentary
  Cell32  m_saved_state;

  // Ascii reader
  ForthReader m_reader;

  // Dictionary
  ForthDico m_dico;
};

#endif /* FORTH_INNER_HPP_ */
