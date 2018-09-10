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

#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "SimTaDynSheet.hpp"
#  include "SceneGraph.tpp"
#  include <sigc++/sigc++.h>

using SceneNode_t = SceneNode<SimTaDynSheet, float, 3U>;

// ***********************************************************************************************
//! \brief
// ***********************************************************************************************
class SimTaDynMap;
using SimTaDynMapPtr = std::shared_ptr<SimTaDynMap>;

// *************************************************************************************************
//! \brief This class defines a SimTaDyn geographic map. It contains, nodes, arcs, Forth scripts,
//! OpenGL datum for the display. It implements the observable (aka listenable) design pattern:
//! When the map changes, it notifies to all observers which have subscribed to this map (for example
//! a MVC design pattern, where SimTaDynMap is the model, for the GUI).
// *************************************************************************************************
class SimTaDynMap
  : public Resource,
    private UniqueID<SimTaDynMap>
{
  friend class MapEditor;

public:

  //! \brief Empty constructor.
  SimTaDynMap()
    : m_name("Map_" + std::to_string(getID()))
  {
    LOGI("New SimTaDynMap with generic name '%s' and ID #%u\n", m_name.c_str(), getID());
  }

  //! \brief Constructor with the desired name for the map.
  SimTaDynMap(std::string const& name)
    : m_name(name)
  {
    LOGI("Creating SimTaDynMap named '%s' with ID #%u\n", m_name.c_str(), getID());
  }

  //! \brief Destructor.
  ~SimTaDynMap()
  {
    // FXME: retirer m_full_path du PathManager
    LOGI("Deleting SimTaDynMap #%u named '%s'\n", getID(), m_name.c_str());
  }

  const std::string &name() const
  {
    return m_name;
  }

  void clear()
  {
    // TODO
  }

  SimTaDynSheet* sheet()
  {
    if (nullptr == m_sheets.root())
      return nullptr;

    return m_sheets.root()->mesh();
  }

  inline bool modified() const
  {
    return (m_nb_graphs_modified > 0U) || (m_nb_scripts_modified > 0U);
  }

  void draw()
  {
    LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  getID(), m_name.c_str());

    //if (nullptr != m_sheets.root())
    //  draw(*(m_sheets.root()));
  }

private:

  /* FIXME
  void setUniform(const char *name, Matrix44f const &mat)
  {
    GLint id  = glCheck(glGetUniformLocation(m_shader, name));
    glCheck(glUniformMatrix4fv(id, 1, GL_FALSE, &mat[0U][0U]));
    }*/

  void draw(SceneNode_t &node)
  {
    LOGI("Renderer:drawNode '%s'", node.m_name.c_str());

    SimTaDynSheet *mesh = node.mesh();
    if (nullptr != mesh)
      {
        //Matrix44f transform = matrix::scale(node.worldTransform(), node.localScale());
        //setUniform("model", transform);
        mesh->draw(GL_POINTS);
      }

    std::vector<SceneNode_t*> const &children = node.children();
    for (auto i: children)
      {
        draw(*i);
      }
  }

public:

  //! \brief Give a name to the element which will be displayed in the
  //! GUI.  Contrary to id_ several cells can have the same name. By
  //! default the name is unique.
  std::string m_name;

  //! \brief the map structured as a graph.
  SceneGraph<SimTaDynSheet, float, 3U> m_sheets; // FIXME *m_sheets ???
  SimTaDynSheet m_sheet; //FIXME temporaire

  //! \brief List of Forth scripts.
  std::vector<std::string> m_scripts_forth;

  std::string               m_zip_path;
  std::string               m_base_dir;
  std::string               m_full_path;

  uint32_t m_nb_graphs_modified = 0U;
  uint32_t m_nb_scripts_modified = 0U;

  sigc::signal<void, SimTaDynMapPtr> signal_changed;
};

#endif /* SIMTADYN_MAP_HPP_ */
