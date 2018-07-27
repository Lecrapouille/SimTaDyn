#ifndef GRAPHALGORITHM_HPP_
#  define GRAPHALGORITHM_HPP_

#  include "Graph.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class GraphAlgorithm
{
public:

  //! \brief Empty constuctor.
  GraphAlgorithm()
    : m_graph(nullptr)
  {
  }

  //! \brief Recursive Depth First Search algorithm
  void depthFirstSearch(BasicGraph_t& graph, const Key startNodeID);

private:

  //! \brief Auxillary method for the recursive Depth First Search
  //! algorithm.
  void depthFirstSearch_aux(BasicNode const& node);

  //! \brief the graph to perform algorithm.
  BasicGraph_t *m_graph;

public:

  //! \brief Store results here.
  std::vector<Key> m_result;
};

#endif /* GRAPHALGORITHM_HPP_ */
