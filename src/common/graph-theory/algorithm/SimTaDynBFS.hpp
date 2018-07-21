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

#ifndef GRAPHALGORITHM_SIMTADYN_BFS_HPP_
#  define GRAPHALGORITHM_SIMTADYN_BFS_HPP_

//#  include "SimTaDynCell.hpp"
//#  include "SimTaDynGraph.hpp"
#  include "GraphAlgorithm.hpp"

namespace graphtheory
{

// *************************************************************************************************
//! \brief Recursive Breadth-first Search algorithm
// *************************************************************************************************
  class GraphAlgorithmSimTaDynBFS: public GraphAlgorithm<SimTaDynGraph<CellNode, CellArc, CellZone>>
{
public:

  GraphAlgorithmSimTaDynBFS()
    : GraphAlgorithm()
  {
  }

  virtual ~GraphAlgorithmSimTaDynBFS()
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

  // FIXME: fonctionne (itere sur les voisins) mais pas sur l'ensemble des elemnts du graphe
  inline virtual const GraphElement* update() override
  {
    const GraphElement *elt = m_queue.front();
    m_queue.pop_front();

    // Set it as visited
    m_graph->markNode(elt->id());

    // Put all its neighbours in the current queue
    for (uint32_t i = 0; i < elt->howManyNeighbors(); ++i)
      {
        GraphElement const& toElt = *(elt->neighbor(i));
        if (!m_graph->markedNode(toElt.id()))
          {
            m_queue.push_back(&toElt);
          }
      }

    return elt;
  }

  virtual std::vector<const GraphElement*>& algorithm(SimTaDynGraph<CellNode, CellArc, CellZone>& graph, GraphElement& elt) override
  {
    bool found = false;

    if (GraphElementId::NODE == elt.type())
      found = graph.hasNode(elt.id());
    else if (GraphElementId::ARC == elt.type())
      found = graph.hasArc(elt.id());

    if (!found)
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

#endif /* GRAPHALGORITHM_SIMTADYN_BFS_HPP_ */
