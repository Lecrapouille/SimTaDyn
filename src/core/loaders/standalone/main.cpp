#include "SimTaDynLoader.hpp"

int main(void)
{
  SimTaDynGraph graph("Graph1");
  ShapefileLoader loader;

  loader.load("~/.SimTaDyn/data/3dpoints.shp", graph);
  return 0;
}
