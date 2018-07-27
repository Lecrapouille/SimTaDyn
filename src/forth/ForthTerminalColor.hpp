//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTH_TERMINAL_COLOR_HPP_
#  define FORTH_TERMINAL_COLOR_HPP_

//------------------------------------------------------------------
//! file This file allows to define syntaxical coloration when
//! displaying Forth code or Forth dictionary.
//------------------------------------------------------------------

#  include "TerminalColor.hpp"

//------------------------------------------------------------------
// Color for error messages
//------------------------------------------------------------------
#define FORTH_ERROR_COLOR                               \
  termcolor::color(termcolor::style::bold, termcolor::fg::red)

//------------------------------------------------------------------
// Color for warning messages
//------------------------------------------------------------------
#define FORTH_WARNING_COLOR                             \
  termcolor::color(termcolor::style::bold, termcolor::fg::yellow)

//------------------------------------------------------------------
// Color for normal messages
//------------------------------------------------------------------
#define FORTH_NORMAL_COLOR                              \
  termcolor::color()

//------------------------------------------------------------------
// Color for dictionary addresses
//------------------------------------------------------------------
#define DICO_ADDRESS_COLOR                              \
  termcolor::color(termcolor::style::normal, termcolor::fg::gray)

//------------------------------------------------------------------
// Color for deleted Forth words
//------------------------------------------------------------------
#define SMUDGED_WORD_COLOR                              \
  termcolor::color(termcolor::style::normal, termcolor::fg::gray)

//------------------------------------------------------------------
// Color for immediate Forth words
//------------------------------------------------------------------
#define IMMEDIATE_WORD_COLOR                            \
  termcolor::color(termcolor::style::bold, termcolor::fg::yellow)

//------------------------------------------------------------------
// Color for primitive Forth words
//------------------------------------------------------------------
#define PRIMITIVE_WORD_COLOR                            \
  termcolor::color(termcolor::style::bold, termcolor::fg::blue)

//------------------------------------------------------------------
// Color for non-primitive Forth words
//------------------------------------------------------------------
#define NON_PRIMITIVE_WORD_COLOR                        \
  termcolor::color(termcolor::style::bold, termcolor::fg::red)

//------------------------------------------------------------------
// Color for displaying token
//------------------------------------------------------------------
#define COLOR_EXEC_TOKEN                                \
  termcolor::color(termcolor::style::normal, termcolor::fg::cyan)

//------------------------------------------------------------------
// Color for displaying numbers
//------------------------------------------------------------------
#define LITERAL_COLOR                                   \
  termcolor::color(termcolor::style::bold, termcolor::fg::green)

//------------------------------------------------------------------
// Default color for displaying dictionary
//------------------------------------------------------------------
#define DICO_DEFAULT_COLOR                              \
  termcolor::color(termcolor::style::bold, termcolor::fg::gray)

#endif // FORTH_TERMINAL_COLOR_HPP_
