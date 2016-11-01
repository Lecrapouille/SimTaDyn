#ifndef FORTH_EXCEPTIONS_HPP_
#  define FORTH_EXCEPTIONS_HPP_

#  include "ForthHelper.hpp"

class ForthException: public std::exception
{
public:
  ForthException() throw () {}
  ~ForthException() throw () {}
  virtual const char* what () const throw ()
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

class ForthTooLongDef: public ForthException
{
public:
  ForthTooLongDef(std::string const& word)
  {
    m_word = word;
  }
  virtual const char* what() const throw ()
  {
    return "Exception from SimTaDynForth: word '" /*+ m_word +*/ "' contains more than 2^16 words.";
  }
  std::string m_word;
};

class ForthMalformedWord: public ForthException
{
public:
  ForthMalformedWord(std::string const& word)
  {
    m_word = word;
  }
  virtual const char* what() const throw ()
  {
    return "Exception from SimTaDynForth: word '" /*+ m_word +*/ "' is mal formed. It shall contain 1 ... 31 characters.";
  }
  std::string m_word;
};

class ForthDicoOOB: public ForthException
{
public:
  ForthDicoOOB(const Cell16* const ip, std::string const& funcName) throw ()
    : wrong_ip(ip),
      error_msg("Exception from SimTaDynForth: attempt to leave the dictionary bounds 0x"
                + convertPointerToStringAddress(ip) + " in " + funcName)
  {
    func_name = funcName;
  }
  virtual const char* what() const throw ()
  {
    return error_msg.c_str();
  }

  const Cell16* const wrong_ip;
  std::string func_name;
  std::string error_msg;
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
    : error_msg("Exception from SimTaDynForth: try to execute an unknown primitive "
                + std::to_string((uint32_t) bad_token) + " in " + func_name)
  {
    bad_token = badToken;
    func_name = funcName;
  }
  virtual const char* what () const throw ()
  {
    return error_msg.c_str();
  }

  Cell16 bad_token;
  std::string func_name;
  std::string error_msg;
};

class ForthUnknownWord: public ForthException
{
public:
  ForthUnknownWord(std::string const& word)
    : error_msg("Exception from SimTaDynForth: unrecognized word " + word)
  {
    unknown_word = word;
  }
  virtual const char* what () const throw ()
  {
    return error_msg.c_str();
  }
  std::string unknown_word;
  std::string error_msg;
};

#endif /* FORTH_EXCEPTIONS_HPP_ */
