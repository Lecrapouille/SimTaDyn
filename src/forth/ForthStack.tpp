// -*- c++ -*- Coloration Syntaxique pour Emacs
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

#ifndef FORTH_STACK_HPP_
#  define FORTH_STACK_HPP_

//------------------------------------------------------------------
//! \file This file define the class stack for storing parameters of
//! Forth words (data stack) or for saving words to visit when
//! interpreting a word (return stack).
//------------------------------------------------------------------

//#  include "ForthCells.hpp"
#  include "ForthExceptions.hpp"
#  include <iostream>

// **************************************************************
// Stack size
// **************************************************************

//! \brief Stack depth (in number of Cells).
#  define FORTH_STACK_DEPTH           (1024u) // of Forth Cells
//! Rerserve additional bytes to detect stack underflows and prevent
//! data corruption (in number of Cells).
#  define FORTH_STACK_SECURITY_MARGIN (8u)   // Forth Cells

namespace forth
{

// **************************************************************
//! \brief Forth Stack class. T is the type of Forth Cell (Cell32).
// **************************************************************
template <typename T>
class Stack
{
public:

  //------------------------------------------------------------------
  //! \brief Constructor. Initialize an empty stack. The name passed
  //! as param is used for logs.
  //------------------------------------------------------------------
  Stack(const char *name)
    : m_sp(m_sp0), m_name(name)
  {
    static_assert(FORTH_STACK_DEPTH > (2u * FORTH_STACK_SECURITY_MARGIN));
  }

  //------------------------------------------------------------------
  //! \brief Reset the stack to an empty state.
  //------------------------------------------------------------------
  inline void reset() { m_sp = m_sp0; }

  //------------------------------------------------------------------
  //! \brief Return the current depth of the stack.
  //------------------------------------------------------------------
  inline int32_t depth() const { return m_sp - m_sp0; }

  //------------------------------------------------------------------
  //! \brief Insert a Forth Cell on the top of the stack
  //------------------------------------------------------------------
  inline void push(T const n) { *(m_sp++) = n; }

  //------------------------------------------------------------------
  //! \brief Remove the top of stack
  //------------------------------------------------------------------
  inline void drop() { --m_sp; }

  //------------------------------------------------------------------
  //! \brief Consum the top of stack
  //------------------------------------------------------------------
  inline T pop() { return *(--m_sp); }

  //------------------------------------------------------------------
  //! \brief Consum the Nth element of stack from its top
  //------------------------------------------------------------------
  inline T pick(T const n) { return *(m_sp - n - 1); }

  //------------------------------------------------------------------
  //! \brief Check if the stack has overflowed.
  //------------------------------------------------------------------
  inline bool overflow() const
  {
    return depth() > ((int32_t) (FORTH_STACK_DEPTH - (2 * FORTH_STACK_SECURITY_MARGIN)));
  }

  //------------------------------------------------------------------
  //! \brief Check if the stack has underflowed.
  //------------------------------------------------------------------
  inline bool underflow() const { return depth() < -1; }

  //------------------------------------------------------------------
  //! \brief Check if the stack has overflowed or underflowed. Trigger
  //! and exception if it's the case.
  //------------------------------------------------------------------
  void check()
  {
    if (overflow())
      throw ForthException(MSG_EXCEPTION_FORTH_STACK_OVERFLOW(m_name));

    if (underflow())
      throw ForthException(MSG_EXCEPTION_FORTH_STACK_UNDERFLOW(m_name));
  }

  //------------------------------------------------------------------
  //! \brief Display the content of a Forth stack. Element are displayed
  //! in the current base (initialy decimal).
  //------------------------------------------------------------------
  friend std::ostream& operator<<(std::ostream& os, Stack<T>& stack)
  {
    os << stack.m_name;
    if (stack.overflow())
      os << "<overflowed>";
    else if (stack.underflow())
      os << "<underflowed>";
    else
      {
        // Display the stack depth
        os << '<' << stack.depth() << ">:";

        // Display stack elements into the current base
        //std::ios_base::fmtflags ifs(os.flags());
        //std::setbase(m_base);
        T* sp = stack.m_sp0;
        while (sp != stack.m_sp)
          {
            os << ' ' << /*std::setbase(m_base) <<*/ (int32_t) *sp++;
          }
        //os.flags(ifs);
      }
    return os;
  }

private:

  //------------------------------------------------------------------
  //! \brief A stack is a memory segment of Forth cells.
  //------------------------------------------------------------------
  T            m_data[FORTH_STACK_DEPTH];

  //------------------------------------------------------------------
  //! \brief Initial address of the top of the stack (fix address).
  //------------------------------------------------------------------
  T* const m_sp0 = m_data + FORTH_STACK_SECURITY_MARGIN;

  //------------------------------------------------------------------
  //! \brief Top of stack.
  //------------------------------------------------------------------
  T           *m_sp;

  //------------------------------------------------------------------
  //! \brief Name of the stack for logs and c++ exceptions.
  //------------------------------------------------------------------
  const char  *m_name;
};

} // namespace

#endif // FORTH_STACK_HPP_
