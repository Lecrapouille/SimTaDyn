//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef GRAPH_ALGORITHM_HPP_
#  define GRAPH_ALGORITHM_HPP_

#  include "Graph.hpp"
#  include <deque>
#  include <memory>

namespace graphtheory
{

// *************************************************************************************************
//! \brief Define an abstract class for traversing a graph.
//! This class can be used in two manners:
//!   -- Either perform all the algorithm and return visited nodes.
//!   -- Or can be called by step to be included by an outer algorithm.
// *************************************************************************************************
  template <class G>
  class GraphAlgorithm
{
public:

  GraphAlgorithm()
    : m_graph(nullptr), m_saveResult(false)
  {
  }

  virtual ~GraphAlgorithm()
  {
  }

  //! \brief Allocate a specialized graph algorithm depending on the name
  //! passed as parameter.
  static std::shared_ptr<GraphAlgorithm<G>> factory(const std::string name);

  //! \brief Check if the algorithm has ended or has still elements to compute.
  //! This method is to be used by an outer algorithm.
  virtual bool finished() const = 0;

  //! \brief Perform a single step in the algorithm.
  //! This method is to be used by an outer algorithm.
  virtual const GraphElement* update() = 0;

  //! \brief Call the whole algorithm in a single call.
  virtual std::vector<const GraphElement*>& algorithm(G& graph, GraphElement& elt) = 0;

  //! \brief init the algorithm. This method reset internal states and call the derived
  //! class reset method.
  virtual void init(G& graph, GraphElement& elt, const bool saveResult)
  {
    // Mark as non visited nodes.
    m_graph = &graph;
    m_graph->unmarkAllNodes();

    // Empty the result and reserve the space for storing nodes
    m_result.clear();
    int32_t reserving = m_graph->howManyNodes() - m_result.capacity();
    m_result.reserve(std::max(0, reserving));

    // Empty the queue ...
    m_queue.clear();

    // Derived class reset
    reset();

    // ... and insert the starting node
    m_queue.push_back(&elt);

    //
    m_saveResult = saveResult;
  }

protected:

  //! \brief Allow to derived class to reset their states
  virtual void reset()
  {
  }

  //! \brief the graph to perform algorithm.
  G *m_graph;

  //! \brief store nodes
  std::deque<const GraphElement*> m_queue;

  //! \brief Store results here.
  std::vector<const GraphElement*> m_result;

  //! \brief If set then store visited nodes in m_result
  bool m_saveResult;
};

} // namespace graphtheory

// **************************************************************
// Include here all algorithms
// **************************************************************
#include "algorithm/BreadthFirstSearch.hpp"
//#include "algorithm/SimTaDynBFS.hpp"

// **************************************************************
//
// **************************************************************

namespace graphtheory
{

    template <class G>
    inline std::shared_ptr<GraphAlgorithm<G>> GraphAlgorithm<G>::factory(const std::string name)
    {
      if (0 == name.compare("BFS"))
        return std::make_shared<GraphAlgorithmBFS>();
      //FIXME else if (0 == name.compare("SimBFS"))
      //  return std::make_shared<GraphAlgorithmSimTaDynBFS>();

      LOGE("Unkown Graph theory traversal named '%s'", name.c_str());
      return nullptr;
    }

} // namespace graphtheory

#endif /* GRAPH_ALGORITHM_HPP_ */
