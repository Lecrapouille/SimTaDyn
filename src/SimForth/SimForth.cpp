#include "SimForth.hpp"
#include "Config.hpp"

void SimForth::boot()
{
  LOGI("Booting SimForth");

  // Start the Forth core
  Forth::boot();

  // Add specialized words for SimTaDyn
  m_dictionary.add(SIMFORTH_PRIMITIVE_TOTO, FORTH_DICO_ENTRY("TOTO"), 0);

  // Initialize basic Forth system
  std::pair<bool, std::string> res;

  res = interpreteFile(Config::instance().data_path("forth/system.fs"));
  ok(res);
}
