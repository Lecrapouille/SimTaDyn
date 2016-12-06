#ifndef CONTEXT_HPP_
#  define CONTEXT_HPP_

#  include "SimTaDynWindow.hpp"
#  include "RTree.hpp"
#  include "Forth.hpp"
#  include "SimTaDynLoader.hpp"

#  define SIMTADYN() SimTaDynContext::getInstance()

// Singleton pattern (not thread safe)
class SimTaDynContext
{
public:

  SimTaDynWindow* m_window;
  SimTaDynLoaderManager m_loader;
  Forth m_forth;
  RTreeNode* m_root; // FIXME: a attacher dans le graph
  SimTaDynGraph m_graph; // FIXME: a renommer en maps

public:

  static SimTaDynContext& getInstance()
  {
    return s_SimTaDyn;
  }

private:

  SimTaDynContext& operator=(const SimTaDynContext&)
  {
    return *this;
  }

  SimTaDynContext(const SimTaDynContext&)
    : m_root(NULL) // FIXME: a virer
  {
  }

  SimTaDynContext()
    : m_root(NULL) // FIXME: a virer
  {
    // Start the Forth core
    s_SimTaDyn.m_forth.boot();
    s_SimTaDyn.m_forth.eatFile("forth/core/system.fs");
    //s_SimTaDyn.m_forth.eatString(": FOO + . ;");
    //s_SimTaDyn.m_forth.dictionary().display();

    // Load an initial map
    //s_SimTaDyn.m_loader.load("../data/Corsica-points.shp", s_SimTaDyn.m_graph);
  }

  ~SimTaDynContext()
  {
  }

  // Singleton
  static SimTaDynContext s_SimTaDyn;
};

#endif /* CONTEXT_HPP_ */
