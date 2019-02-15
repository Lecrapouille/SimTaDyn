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

#ifndef MVP_HPP
#define MVP_HPP

#include "DrawingArea.hpp"
#include "SimTaDynMap.hpp"

class MapPresenter
{
public:

  MapPresenter(PopupException& popup_exception, SimTaDynMapPtr map = nullptr)
    : m_glarea(popup_exception),
      m_map(map)
  {}

  void model(SimTaDynMapPtr map)
  {
    m_map = map;
  }

  inline GLDrawingArea& view()
  {
    return m_glarea;
  }

  inline SimTaDynMapPtr modelPtr()
  {
    return m_map;
  }

  inline SimTaDynMap& model()
  {
    return *m_map;
  }

private:

  // Thread safe si glarea a ses propres donnees a dessiner
  // (genre un gros VBO (2 pour faire un swap VBO)
  // independant du model (on a parcouru le scenegraph et on a
  // appliquer le prod matricielle, le resultat est sauve dans un VBO)
  // on le fait uniquement quand le scene graph a change.
  void drawMap()
  {
    //LOGI("Repainting map '%s'", m_map.name().c_str());
    //map.drawnBy(m_glarea);
  }

private:

  GLDrawingArea  m_glarea;
  SimTaDynMapPtr m_map;
};

#endif
