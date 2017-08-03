#include "Graph.hpp"

// Useless node just used for init other nodes and arcs.
static BasicNode fakenode(0);

BasicNode::BasicNode()
  :  m_id(UniqueID<BasicNode>::getID())
{
  //LOGIS("New BasicNode empty constructor\n");
}

BasicArc::BasicArc()
  : m_id(UniqueID<BasicArc>::getID()),
    m_fromNode(&fakenode),
    m_toNode(&fakenode)
{
}

void BasicArc::from(BasicNode& fromNode)
{
  m_fromNode = &fromNode;
}

void BasicArc::to(BasicNode& toNode)
{
  m_toNode = &toNode;
}
