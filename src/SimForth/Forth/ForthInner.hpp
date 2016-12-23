#ifndef FORTH_INNER_HPP_
#  define FORTH_INNER_HPP_

//! \brief This file contains the root class for the Forth interpretor.
//! containing other classes like dictionary and stream reader context.

#  include "ForthStream.hpp"
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

// TODO: charge un fichier dico et ecrase le dico ou le charge a la fin
// loadCompilFile(std::string const& filename, bool append) ou alors boot(std::string const& filename, bool append)


//! \class Forth
//! \brief class containg the whole Forth interpretor context.
class Forth
{
public:
  //! \brief Constructor with the reference of a Forth dictionary for
  //! an easier inheritance management.
  Forth(ForthDico& dico);
  //! \brief Load all Forth primitives in the dictionary.
  virtual void boot();
  //! \brief interprete a new forth word extracted from a stream.
  void interpreteWord(std::string const& word);
  //! \brief interprete a Forth script stored as a string.
  std::pair<bool, std::string> interpreteString(std::string const& code_forth);
  //! \brief interprete a Forth script stored as a char*.
  std::pair<bool, std::string> interpreteString(const char* const code_forth);
  //! \brief interprete a Forth script stored in an ascii file.
  std::pair<bool, std::string> interpreteFile(std::string const& filename);
  //! \brief Display the result prompt after interpreting a script.
  virtual void ok(std::pair<bool, std::string> const& res);
  //! \brief Accessor. Return the reference of the dictionary as const.
  inline const ForthDico& dictionary() const { return m_dico; }
  //! \brief Accessor. Return the reference of the dictionary.
  inline ForthDico& dictionary() { return m_dico; }
  //! \brief Accessor. Get the reference of a new forth dictionary.
  inline void dictionary(ForthDico& dico) { m_dico = dico; }
  //! \brief Complete the name a forth word from the dictionary.
  void completion(std::string const& partial_name);
  //! \brief Display the data stack.
  virtual void displayDStack() const;
  //! \brief Display the return stack.
  virtual void displayRStack() const;
  //! \brief restore the Forth context to its initial state.
  void abort();
  //! \brief restore the Forth context to its initial state and throw an exception.
  void abort(std::string const& msg);
protected:
  //! \brief Create the header of a Forth word in the dictionary.
  void create(std::string const& word);
  //! \brief Get the Forth word in the stream.
  std::string nextWord();
  //! \brief
  std::pair<bool, std::string> parseStream();
  //! \brief Parse an included file when parsing a Forth script.
  void includeFile(std::string const& filename);
  //! \brief Try converting a Forth word as a number.
  bool toNumber(std::string const& word, Cell32& number);
  //! \brief Perform the action of a Forth primitive.
  void execPrimitive(const Cell16 idPrimitive);
  //! \brief Perform the action of a Forth token (byte code).
  void execToken(const Cell16 token);
  //! \brief Return the depth of the return stack.
  inline int32_t RStackDepth() const
  {
    return m_rsp - m_return_stack;
  }
  //! \brief Return the depth of the data stack.
  inline int32_t DStackDepth() const
  {
    return m_dsp - m_data_stack;
  }
  //! \brief Return the token is a Forth primitive (or a user defined word).
  inline bool isPrimitive(const Cell16 token) const
  {
    return /*(token >= 0) &&*/ (token < NUM_PRIMITIVES);
  }
  //! \brief Change the base of displayed numbers.
  bool changeDisplayBase(const uint8_t base);

protected:
  //! Data stack: store function parameters.
  Cell32  m_data_stack[STACK_SIZE];
  //! Alternative data stack (secondary stack).
  Cell32  m_alternative_stack[STACK_SIZE];
  //! Return stack: store word tokens (function addresses)
  Cell32  m_return_stack[STACK_SIZE];
  //! Top of Stack
  Cell32  m_tos, m_tos1, m_tos2, m_tos3, m_tos4;
  // Registers
  Cell32 *m_dsp;   //! Data stack pointer
  Cell32 *m_asp;   //! Alternative data stack pointer
  Cell32 *m_rsp;   //! Return stack pointer
  Cell16  m_ip;    //! Instruction pointer (CFA of the next word to be executed)
  int32_t m_base;  //! Base (octal, decimal, hexa) when displaying numbers
  Cell32  m_state; //! compile/execution
  int32_t  m_depth_at_colon; //! Save the stack depth before creating a new Forth word.
  Cell16  m_last_at_colon;   //! Save the last dictionary entry before creating a new Forth word.
  Cell16  m_here_at_colon;   //! Save the last dictionary free slot before creating a new Forth word.
  std::string m_creating_word; //! The Forth word currently in creation.
  Cell32  m_saved_state; //! Save the interpreter state when enetring in a comment.
  ForthStream m_streams_stack[MAX_OPENED_STREAMS]; //! A stack of streams when script file include other files
  uint32_t m_opened_streams; //! Number of streams opened.
  ForthDico& m_dico; //! Forth dictionary.
  bool  m_trace; //! Trace the execution of a word.
  Cell16 m_last_completion;
};

#endif /* FORTH_INNER_HPP_ */
