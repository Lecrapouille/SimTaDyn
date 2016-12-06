#include "ShapeFile.hpp"

class SimTaDynLoaderManager
{
public:
  bool load(const std::string& filename, SimTaDynGraph& graph)
  {
    bool res = false;
    std::string extension;

    extension = filename.substr(filename.find_last_of(".") + 1);
    if (0 == extension.compare("shp"))
      {
        ShapefileLoader loader;
        res = loader.load(filename, graph);
      }
    // else if ... other extensions
    return res;
  }
};
