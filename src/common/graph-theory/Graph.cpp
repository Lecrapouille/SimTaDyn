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

#include "Graph.hpp"

// Useless node just used for init other nodes and arcs.
static BasicNode fakenode(0);

BasicNode::BasicNode()
  :  m_id(UniqueID<BasicNode>::getID())
{
  //LOGIS("New BasicNode empty constructor\n");
}

BasicArc::BasicArc()
  : m_id(UniqueID<BasicArc>::getID()),
    m_fromNode(&fakenode),
    m_toNode(&fakenode)
{
}

void BasicArc::from(BasicNode& fromNode)
{
  m_fromNode = &fromNode;
}

void BasicArc::to(BasicNode& toNode)
{
  m_toNode = &toNode;
}
