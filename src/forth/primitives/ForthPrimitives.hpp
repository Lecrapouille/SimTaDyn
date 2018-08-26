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

#ifndef FORTH_PRIMITIVES_HPP_
#  define FORTH_PRIMITIVES_HPP_

#define LABELIZE(a)                      [TOK_##a] = &&L_##a
#define ENTRY(a)                         a, ((sizeof a) - 1U)
#define COMPILE(token, name, immediate)  m_dictionary.compileWord(TOK_ ## token, ENTRY(name), immediate)
#define PRIMITIVE(token, name)           COMPILE(token, name, false)
#define IMMEDIATE_PRIMITIVE(token, name) COMPILE(token, name, true)

// **************************************************************
// Data stack (function parameters manipulation) short names
#  define DPUSH(n) m_data_stack.push(n)             // Store the cell value on the top of stack
#  define DDROP()  m_data_stack.drop()              // Discard the top of the stack
#  define DPOP()   m_data_stack.pop()               // Discard the top of the stack and save its value in the register r
#  define DPICK(n) m_data_stack.pick(n)             // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

// **************************************************************
// Return stack (word addresses manipulation) short names
#  define RPUSH(n) m_return_stack.push(n)             // Store the cell value on the top of stack
#  define RDROP()  m_return_stack.drop()              // Discard the top of the stack
#  define RPOP( )  m_return_stack.pop()               // Discard the top of the stack and save its value in the register r
#  define RPICK(n) m_return_stack.pick(n)             // Look at the nth element (n >= 1) of the stack from the top (1 = 1st element)

//------------------------------------------------------------------
//! \file This file defines two ways for defining primitives:
//! -- the first uses a classic switch(token) { case XXX: ... }
//! -- the second uses computed goto with is 25% faster than switch
//------------------------------------------------------------------

#  ifdef USE_COMPUTED_GOTO

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define DISPATCH(xt) goto *c_primitives[xt];

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define CODE(a)    L_##a:

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define NEXT return
  //if (*m_ip < FORTH_MAX_PRIMITIVES) goto *c_primitives[*m_ip++];
  //else throw ForthException("unknown token");

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define UNKNOWN    L_UNKNOWN

// *****************************************************************
#  else // !USE_COMPUTED_GOTO
// *****************************************************************

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define DISPATCH(xt) switch (xt)

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define CODE(a)    case TOK_##a:

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define NEXT       break

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
#    define UNKNOWN    default

#  endif // USE_COMPUTED_GOTO
#endif // FORTH_PRIMITIVES_HPP_
