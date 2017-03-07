#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "Forth.hpp"
#  include "Singleton.hpp"
//#  include <list> of dictionaries ?

class SimForthDictionary : public ForthDictionary
{
public:
  SimForthDictionary()
    : ForthDictionary()
  {
  }
};

class SimForth : public Forth, public Singleton<SimForth>
{
  friend class Singleton<SimForth>;

public:

  virtual void boot() override;

  SimForthDictionary m_dictionaries;
  NoColor m_color;

private:

  SimForth()
    : Forth(m_dictionaries, m_color)
  {
  }
};

#endif /* SIMFORTH_HPP_ */
