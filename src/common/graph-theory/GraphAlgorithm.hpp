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
