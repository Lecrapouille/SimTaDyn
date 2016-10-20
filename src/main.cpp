#include "SimTaDynContext.hpp"

SimTaDynContext SimTaDynContext::singleton = SimTaDynContext();

int main(int argc, char** argv)
{
  SimTaDynContext& simtadyn = SimTaDynContext::getInstance();

  const Gtk::Main kit(argc, argv);
  Gtk::GL::init(argc, argv);

  // Start the Forth core
  simtadyn.forth.boot();
  // Load an initial map
  simtadyn.loader.loadShapefile("../data/Corsica-points.shp", simtadyn.graph);
  //simtadyn.loader.loadShapefile("../data/3dpoints.shp", simtadyn.graph);
  //std::cout << simtadyn.graph.name << std::endl;

  SimTaDynWindow main_window("SimTaDyn");
  kit.run(main_window);

  return 0;
}
