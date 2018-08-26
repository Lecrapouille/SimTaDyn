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

#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

//------------------------------------------------------------------
//! \brief This file defined all exceptions used for the Forth parser.
//! Exceptions are derived from the project common ancestor
//! SimTaDyn::Exception which is also derived from the POCO library
//! execption.
//------------------------------------------------------------------

#  include "Exception.hpp"

//! This macro (from the library POCO) will declare a class
//! ForthException derived from simtadyn::Exception.
DECLARE_EXCEPTION(ForthException, Exception)

#  define MSG_EXCEPTION_ABORT_FORTH(msg)            \
  "Abort with message '" + msg + "'"

#  define MSG_EXCEPTION_UNKNOWN_FORTH_WORD(word)    \
  "Unrecognized word '" + word + "'"

#  define MSG_EXCEPTION_UNKNOWN_FORTH_PRIMITIVE(xt, word)               \
  "Tried to execute an unknown token " +                                \
  std::to_string((uint32_t) xt) + " from " + word;

#  define MSG_EXCEPTION_FORTH_STACK_OVERFLOW(stack)     \
  std::string(stack) + " overflow"

#  define MSG_EXCEPTION_FORTH_STACK_UNDERFLOW(stack)    \
  std::string(stack) + " underflow"

#  define MSG_EXCEPTION_FORTH_STACK_CHANGED_DURING_DEF(word)       \
  "Data stack depth changed during the definition of the word '"   \
  + word + "' probably unbalanced conditionals"

#  define MSG_EXCEPTION_FORTH_DICTIONARY_IS_FULL                \
  "The Forth dictionary is full"

#  define MSG_EXCEPTION_FORTH_DICTIONARY_ALLOT  \
  "Cannot allot memory in the dictionary: not enough space"

#  define MSG_EXCEPTION_FORTH_DICTIONARY_FREE  \
  "Cannot free memory in the dictionary: bounding error"

#  define MSG_EXCEPTION_FORTH_DICTIONARY_OUT_OF_BOUND(addr)     \
  "Tried to access dictionary memory " + std::to_string(addr)   \
  + " outside its bounds"

#  define MSG_EXCEPTION_TOO_MANY_OPENED_FORTH_STREAMS   \
  "Reached the max depth of file inclusions"

#  define MSG_EXCEPTION_UNFINISHED_STREAM       \
  "Unfinished stream"

#  define MSG_EXCEPTION_UNFINISHED_FORTH_COMMENT(line, column)          \
  "Commentary started at " + std::to_string(line) + ":"                 \
  + std::to_string(column) + " not terminated when reaching the end of the stream"

#  define MSG_EXCEPTION_UNFINISHED_FORTH_DEFINITION(line, column)       \
  "Word definition started at " + std::to_string(line) + ":"            \
  + std::to_string(column) + " not terminated when reaching the end of the stream"

#  define MSG_EXCEPTION_FORTH_NAME_TOO_LONG(word)               \
  "Length name for '" + std::string(word) + "' contains more than 31 chars"

#  define MSG_EXCEPTION_FORTH_DEFINITION_TOO_LONG                       \
  "Tried to create a word containing more than 2^16 words on its definition"

#  define MSG_EXCEPTION_FORTH_SAVE_DICO_NO_FILE(filename)       \
  "Cannot save the dictionary to the file '" + filename       \
  + "'. Reason is '" + strerror(errno) + "'"

#  define MSG_EXCEPTION_FORTH_FAILED_LOADING_DICO(filename)             \
  "Failed when loading the dictionary with the file '" + filename       \
  + "'. Reason is '" + strerror(errno) + "'"

#  define MSG_EXCEPTION_FORTH_LOAD_DICO_NO_FILE(filename)       \
  "Cannot load the dictionary from the file '" + filename       \
  + "'. Reason is '" + strerror(errno) + "'"

#  define MSG_EXCEPTION_FORTH_LOAD_DICO_NO_SPACE(filename)              \
  "Cannot load the dictionary from the file '" + filename               \
  + "'. Reason is the image is bigger than the dictionary size"

#endif // FORTH_EXCEPTIONS_HPP_
