//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#ifndef MAPEDITIONTOOLS_HPP
#  define MAPEDITIONTOOLS_HPP

// ***********************************************************************************************
//! \brief Command Pattern for map edition (insertion, move ...)
//! http://gameprogrammingpatterns.com/command.html
// ***********************************************************************************************
class MapEditionTools
{
public:

  MapEditionTools(MapEditor& map_editor)
    : m_map_editor(map_editor)
  {}

  virtual ~MapEditionTools() {}
  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) = 0;
  virtual void undo() {};

protected:

  MapEditor& m_map_editor;
};

class AddCellTool: public MapEditionTools
{
public:

  AddCellTool(MapEditor& map_editor)
    : MapEditionTools(map_editor)
  {}

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute1 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << " " << static_cast<int>(z) << std::endl;

#if 0
    switch (m_action_on)
    {
      case Node:
        m_map_editor.sheet().addNode(""); // FIXME: simplement m_map_editor.addNode(""); car m_map_editor appellera currentSheet()
        break;
      case Arc:
        if (state == FROM) { fromNode = m_map_editor.sheet().addNode(""); state = TO; }
        else {
            toNode = m_map_editor.sheet().addNode("");
            m_map_editor.sheet().addLink(fromNode, toNode);
            state = FROM;
        }
        break;
      case Zone:
        if (state == 0) { state = 1; vec.clear(); }
        node = m_map_editor.sheet().addNode("");
        if ((!vec.empty()) && (node == vec)) { // boucle
           state = 0;
           for (it: vec) { addLink(); }
           addZone(vec);
        } else {
           vec.push_back();
        }
        break;
    }
#endif
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute2 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << " " << static_cast<int>(z) << std::endl;
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    std::cout << "Ajoute3 une cellule a la position " << static_cast<int>(x) << " " << static_cast<int>(y) << " " << static_cast<int>(z) << std::endl;
  }
};

class RemoveCellTool: public MapEditionTools
{
public:

  RemoveCellTool(MapEditor& map_editor)
    : MapEditionTools(map_editor)
  {}

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Remove3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

class SelectCellTool: public MapEditionTools
{
public:

  SelectCellTool(MapEditor& map_editor)
    : MapEditionTools(map_editor)
  {}

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Select3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

class MoveCellTool: public MapEditionTools
{
public:

  MoveCellTool(MapEditor& map_editor)
    : MapEditionTools(map_editor)
  {}

  virtual void button1PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move1 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button2PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move2 une cellule a la position %f/%f/%f", x, y, z);
  }

  virtual void button3PressEvent(const gdouble x, const gdouble y, const gdouble z = 0.0f) override
  {
    LOGI("Move3 une cellule a la position %f/%f/%f", x, y, z);
  }
};

#endif
