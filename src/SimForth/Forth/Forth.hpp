#ifndef FORTH_INNER_HPP_
#  define FORTH_INNER_HPP_

//! \brief This file contains the root class for the Forth interpretor.
//! containing other classes like dictionary and stream reader context.

#  include "ForthStream.hpp"
#  include "ForthDictionary.hpp"

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

//! \class Forth
//! \brief class containg the whole Forth interpretor context.
class Forth
{
public:
  //! \brief Constructor with the reference of a Forth dictionary for
  //! an easier inheritance management.
  Forth(ForthDictionary& dico, TextColor &color);
  //! \brief Load all Forth primitives in the dictionary.
  virtual void boot();
  //! \brief interprete a new forth word extracted from a stream.
  void interpreteWord(std::string const& word);
  //! \brief interprete a Forth script stored as a string.
  std::pair<bool, std::string> interpreteString(std::string const& code_fort,
                                                std::string const& name = "<string>");
  //! \brief interprete a Forth script stored as a char*.
  std::pair<bool, std::string> interpreteString(const char* const code_forth,
                                                std::string const& name = "<string>");
  //! \brief interprete a Forth script stored in an ascii file.
  std::pair<bool, std::string> interpreteFile(std::string const& filename);
  //! \brief Display the result prompt after interpreting a script.
  virtual void ok(std::pair<bool, std::string> const& res);
  //! \brief Accessor. Return the reference of the dictionary as const.
  inline const ForthDictionary& dictionary() const { return m_dictionary; }
  //! \brief Accessor. Return the reference of the dictionary.
  inline ForthDictionary& dictionary() { return m_dictionary; }
  //! \brief Accessor. Get the reference of a new forth dictionary.
  inline void dictionary(ForthDictionary& dico) { m_dictionary = dico; }
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
  bool toNumber(std::string const& word, Cell32& number) const;
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
  //! \brief Check if the data stack is not under/overflowing.
  void isDStackUnderOverFlow() const;
  //! \brief Check if the return stack is not under/overflowing.
  int32_t isRStackUnderOverFlow() const;

  //! \brief Change the base of displayed numbers.
  bool changeDisplayBase(const uint8_t base);
protected:
  TextColor &m_color;
  //! Data stack: store function parameters.
  Cell32  m_data_stack_[STACK_SIZE];
  //! Data stack with a marging of security to prevent against stack underflow.
  Cell32  *m_data_stack;
  //! Alternative data stack (secondary stack).
  Cell32  m_alternative_stack_[STACK_SIZE];
  //! Alternative stack with a marging of security to prevent against stack underflow.
  Cell32  *m_alternative_stack;
  //! Return stack: store word tokens (function addresses)
  Cell32  m_return_stack_[STACK_SIZE];
  //! Return stack with a marging of security to prevent against stack underflow.
  Cell32  *m_return_stack;
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
public:
  ForthStream m_streams_stack[MAX_OPENED_STREAMS]; //! A stack of streams when script file include other files
  uint32_t m_opened_streams; //! Number of streams opened.
  ForthDictionary& m_dictionary; //! Forth dictionary.
  bool  m_trace; //! Trace the execution of a word.
  Cell16 m_last_completion;
  int32_t m_err_stream;
};

#endif /* FORTH_INNER_HPP_ */
