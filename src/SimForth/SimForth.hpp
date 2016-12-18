#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "Forth.hpp"

class SimForthDico : public ForthDico
{
public:
  SimForthDico()
    : ForthDico()
  {
  }
};

class SimForth : public Forth
{
public:
  SimForth(ForthDico& dico)
    : Forth(dico)
  {
  }
};

#endif /* SIMFORTH_HPP_ */
