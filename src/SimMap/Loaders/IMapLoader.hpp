#ifndef IMAP_LOADER_HPP_
#  define IMAP_LOADER_HPP_

#  include "SimTaDynGraphs.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class SimTaDynMapLoaderException // TBD: : public Exception
{
};

// *************************************************************************************************
// Abstract class
// *************************************************************************************************
class SimTaDynMapLoader
{
public:
  virtual bool load(const std::string& filename, SimTaDynGraph& graph) = 0;
};

#endif /* IMAP_LOADER_HPP_ */
