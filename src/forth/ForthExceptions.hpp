#ifndef FORTHEXCEPTIONS_HPP_
#  define FORTHEXCEPTIONS_HPP_

#  include "ForthIncludes.hpp"

class ForthException: public std::exception
{
public:
  ForthException() throw () {}
  ~ForthException() throw () {}
  virtual const char* what () const throw ()
  {
    return "Exception from SimTaDynForth";
  }
};

class ForthDicoOOB: public ForthException
{
public:
  ForthDicoOOB(const Cell16* const ip, std::string const& funcName) throw ()
    : wrong_ip(ip),
      error_msg("Exception from SimTaDynForth: attempt to leave the dictionary bounds"
                + std::to_string((uint32_t) ip) + " in " + funcName)
  {
    func_name = funcName;
  }
  virtual const char* what () const throw ()
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
  virtual const char* what () const throw ()
  {
    return "Exception from SimTaDynForth: the dictionary has no more space";
  }
};

class ForthRStackOV: public ForthException
{
public:
  virtual const char* what () const throw ()
  {
    return "Exception from SimTaDynForth: return stack over flow";
  }
};

class ForthDStackOV: public ForthException
{
public:
  virtual const char* what () const throw ()
  {
    return "Exception from SimTaDynForth: data stack over flow";
  }
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
    : error_msg("Exception from SimTaDynForth: unrocgnize word " + word)
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

#endif /* FORTHEXCEPTIONS_HPP_ */
