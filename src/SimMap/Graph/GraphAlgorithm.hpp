#ifndef GRAPHALGORITHM_HPP_
#  define GRAPHALGORITHM_HPP_

#  include "SimTaDynGraph.hpp"

class GraphAlgorithm
{
public:

  void depthFirstSearch(SimTaDynGraph_t& graph, const Key startNodeID);

private:

  void depthFirstSearch_aux(SimTaDynGraph_t& graph, SimTaDynNode const& node);
};

#endif /* GRAPHALGORITHM_HPP_ */
