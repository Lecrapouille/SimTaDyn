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

#ifndef SPREADSHEETCELL_HPP_
#  define SPREADSHEETCELL_HPP_

#  include "ASpreadSheetCell.hpp"
#  include "SimTaDynGraph.hpp"

using namespace graphtheory;
class SimForth;

// **************************************************************
//! \brief Specialize a graph node with a spreadsheet cell.
// **************************************************************
class CellNode: public Node, public ASpreadSheetCell
{
public:

  CellNode(std::string const& formulae = "")
    : Node(), ASpreadSheetCell(formulae)
  {
    init();
    //std::cout << "New CellNode " << id() << ": " << formulae << std::endl;
  }

  CellNode(const Key nodeID, std::string const& formulae = "")
    : Node(nodeID), ASpreadSheetCell(formulae)
  {
    init();
  }

  CellNode(Node const& node, std::string const& formulae = "")
    : Node(node), ASpreadSheetCell(formulae)
  {
    init();
  }

  inline virtual Key id() const override
  {
    return Node::id();
  }

  inline virtual const std::string name() const override
  {
    return Node::name();
  }

private:

  inline void init()
  {
    m_dataKey = id();
  }
};

// **************************************************************
//! \brief Specialize a graph arc with a spreadsheet cell.
// **************************************************************
class CellArc: public ExtArc, public ASpreadSheetCell
{
public:

  CellArc(std::string const& formulae = "")
    : ExtArc(), ASpreadSheetCell(formulae)
  {
    init();
  }

  CellArc(const Key id, Node& fromNode, Node& toNode,
          std::string const& formulae = "")
    : ExtArc(id, fromNode, toNode), ASpreadSheetCell(formulae)
  {
    init();
  }

  CellArc(ExtArc const& arc, std::string const& formulae = "")
    : ExtArc(arc), ASpreadSheetCell(formulae)
  {
    init();
  }

  inline virtual Key id() const override
  {
    return ExtArc::id();
  }

  inline virtual const std::string name() const override
  {
    return ExtArc::name();
  }

private:

  inline void init()
  {
    m_dataKey = ExtArc::id();
  }
};

// **************************************************************
//
// **************************************************************
class CellZone: public Zone, public ASpreadSheetCell
{
public:

  CellZone(std::string const& formulae = "")
    : Zone(), ASpreadSheetCell(formulae)
  {
    init();
  }

  CellZone(const Key id,
           std::initializer_list<Arc*> arcs,
           std::string const& formulae = "")
    : Zone(id, arcs), ASpreadSheetCell(formulae)
  {
    init();
  }

  CellZone(Zone const& zone, std::string const& formulae = "")
    : Zone(zone), ASpreadSheetCell(formulae)
  {
    init();
  }

  inline virtual Key id() const override
  {
    return Zone::id();
  }

  inline virtual const std::string name() const override
  {
    return Zone::name();
  }

private:

  inline void init()
  {
    m_dataKey = Zone::id();
  }
};

#endif /* SPREADSHEETCELL_HPP_ */
