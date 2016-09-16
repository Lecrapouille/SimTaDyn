#ifndef CONTEXT_HPP_
#  define CONTEXT_HPP_

#  include "gui/SimTaDynWindow.hpp"
#  include "RTree.hpp"
#  include "Forth.hpp"
#  include "ShapeFile.hpp"
#  include <algorithm> // swap

// With singleton pattern (not thread safe)
class SimTaDynContext
{
public:
  static SimTaDynContext& getInstance()
  {
    return singleton;
  }

  //SimTaDynWindow main_window;
  // renommer ca en map
  Forth forth;
  RTreeNode* root; // a attacher dans le graph
  SimTaDynGraph graph;
  ShapefileLoader loader;

private:

  SimTaDynContext& operator=(const SimTaDynContext&)
  {
    return *this;
  }

  SimTaDynContext(const SimTaDynContext&)
  {
  }

  SimTaDynContext()
    : root(NULL)
  {
  }

  ~SimTaDynContext()
  {
  }

  static SimTaDynContext singleton;
};

#endif /* CONTEXT_HPP_ */
