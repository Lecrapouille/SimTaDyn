#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

#  include "ForthHelper.hpp"

class ForthException: public std::exception
{
public:
  ForthException() throw () {}
  ~ForthException() throw () {}
  virtual const char* what() const throw ()
  {
    return "Exception from SimTaDynForth";
  }
  std::string convertPointerToStringAddress(const Cell16* const ad)
  {
    const void* address = static_cast<const void*>(ad);
    std::stringstream stream;

    stream << address;
    return stream.str();
  }
};

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
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_error_msg;
};

class ForthUnbalancedDef: public ForthException
{
public:
  ForthUnbalancedDef(std::string const& word)
  {
    m_word = word;
    m_error_msg = "Stack depth changed during the definition of the word '" + m_word + "' probably unbalanced condition";
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_word;
  std::string m_error_msg;
};

class ForthTooLongDef: public ForthException
{
public:
  ForthTooLongDef(std::string const& word)
  {
    m_word = word;
    m_error_msg = "Exception from SimTaDynForth: word '" + m_word + "' contains more than 2^16 words.";
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_word;
  std::string m_error_msg;
};

class ForthMalformedWord: public ForthException
{
public:
  ForthMalformedWord(std::string const& word)
  {
    m_word = word;
    m_error_msg = "Exception from SimTaDynForth: word '" + m_word + "' is mal formed. It shall contain 1 ... 31 characters.";
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_word;
  std::string m_error_msg;
};

class ForthDicoOOB: public ForthException
{
public:
  ForthDicoOOB(const Cell16* const ip) throw ()
    : m_wrong_addr(0),
      m_wrong_ip(ip),
      m_error_msg("Exception from SimTaDynForth: IP tried to access an invalid address: "
                  + convertPointerToStringAddress(ip))
  {
  }
  ForthDicoOOB(const uint32_t addr) throw ()
    : m_wrong_addr(addr),
      m_wrong_ip(nullptr),
      m_error_msg("Exception from SimTaDynForth: " + std::to_string(addr) + "is an invalid dictionary address ")
  {
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }

  const uint32_t m_wrong_addr;
  const Cell16* const m_wrong_ip;
  std::string m_error_msg;
};

class ForthDicoNoSpace: public ForthException
{
public:
  virtual const char* what() const throw ()
  {
    return "Exception from SimTaDynForth: the dictionary has no more space";
  }
};

class ForthStackOV: public ForthException
{
public:
  ForthStackOV(uint32_t stack_id) // FIXME SimForth::StackId
  {
    m_stack_id = stack_id;
  }
  virtual const char* what() const throw ()
  {
    switch (m_stack_id)
    {
    case DATA_STACK:
      return "Exception from SimTaDynForth: Data Stack overflow";
    case RETURN_STACK:
      return "Exception from SimTaDynForth: Return Stack overflow";
    default:
      return "Exception from SimTaDynForth: stack overflow";
    }
  }
  uint32_t m_stack_id;
};

class ForthUnknownPrimitive: public ForthException
{
public:
  ForthUnknownPrimitive(const Cell16 badToken, std::string const& funcName)
    : m_error_msg("Exception from SimTaDynForth: try to execute an unknown primitive "
                  + std::to_string((uint32_t) badToken) + " in " + funcName)
  {
    m_bad_token = badToken;
    m_func_name = funcName;
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }

  Cell16 m_bad_token;
  std::string m_func_name;
  std::string m_error_msg;
};

class ForthUnknownWord: public ForthException
{
public:
  ForthUnknownWord(std::string const& word)
  {
    m_unknown_word = word;
    m_error_msg = "Exception from SimTaDynForth: unrecognized word '" + word + "'";
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_unknown_word;
  std::string m_error_msg;
};

class ForthReaderTruncatedFile: public ForthException
{
public:
  ForthReaderTruncatedFile(std::string const& filename)
  {
    m_filename = filename;
    m_error_msg = "Exception from SimTaDynForth: unfinished '" + filename + "'";
  }
  virtual const char* what() const throw ()
  {
    return m_error_msg.c_str();
  }
  std::string m_filename;
  std::string m_error_msg;
};

#endif /* FORTH_EXCEPTIONS_HPP_ */
