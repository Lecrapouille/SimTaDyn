#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "Forth.hpp"
#  include "SimForthPrimitives.hpp"
#  include "Singleton.hpp"
//#  include <list> of dictionaries ?

class SimForthDictionary : public ForthDictionary
{
public:
  SimForthDictionary()
    : ForthDictionary()
  {
    LOGI("Creating SimForthDictionary");
  }
};

class SimForth : public Forth, public Singleton<SimForth>
{
  friend class Singleton<SimForth>;

public:

  virtual void boot() override;

  SimForthDictionary m_dictionaries;
  NoColor m_color;

protected:

  virtual inline uint32_t maxPrimitives() const
  {
    return SIMFORTH_MAX_PRIMITIVES;
  }

  virtual void execPrimitive(const Cell16 idPrimitive) override
  {
    switch (idPrimitive)
      {
      case SIMFORTH_PRIMITIVE_TOTO:
        std::cout << "TOTOTOTOTO\n";
        break;
      default:
        Forth::execPrimitive(idPrimitive);
        break;
      }
  }

private:

  SimForth()
    : Forth(m_dictionaries, m_color)
  {
    LOGI("Creating SimForth");
  }

  virtual ~SimForth()
  {
    LOGI("Destroying SimForth");
  }
};

#endif /* SIMFORTH_HPP_ */
