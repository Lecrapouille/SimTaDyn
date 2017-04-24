#include "GraphAlgorithm.hpp"

void GraphAlgorithm::depthFirstSearch_aux(SimTaDynGraph_t& graph, SimTaDynNode const& node)
{
  graph.mark(node.id());
  for (uint32_t i = 0; i < node.degree(); ++i)
    {
      BasicNode& toNode = node.neighbor(i)->to();
      if (false == graph.alreadyMarked(toNode.id()))
        {
          depthFirstSearch_aux(graph, toNode);
        }
    }
}

void GraphAlgorithm::depthFirstSearch(SimTaDynGraph_t& graph, const Key startNodeID)
{
  LOGI("GraphAlgorithm::depthFirstSearch on graph '%s' starting on node %u",
       graph.m_name, startNodeID);

  if (!graph.hasNode(startNodeID))
    {
      LOGE("The node %u does not exist on the graph '%s'", startNodeID, graph.m_name);
      return ;
    }

  BasicNode& node = graph.getNode(startNodeID);
  graph.unmarkAll();
  depthFirstSearch_aux(graph, node);
  graph.unmarkAll();
}
