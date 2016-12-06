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
  inline const ForthDico& dictionary() const { return m_dico; }
  inline ForthDico& dictionary() { return m_dico; }
  void displayDStack() const;
  void displayRStack() const;
  // TODO: charge un fichier dico et ecrase le dico ou le charge a la fin
  // loadCompilFile(std::string const& filename, bool append) ou alors boot(std::string const& filename, bool append)
  void restore();
protected:
  void createWord(std::string const& word);
  std::pair<bool, std::string> parseStream();
  void includeFile(std::string const& filename);
  bool toNumber(std::string const& word, Cell32& number);
  void execPrimitive(const Cell16 idPrimitive);
  void execToken(const Cell16 tx);
  inline int32_t RStackDepth() const { return m_rsp - m_return_stack; }
  inline int32_t DStackDepth() const { return m_dsp - m_data_stack; }
  inline bool isPrimitive(const Cell16 id) const { return /*(id >= 0) &&*/ (id < NUM_PRIMITIVES); }
  bool changeDisplayBase(const uint8_t newbase);
  std::string getWord();

protected:
  // Stacks
  Cell32  m_data_stack[STACK_SIZE];
  Cell32  m_alternative_stack[STACK_SIZE];
  Cell32  m_return_stack[STACK_SIZE];

  // Top of (data) Stack
  Cell32  m_tos, m_tos1, m_tos2, m_tos3, m_tos4;

  // Registers
  Cell32 *m_dsp;   // Data stack pointer
  Cell32 *m_asp;   // Alternative data stack pointer
  Cell32 *m_rsp;   // Return stack pointer
  Cell16  m_ip;    // Instruction pointer (CFA of the next word to be executed)
  int32_t m_base;  // Base (octal, decimal, hexa) when displaying numbers
  Cell32  m_state; // compile/execution

  // Backup during the definition of a new word
  int32_t  m_depth_at_colon;
  Cell16  m_last_at_colon;
  Cell16  m_here_at_colon;
  std::string m_creating_word;

  // Backup Forth state when entering in a commentary
  Cell32  m_saved_state;

  // Ascii reader
  ForthReader m_readers[MAX_OPENED_STREAMS];
  uint32_t m_stream;

  // Dictionary
  ForthDico m_dico;

  // Debug
  bool  m_trace;
};

#endif /* FORTH_INNER_HPP_ */
