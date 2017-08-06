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
      LOGE("The node %u does not exist on the graph '%s'", startNodeID, graph.m_name.c_str());
      return ;
    }

  // Reset states and results
  graph.unmarkAllNodes();
  m_result.clear();
  uint32_t reserving = graph.howManyNodes() - m_result.capacity();
  m_result.reserve(std::max(0U, reserving));
  m_graph = &graph;

  // Do the algorithm
  BasicNode& node = graph.getNode(startNodeID);
  depthFirstSearch_aux(node);

  graph.unmarkAllNodes();
}
