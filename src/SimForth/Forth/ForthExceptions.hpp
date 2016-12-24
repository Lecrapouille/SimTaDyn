#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

//! \brief This file defined all exceptions used for the Forth parser.
//! Exceptions are derived from the project common ancestor SimTaDyn::Exception
//! which is also derived from the POCO library execption.

#  include "Exception.hpp"
#  include "ForthHelper.hpp"

//! This macro (from the library POCO) will declare a class
//! ForthException derived from SimTaDyn::Exception.
POCO_DECLARE_EXCEPTION(ForthException, SimTaDyn::Exception);

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
      case COMPILATION_STATE:
        m_msg = "Word definition not terminated when reaching the end of the stream";
        break;
      case COMMENT_STATE:
        m_msg = "Commentary not terminated when reaching the end of the stream";
        break;
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
    m_msg = "too many opened streams";
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
  // FIXME SimForth::StackId au lieu de uint32_t
  OutOfBoundStack(const uint32_t stack_id, const int32_t depth)
    : ForthException(36)
  {
    m_stack_id = stack_id;
    m_depth = depth;

    switch (m_stack_id)
      {
      case DATA_STACK:
        m_msg = "Data Stack";
      case RETURN_STACK:
        m_msg = "Return Stack";
      default:
        m_msg = "";
      }

    if (depth < 0)
      {
        m_msg += " underflow";
      }
    else
      {
        m_msg += " overflow";
      }
  }

  uint32_t m_stack_id;
  int32_t m_depth;
};

// **************************************************************
//
// **************************************************************
class UnknownForthPrimitive: public ForthException
{
public:
  UnknownForthPrimitive(const Cell16 badToken, std::string const& funcName)
    : ForthException(35)
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
    : ForthException(34)
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
