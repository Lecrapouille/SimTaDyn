#include "SimTaDynMap.hpp"

SimTaDynNode *SimTaDynMap::addNode(Vector3D const& p)
{
  SimTaDynNode *node = m_graph.addNode();
  if (nullptr != node)
    {
      node->dataID = m_positions.occupation();
      m_positions.append(p);
      //m_colors.append();
    }
  return node;
}

bool SimTaDynMap::removeNode(const Key nodeID)
{
  SimTaDynNode *to_remove = m_graph.getNode(nodeID);
#warning "fixme 01"
  SimTaDynNode *the_last = nullptr; // TODO m_graph.getLastNode();

  if ((nullptr == to_remove) || (nullptr == the_last))
    return false;

  m_positions.swap(to_remove->dataID, the_last->dataID);

  the_last->dataID = to_remove->dataID;
  //m_graph.removeNode(nodeID);
#warning "fixme 02"
  return true;
}
