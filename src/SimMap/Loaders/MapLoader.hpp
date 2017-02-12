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
  virtual SimTaDynMap *load(const std::string& filename, SimTaDynMap *map = nullptr) = 0;
  virtual const std::string &error()
  {
    return m_error;
  }

protected:
  std::string   m_error = c_no_error;

private:
  const char   *c_no_error = "no error";
};

#endif /* MAP_LOADER_HPP_ */
