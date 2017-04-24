#include "BasicGraph.hpp"

// Useless node just used for init other nodes and arcs.
static BasicNode fakenode(0);

BasicNode::BasicNode()
  :  m_id(ClassCounter<BasicNode>::count())
{
  //LOGIS("New BasicNode empty constructor\n");
}

BasicArc::BasicArc()
  : m_id(ClassCounter<BasicArc>::count()),
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
