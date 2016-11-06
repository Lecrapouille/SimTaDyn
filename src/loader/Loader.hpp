#include "SimTaDynGraphs.hpp"

class SimTaDynLoaderException
{
};

class SimTaDynLoader
{
public:
  virtual bool load(const std::string& filename, SimTaDynGraph& graph) = 0;
};
