#ifndef MAP_LOADER_HPP_
#  define MAP_LOADER_HPP_

#  include "Exception.hpp"
#  include "SimTaDynMap.hpp"

//! This macro (from the library POCO) will declare a class
//! MapLoaderException derived from simtadyn::Exception.
POCO_DECLARE_EXCEPTION(MapLoaderException, simtadyn::Exception)

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

  virtual bool load(std::string const& filename, SimTaDynMap* &map) = 0;

  //virtual bool save(std::string const& filename, SimTaDynMap const& map) = 0;

  // FIXME: faire onError()
  virtual const std::string& error()
  {
    return m_error;
  }

protected:

  std::string m_error;
};

#endif /* MAP_LOADER_HPP_ */
