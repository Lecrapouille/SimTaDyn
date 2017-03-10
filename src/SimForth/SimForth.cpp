#include "SimForth.hpp"
#include "Config.hpp"

void SimForth::boot()
{
  LOGI("Booting SimForth");
  // Start the Forth core
  Forth::boot();

  // Add specialized words for SimTaDyn

  // Initialize basic Forth system
  std::pair<bool, std::string> res;

  LOGI("Loading minimal SimForth");
  res = interpreteFile(Config::instance().data_path("forth/system.fs"));
  ok(res);
}
