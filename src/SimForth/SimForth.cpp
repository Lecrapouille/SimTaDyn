#include "SimForth.hpp"
#include "Config.hpp"

void SimForth::boot()
{
  // Start the Forth core
  Forth::boot();

  // Add specialized words for SimTaDyn

  // Initialize basic Forth system
  std::pair<bool, std::string> res;

  res = interpreteFile(Config::instance().data_path("forth/system.fs"));
  ok(res);
}
