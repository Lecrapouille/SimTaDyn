//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTH_INNER_HPP_
#  define FORTH_INNER_HPP_

//! \brief This file contains the root class for the Forth interpretor.
//! containing other classes like dictionary and stream reader context.

#  include "ForthDictionary.hpp"
#  include "ForthClibrary.hpp"
#  include <ostream>

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
  Forth(ForthDictionary& dico);
  ~Forth() { }
  //! \brief Load all Forth primitives in the dictionary.
  virtual void boot();
  //! \brief interprete a new forth word extracted from a stream.
  virtual void interpreteWord(std::string const& word);
  //! \brief interprete a Forth script stored as a string.
  std::pair<bool, std::string> interpreteString(std::string const& code_fort,
                                                std::string const& name = "<string>");
  //! \brief interprete a Forth script stored as a char*.
  virtual std::pair<bool, std::string> interpreteString(const char* const code_forth,
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
  const char* completion(std::string const& partial_name);
  //! \brief Display the data stack.
  virtual void displayStack(std::ostream& stream, const forth::StackID id) const;
  //! \brief restore the Forth context to its initial state.
  void abort();
  //! \brief restore the Forth context to its initial state and throw
  //! an exception.
  void abort(std::string const& msg);
  //! \brief Try converting a Forth word as a number.
  bool toNumber(std::string const& word, Cell32& number) const;
  //! \brief Return the name of the stream which triggereg a fault.
  inline const std::string& nameStreamInFault() const
  {
    return m_streams_stack[m_err_stream].name();
  }
  //!
  void displayDictionary()
  {
    dictionary().display(maxPrimitives());
  }
protected:
  virtual void interpreteWordCaseInterprete(std::string const& word);
  virtual void interpreteWordCaseCompile(std::string const& word);
  //! \brief Create the header of a Forth word in the dictionary.
  void create(std::string const& word);
  //! \brief Get the Forth word in the stream.
  //! Called by Forth words needed to extract the next word. The
  //! stream shall contains at least one word else an exception is
  //! triggered.
  //! \return the next Forth word in the stream.
  //! \throw UnfinishedStream
  inline const std::string& nextWord()
  {
    if (!STREAM.hasMoreWords())
    {
      UnfinishedStream e(m_state);
      throw e;
    }
    return STREAM.nextWord();
  }
  //! \brief
  std::pair<bool, std::string> parseStream();
  //! \brief Parse an included file when parsing a Forth script.
  void includeFile(std::string const& filename);
  //! \brief Perform the action of a Forth primitive.
  virtual void execPrimitive(const Cell16 idPrimitive);
  //! \brief Perform the action of a Forth token (byte code).
  virtual void execToken(const Cell16 token);
  //! \brief Return the depth of the return stack.
public: // FIXME
  inline int32_t stackDepth(const forth::StackID id) const
  {
    switch (id)
      {
      case forth::DataStack:
        return m_dsp - m_data_stack;
      case forth::ReturnStack:
        return m_rsp - m_return_stack;
      case forth::AuxStack:
        return m_asp - m_alternative_stack;
      default:
        LOGES("Pretty print this stack %u is not yet implemented", id);
        return 0;
      }
  }
protected:
  virtual inline uint32_t maxPrimitives() const
  {
    return FORTH_MAX_PRIMITIVES;
  }
  //! \brief Return the token is a Forth primitive (or a user defined word).
  virtual inline bool isPrimitive(const Cell16 token) const
  {
    return /*(token >= 0) &&*/ (token < maxPrimitives());
  }
  //! \brief Check if the data stack is not under/overflowing.
  int32_t isStackUnderOverFlow(const forth::StackID id) const; // FIXME: a renommer en checkStack
  //! \brief Change the base of displayed numbers.
  bool changeDisplayBase(const uint8_t base);

protected:

  //! Data stack: store function parameters.
  Cell32  m_data_stack_[STACK_SIZE];
  //! Data stack with a marging of security to prevent against stack underflow.
  Cell32  *m_data_stack;
  //! Alternative data stack (secondary stack).
  Cell32  m_alternative_stack_[STACK_SIZE];
  //! Alternative stack with a marging of security to prevent against
  //! stack underflow.
  Cell32  *m_alternative_stack;
  //! Return stack: store word tokens (function addresses)
  Cell32  m_return_stack_[STACK_SIZE];
  //! Return stack with a marging of security to prevent against stack
  //! underflow.
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
  ForthStream m_streams_stack[MAX_OPENED_STREAMS]; //! A stack of streams when script file include other files
  ForthCLib m_dynamic_libs; //! Load C dynamic libs and load them as Forth words.
  uint32_t m_opened_streams; //! Number of streams opened.
  ForthDictionary& m_dictionary; //! Forth dictionary.
  bool  m_trace; //! Trace the execution of a word.
  Cell16 m_last_completion;
  int32_t m_err_stream;
};

class ForthStackDiplayer
{
public:
  ForthStackDiplayer(const Forth *const forth, const forth::StackID id)
    : m_forth(forth), m_id(id)
  {
  }

  const Forth *const m_forth;
  forth::StackID m_id;
};

inline std::ostream& operator<<(std::ostream& os, const ForthStackDiplayer& s)
{
  s.m_forth->displayStack(os, s.m_id);
  return os;
}

#endif /* FORTH_INNER_HPP_ */
