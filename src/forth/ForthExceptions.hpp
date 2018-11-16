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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

//! \brief This file defined all exceptions used for the Forth parser.
//! Exceptions are derived from the project common ancestor SimTaDyn::Exception
//! which is also derived from the POCO library execption.

#  include "Exception.hpp"
#  include "ForthHelper.hpp"

//! This macro (from the library POCO) will declare a class
//! ForthException derived from simtadyn::Exception.
DECLARE_EXCEPTION(ForthException, Exception)

// **************************************************************
//
// **************************************************************
class UnfinishedStream: public ForthException
{
public:
  UnfinishedStream(const Cell32 Forthstate)
    : ForthException(42)
  {
    switch (Forthstate)
      {
      case forth::Compile:
        m_msg = "Word definition not terminated when reaching the end of the stream";
        break;
      case forth::Comment:
        m_msg = "Commentary not terminated when reaching the end of the stream";
        break;
      case forth::Interprete:
      default:
        // This case is not possible
        assert(1);
        break;
      }
  }
};

// **************************************************************
//
// **************************************************************
class TooManyOpenedStreams: public ForthException
{
public:
  TooManyOpenedStreams()
    : ForthException(41)
  {
    m_msg = "Including nested files too deeply: too many opened streams";
  }
};


// **************************************************************
//
// **************************************************************
class ModifiedStackDepth: public ForthException
{
public:
  ModifiedStackDepth(std::string const& word)
    : ForthException(41)
  {
    m_msg = "Stack depth changed during the definition of the word '" + word + "' probably unbalanced condition";
  }
};

// **************************************************************
//
// **************************************************************
class ForthDefinition: public ForthException
{
public:
  ForthDefinition(std::string const& word)
    : ForthException(40)
  {
    m_msg = "Exception from SimTaDynForth: word '" + word + "' contains more than 2^16 words.";
  }
};

// **************************************************************
//
// **************************************************************
class MalformedForthWord: public ForthException
{
public:
  MalformedForthWord(std::string const& word)
    : ForthException(39)
  {
    m_msg = "Exception from SimTaDynForth: word '" + word + "' is mal formed. It shall contain 1 ... 31 characters.";
  }
};

// **************************************************************
//
// **************************************************************
class OutOfBoundDictionary: public ForthException
{
public:
  OutOfBoundDictionary(const uint32_t addr)
    : ForthException(38)
  {
    m_msg = "Trying to access out of the dictionary bounds ("
      + std::to_string(addr) + ')';
  }
};

// **************************************************************
//
// **************************************************************
class NoSpaceDictionary: public ForthException
{
public:
  NoSpaceDictionary()
    : ForthException(37)
  {
    m_msg = "The Forth dictionary is full";
  }
};

// **************************************************************
//
// **************************************************************
class OutOfBoundStack: public ForthException
{
public:

  OutOfBoundStack(const forth::StackID stack_id, const int32_t depth);

  uint32_t m_stack_id;
};


// **************************************************************
//
// **************************************************************
class UnknownForthPrimitive: public ForthException
{
public:
  UnknownForthPrimitive(const Cell16 badToken, std::string const& funcName)
    : ForthException(36)
  {
    m_msg = "Exception from SimTaDynForth: try to execute an unknown primitive "
      + std::to_string((uint32_t) badToken) + " in " + funcName;
  }
};

// **************************************************************
//
// **************************************************************
class UnknownForthWord: public ForthException
{
public:
  UnknownForthWord(std::string const& word)
    : ForthException(35)
  {
    m_msg = "Unrecognized word '" + word + "'";
  }
};

// **************************************************************
//
// **************************************************************
class AbortForth: public ForthException
{
public:
  AbortForth(std::string const& msg)
    : ForthException(34)
  {
    m_msg = "Aborting '" + msg + "'";
  }
};

#endif /* FORTH_EXCEPTIONS_HPP_ */
