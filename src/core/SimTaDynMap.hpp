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

#ifndef SIMTADYN_MAP_HPP_
#  define SIMTADYN_MAP_HPP_

#  include "SimTaDynSheet.hpp"
#  include "SceneGraph.tpp"
#  include "Types.hpp"
#  include <sigc++/sigc++.h>

// *************************************************************************************************
//! \brief This class defines a SimTaDyn geographic map. It contains, nodes, arcs, Forth scripts,
//! OpenGL datum for the display. It implements the observable (aka listenable) design pattern:
//! When the map changes, it notifies to all observers which have subscribed to this map (for example
//! a MVC design pattern, where SimTaDynMap is the model, for the GUI).
// *************************************************************************************************
class SimTaDynMap
  : private UniqueID<SimTaDynMap>
{
  friend class MapEditor;

public:

  //! \brief Empty constructor.
  SimTaDynMap()
  {
    LOGI("New SimTaDynMap #%u without name\n", getID());
  }

  //! \brief Constructor with the desired name for the map.
  SimTaDynMap(std::string const& name)
    : m_name(name)
  {
    LOGI("Creating SimTaDynMap #%u named '%s'\n", getID(), m_name.c_str());
  }

  //! \brief Destructor.
  ~SimTaDynMap()
  {
    // FXME: retirer m_full_path du PathManager
    LOGI("Deleting SimTaDynMap #%u named '%s'\n", getID(), m_name.c_str());
    m_sheets.reset();
  }

  //-----------------------------------------------------------------
  //! \brief Return the name. The name cannot be changed (TODO this is
  //! possible if we prevent the resource manager).
  //-----------------------------------------------------------------
  const std::string &name() const
  {
    return m_name;
  }

  //-----------------------------------------------------------------
  //! \brief Delete all the scene graph and create a new map.
  //! TODO prevent GTK to refresh map manager
  //-----------------------------------------------------------------
  void resetSheets()
  {
    // Make smart pointers delete everything.
    m_sheets.reset();
    m_current_sheet = nullptr;
  }

  //-----------------------------------------------------------------
  //! \brief Return the reference of the current sheet.
  //! \note a new sheet can be created.
  //-----------------------------------------------------------------
  SimTaDynSheet& currentSheet(bool const directed = false) // FIXME: is it a good stratgey ?
  {
    if (nullptr == m_current_sheet)
      createSheet(directed);
    return *m_current_sheet;
  }

  //-----------------------------------------------------------------
  //! \brief Return the pointer of the current sheet or nullptr
  //-----------------------------------------------------------------
  inline SimTaDynSheetPtr currentSheetPtr()
  {
    return m_current_sheet;
  }

  //-----------------------------------------------------------------
  //! \brief Return the pointer of the sheet by its name or nullptr
  //-----------------------------------------------------------------
  inline SimTaDynSheetPtr sheet(std::string const& name)
  {
    return m_sheets.findRenderable(name);
  }

  //-----------------------------------------------------------------
  //! \brief Return the root sheet which may not exists or does not
  //! have a sheet.
  //-----------------------------------------------------------------
  SceneNodePtr rootSceneGraph()
  {
    return m_sheets.root();
  }

  //-----------------------------------------------------------------
  //! \brief Create a new sheet. Store it in the scene graph.
  //-----------------------------------------------------------------
  SimTaDynSheetPtr createSheet(bool const directed)
  {
    m_current_sheet = std::make_shared<SimTaDynSheet>(directed);
    SceneNodePtr node = m_sheets.attach(m_current_sheet, m_current_sheet->name());

    // TODO: ceci est un exemple
    node->localScale(Vector3f(1.0f));
    node->position(Vector3f(0.0f, 0.0f, 0.0f));

    return m_current_sheet;
  }

  //-----------------------------------------------------------------
  //! \brief Create a new sheet. Store it in the scene graph.
  //-----------------------------------------------------------------
  SimTaDynSheetPtr createSheet(std::string const& name, bool const directed = true)
  {
    m_current_sheet = std::make_shared<SimTaDynSheet>(name, directed);
    SceneNodePtr node = m_sheets.attach(m_current_sheet, name);

    // TODO: ceci est un exemple
    node->localScale(Vector3f(1.0f));
    node->position(Vector3f(0.0f, 0.0f, 0.0f));

    return m_current_sheet;
  }

  //-----------------------------------------------------------------
  //! \brief Return if the map has been modified. FIXME: this is a
  //! fake code
  //-----------------------------------------------------------------
  inline bool modified() const
  {
    LOGE("SimTaDynMap::modified() not yet implemented");
    return (m_nb_graphs_modified > 0U) || (m_nb_scripts_modified > 0U);
  }

  //-----------------------------------------------------------------
  //! \brief Draw with OpenGL the map and all its sheets.
  //-----------------------------------------------------------------
  void drawnBy(ISceneGraphRenderer<SimTaDynSheet, float, 3u>& renderer)
  {
    LOGI("SimTaDynMap.drawnBy 0x%x", this);
    LOGI("SimTaDynMap #%u %s drawnBy renderer",  getID(), m_name.c_str());

    float dt = 0.0f; // TODO: for animation
    m_sheets.update(dt);
    m_sheets.drawnBy(renderer);
  }

public:

  //! \brief Give a name to the element which will be displayed in the
  //! GUI.  Contrary to id_ several cells can have the same name. By
  //! default the name is unique.
  std::string m_name;

  //! \brief Some information about the map
  std::string about;

  //! \brief the map structured as a graph.
  SceneGraph m_sheets;

  //TODO SceneNode<SimTaDynSheet, float, 3U> m_current_node = nullptr;
  //TODO m_current_sheet =  m_current_node->mesh()
  //TODO m_current_node = gtkmm::TreeView::on_click()
  SimTaDynSheetPtr m_current_sheet = nullptr;

  //! \brief List of Forth scripts.
  std::vector<std::string> m_scripts_forth;

  std::string m_zip_path;
  std::string m_base_dir;
  std::string m_full_path;

  uint32_t m_nb_graphs_modified = 0U;
  uint32_t m_nb_scripts_modified = 0U;

  //sigc::signal<void, SimTaDynMapPtr> signal_map_changed;
};

#endif /* SIMTADYN_MAP_HPP_ */
