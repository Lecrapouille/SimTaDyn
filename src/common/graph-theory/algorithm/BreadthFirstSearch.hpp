//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef GRAPHALGORITHM_BFS_HPP_
#  define GRAPHALGORITHM_BFS_HPP_

#  include "GraphAlgorithm.hpp"

namespace graphtheory
{

// *************************************************************************************************
//! \brief Recursive Breadth-first Search algorithm
// *************************************************************************************************
class GraphAlgorithmBFS: public GraphAlgorithm<Graph_t>
{
public:

  GraphAlgorithmBFS()
    : GraphAlgorithm()
  {
  }

  virtual ~GraphAlgorithmBFS()
  {
  }

  inline virtual void reset() override
  {
    // Nothing to do !
  }

  inline virtual bool finished() const override
  {
    return m_queue.empty();
  }

  inline virtual const GraphElement* update() override
  {
    const Node *node = static_cast<const Node*>(m_queue.front());
    m_queue.pop_front();

    // Set it as visited
    m_graph->markNode(node->id());

    // Put all its neighbours in the current queue
    for (size_t i = 0_z; i < node->degree(); ++i)
      {
        Node const& toNode = static_cast<const Arc*>(node->neighbor(i))->to();
        if (!m_graph->markedNode(toNode.id()))
          {
            m_queue.push_back(&toNode);
          }
      }

    return node;
  }

  virtual std::vector<const GraphElement*>& algorithm(Graph_t& graph, GraphElement& elt) override
  {
    if (GraphElementId::NODE != elt.type())
      {
        LOGE("The element %u is not a Node on the graph '%s'", elt.id(), graph.m_name.c_str());
        return m_result;
      }

    if (!graph.hasNode(elt.id()))
      {
        LOGE("The element %u does not exist on the graph '%s'", elt.id(), graph.m_name.c_str());
        return m_result;
      }

    init(graph, elt, true);
    if (m_saveResult)
      {
        while (!finished())
          {
            m_result.push_back(update());
          }
      }
    else
      {
        while (!finished())
          {
            update();
          }
      }
    graph.unmarkAllNodes();

    return m_result;
  }
};

} // namespace graphtheory

#endif /* GRAPHALGORITHM_BFS_HPP_ */
