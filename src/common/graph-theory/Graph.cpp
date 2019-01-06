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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "Graph.hpp"

namespace graphtheory
{

  // Useless node just used for init other nodes and arcs.
  static Node fakeNode(0);

  Node::Node()
    : GraphElementWithNeighbors(UniqueID<Node>::getID())
  {
  }

  Arc::Arc()
    : GraphElement(UniqueID<Arc>::getID())
  {
    m_borders = {&fakeNode, &fakeNode};
  }

} // namespace graphtheory
