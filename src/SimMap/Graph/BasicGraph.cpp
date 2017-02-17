#include "BasicGraph.hpp"

static void depthFirstSearch_aux(BasicGraph<BasicNode, BasicArc> *const graph, BasicNode *node)
{
  std::cout << (char) node->id() << std::endl;
  graph->markVisitedNode(node->id());
  for (uint32_t i = 0; i < node->degree(); ++i)
    {
      BasicNode &toNode = node->neighbor(i)->to();
      if (!graph->nodeHasBeenVisited(toNode.id()))
        {
          depthFirstSearch_aux(graph, &toNode);
        }
    }
}

void depthFirstSearch(BasicGraph<BasicNode, BasicArc> *const graph, const Key satrtNodeID)
{
  BasicNode *node = graph->getNode(satrtNodeID);
  if (nullptr == node)
    return ;

  graph->unmarkAllVisitedNodes();
  depthFirstSearch_aux(graph, node);
  graph->unmarkAllVisitedNodes();
}
