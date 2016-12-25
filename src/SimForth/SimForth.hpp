#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "Forth.hpp"

class SimForthDictionary : public ForthDictionary
{
public:
  SimForthDictionary()
    : ForthDictionary()
  {
  }
};

class SimForth : public Forth
{
public:
  SimForth(ForthDictionary& dictionary)
    : Forth(dictionary)
  {
  }
};

#endif /* SIMFORTH_HPP_ */
