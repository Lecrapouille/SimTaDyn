#include "GraphAlgorithm.hpp"

void GraphAlgorithm::depthFirstSearch_aux(BasicNode const& node)
{
  m_graph->markNode(node.id());
  m_result.push_back(node.id());

  for (uint32_t i = 0; i < node.degree(); ++i)
    {
      BasicNode const& toNode = node.neighbor(i)->to();

      // FIXME: side-effect on graph non directed: le noeud se teste lui-meme car il tente de revenir vers lui meme
      if (!m_graph->markedNode(toNode.id()))
        {
          depthFirstSearch_aux(toNode);
        }
    }
}

void GraphAlgorithm::depthFirstSearch(BasicGraph_t& graph, const Key startNodeID)
{
  LOGI("GraphAlgorithm::depthFirstSearch on graph '%s' starting on node %u",
       graph.m_name.c_str(), startNodeID);

  if (!graph.hasNode(startNodeID))
    {
      LOGE("The node %u does not exist on the graph '%s'", startNodeID, graph.m_name);
      return ;
    }

  // Reset states and results
  graph.unmarkAllNodes();
  m_result.clear();
  m_result.reserve(std::max(0U, graph.howManyNodes() - m_result.capacity()));
  m_graph = &graph;

  // Do the algorithm
  BasicNode& node = graph.getNode(startNodeID);
  depthFirstSearch_aux(node);

  graph.unmarkAllNodes();
}
