#ifndef MAP_LOADER_HPP_
#  define MAP_LOADER_HPP_

#  include "SimTaDynMap.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class MapLoaderException // TBD: : public Exception
{
};

// *************************************************************************************************
// Abstract class
// *************************************************************************************************
class MapLoader
{
public:
  MapLoader()
    : m_error("no error")
  {
  }

  virtual bool load(std::string const& filename, SimTaDynMap *map) = 0;
  virtual const std::string error()
  {
    return m_error;
  }

protected:
  std::string m_error;
};

#endif /* MAP_LOADER_HPP_ */
