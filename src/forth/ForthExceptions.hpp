#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

#  include "ForthHelper.hpp"

// **************************************************************
//
// **************************************************************
class ForthException: public std::exception
{
public:
  ForthException() throw ()
  {
    m_error_msg = "Ambiguous condition";
  }
  ForthException(std::string const& word) throw ()
  {
    m_error_msg = word;
  }
  ~ForthException() throw ()
  {
  }

  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }

  std::string m_error_msg;
};

// **************************************************************
//
// **************************************************************
class ForthTruncatedStream: public ForthException
{
public:
  ForthTruncatedStream(const Cell32 Forthstate)
  {
    switch (Forthstate)
      {
      case COMPILATION_STATE:
        m_error_msg = "Word definition not terminated when reaching the end of the stream";
        break;
      case COMMENT_STATE:
        m_error_msg = "Commentary not terminated when reaching the end of the stream";
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
class ForthUnbalancedDef: public ForthException
{
public:
  ForthUnbalancedDef(std::string const& word)
  {
    m_error_msg = "Stack depth changed during the definition of the word '" + word + "' probably unbalanced condition";
  }
};

// **************************************************************
//
// **************************************************************
class ForthTooLongDef: public ForthException
{
public:
  ForthTooLongDef(std::string const& word)
  {
    m_error_msg = "Exception from SimTaDynForth: word '" + word + "' contains more than 2^16 words.";
  }
};

// **************************************************************
//
// **************************************************************
class ForthMalformedWord: public ForthException
{
public:
  ForthMalformedWord(std::string const& word)
  {
    m_error_msg = "Exception from SimTaDynForth: word '" + word + "' is mal formed. It shall contain 1 ... 31 characters.";
  }
};

class ForthDicoOOB: public ForthException
{
public:
  ForthDicoOOB(const uint32_t addr) throw ()
  {
    m_error_msg = "Exception from SimTaDynForth: " + std::to_string(addr) + "is an invalid dictionary address ";
  }
};

// **************************************************************
//
// **************************************************************
class ForthDicoNoSpace: public ForthException
{
public:
  ForthDicoNoSpace()
  {
    m_error_msg = "Exception from SimTaDynForth: the dictionary has no more space";
  }
};

// **************************************************************
//
// **************************************************************
class ForthStackOV: public ForthException
{
public:
  ForthStackOV(uint32_t stack_id) // FIXME SimForth::StackId
  {
    m_stack_id = stack_id;

    switch (m_stack_id)
      {
      case DATA_STACK:
        m_error_msg = "Exception from SimTaDynForth: Data Stack overflow";
      case RETURN_STACK:
        m_error_msg = "Exception from SimTaDynForth: Return Stack overflow";
      default:
        m_error_msg = "Exception from SimTaDynForth: stack overflow";
      }
  }

  uint32_t m_stack_id;
};

// **************************************************************
//
// **************************************************************
class ForthUnknownPrimitive: public ForthException
{
public:
  ForthUnknownPrimitive(const Cell16 badToken, std::string const& funcName)
  {
    m_error_msg = "Exception from SimTaDynForth: try to execute an unknown primitive "
      + std::to_string((uint32_t) badToken) + " in " + funcName;
  }
};

// **************************************************************
//
// **************************************************************
class ForthUnknownWord: public ForthException
{
public:
  ForthUnknownWord(std::string const& word)
  {
    m_error_msg = "Exception from SimTaDynForth: unrecognized word '" + word + "'";
  }
};

// **************************************************************
//
// **************************************************************
class ForthReaderTruncatedFile: public ForthException
{
public:
  ForthReaderTruncatedFile(std::string const& filename)
  {
    m_error_msg = "Exception from SimTaDynForth: unfinished '" + filename + "'";
  }
};

#endif /* FORTH_EXCEPTIONS_HPP_ */
