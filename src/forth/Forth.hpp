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

#ifndef FORTH_INTERPRETER_HPP_
#  define FORTH_INTERPRETER_HPP_

//------------------------------------------------------------------
//! \file This file defines the Forth interpreter following the 83
//! standard: dictionary, data stack, return stack can interprete
//! scripts from a stream (file or string).
//------------------------------------------------------------------

#  include "ForthExceptions.hpp"
#  include "ForthDictionary.hpp"
#  include "ForthStack.tpp"
#  include "ForthStream.hpp"


// **************************************************************
#define MAX_OPENED_STREAMS   (16U)    // depth of INCLUDE calls

enum S { CURRENT, SAVED };

// **************************************************************
//! \brief Forth interpreter mode
// **************************************************************
namespace forth
{
  enum class state
    {
      //! \brief Forth interpreter in interpreting mode.
      Interprete,
      //! \brief Forth interpreter in compilation mode.
      Compile,
      Comment
    };

  // **************************************************************
  //! \brief When a Forth interpreter see that a script includes other
  //! scripts, it will push them in a stack and pop them when the end of
  //! the file is reached.
  // **************************************************************
  struct StreamStack
  {
    //! The stack of opened streams.
    forth::Stream stack[MAX_OPENED_STREAMS];
    //! Top of the stack pointer. The stack depth indicates the number
    //! of opened streams.
    uint32_t    opened;
    //! When the Forth interpreter detects a fault, save the stream
    //! which produced the fault.
    uint32_t    faulty;
  };
}

// **************************************************************
//! \brief Forth interpreter context
// **************************************************************
class Forth
{
public:

  //------------------------------------------------------------------
  //! \brief Constructor with the reference of a Forth dictionary for
  //! an easier inheritance management. A minimalist Forth is avalaible
  //! an if you want to load more stuffs on it call the boot() method.
  //------------------------------------------------------------------
  Forth(forth::Dictionary& dico);

  //------------------------------------------------------------------
  //! \brief Destructor
  //------------------------------------------------------------------
  ~Forth();

  //! \brief Load all Forth primitives in the dictionary.
  virtual void boot();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void save(std::string const& filename) const
  {
    m_dictionary.save(filename);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void load(std::string const& filename, const bool replace)
  {
    m_dictionary.load(filename, replace);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void displayDictionary() const
  {
    m_dictionary.display(); //FIXME maxPrimitives());
  }

  std::pair<bool, std::string> interpreteStream();

  std::pair<bool, std::string>
  interpreteString(std::string const& forth_script, std::string const& script_name = "<string>");

  std::pair<bool, std::string> interpreteFile(std::string const& filename);

  void ok(std::pair<bool, std::string> const& res);

  inline const char* completeWordName(std::string const& partial_name) const
  {
    return m_dictionary.completeWordName(partial_name);
  }

  //! \brief Accessor. Return the reference of the dictionary as const.
  inline const forth::Dictionary& dictionary() const { return m_dictionary; }
  //! \brief Accessor. Return the reference of the dictionary.
  inline forth::Dictionary& dictionary() { return m_dictionary; }

protected:

  const std::string& nextWord();
  virtual uint32_t maxPrimitives() const;

private:

  //------------------------------------------------------------------
  //! \brief Reset the Forth context to its initial state.
  //------------------------------------------------------------------
  void reset();

  //------------------------------------------------------------------
  //! \brief When Forth displays numbers on screen, it convert
  //! them into the current base. This getter allows to change
  //! the current base.
  //------------------------------------------------------------------
  bool setDisplayBase(const uint8_t base);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  bool toNumber(std::string const& word, forth::cell& number) const;

  //------------------------------------------------------------------
  //! \brief Throw an exception which will abort the Forth
  //! interpreter.
  //! \throw ForthException with a message inside.
  //! \param msg the message to pass in the exception.
  //------------------------------------------------------------------
  inline void abort(std::string const& msg) const
  {
    throw ForthException(MSG_EXCEPTION_ABORT_FORTH(msg));
  }

  void executeToken(forth::token const token);

  // FIXME a placer dans le code du mot :
  void startCompilingWord(std::string const& word);

  void parseStream();

  void includeFile(std::string const& filename);
  void skipCommentary();

  /*inline   forth::Stream& currentStream()
  {
    return m_streams.stack[m_streams.opened];
  }

  inline   forth::Stream const& currentStream() const
  {
    return m_streams.stack[m_streams.opened];
  }

  inline   forth::Stream const& faultyStream() const
  {
    return m_streams.stack[m_streams.faulty];
    }*/

private:

  //! \brief Forth dictionary.
  forth::Dictionary& m_dictionary;
  //! \brief Return stack: store word address when executing a word.
  //! \note while the return stack is made for stoeing tokens, in classic
  //! Forth it also stores temporarily elements of the data stack and
  //! therefore the type should be the same.
  forth::Stack<forth::cell> m_return_stack;
  //! \brief Data stack: store function parameters (operands).
  forth::Stack<forth::cell> m_data_stack;
  //! \brief Alternative data stack (secondary stack).
  //ForthStack<forth::cell> m_auxiliary_stack;
  //! \brief Registers: Top Of Stack
  forth::cell m_tos, m_tos1, m_tos2, m_tos3, m_tos4;
  //! \brief Instruction pointer (CFA of the next word to be executed)
  forth::token m_ip;
  //! \brief Current and previous Forth interpreter state: compile, execution
  forth::state m_state[2];
  //! \brief Current base (octal, decimal, hexa) when displaying numbers
  uint8_t m_base;
  //! \brief Save informations on streams currently reading by Forth.
  forth::StreamStack m_streams;
  //! \brief Load C dynamic libs and load them as Forth words.
  //ForthCLib m_clibs;
};

#endif
