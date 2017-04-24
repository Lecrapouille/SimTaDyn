#include "SimTaDynMap.hpp"

// FIXME: temporaire
/*static Set<Vector3D> P;
  static Set<Color> C;*/
const Vector3D* vec;

/*SimTaDynNode& SimTaDynMap::addNode(Vertex const& p)
{
  SimTaDynNode& node = m_graph.addNode();

  m_vertices.append(p);
  //m_colors.append();
  //assert(m_vertices.index() == m_colors.index());

  node.dataID = m_vertices.index();
  return node;
}

bool SimTaDynMap::removeNode(const Key nodeID)
{
  SimTaDynNode *to_remove = m_graph.getNode(nodeID);
#warning "fixme 01"
  SimTaDynNode *the_last = nullptr; // TODO m_graph.getLastNode();

  if ((nullptr == to_remove) || (nullptr == the_last))
    return false;

  m_vertices.swap(to_remove->dataID, the_last->dataID);

  the_last->dataID = to_remove->dataID;
  //m_graph.removeNode(nodeID);
#warning "fixme 02"
return true;
}*/
