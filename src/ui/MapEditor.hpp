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

#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include "Names.hpp"
#  include "SimTaDynLoaders.hpp"
#  include "ToggleButtons.hpp"
#  include "ForthInspector.hpp"
#  include "DrawingArea.hpp"
#  include "SimTaDynMapExplorer.hpp"

class LoaderManager;
class MapEditionTools;
class GLDrawingArea;

// *************************************************************************************************
//! \brief A class holding the currently edited by SimTaDynMap. When
//! the user changes of map, this class will notifies to observers that
//! map changed.
// ***********************************************************************************************
class SimTaDynMapHolder
{
public:

  SimTaDynMapHolder() {}
  ~SimTaDynMapHolder() {}

  //------------------------------------------------------------------
  //! \brief TODO: WIP suppressing singletons
  //------------------------------------------------------------------
  inline SimTaDynMapManager& resourceManager()
  {
    return SimTaDynMapManager::instance();
  }

  void set(SimTaDynMapPtr p)
  {
    m_map = p;
    emitMapChanged();
    // TODO: notify to SimForth to get the address of the scenegraph<SimTaDynSheet>
  }

  SimTaDynMapPtr get(std::string const& name)
  {
    // The desired map is already currently used
    if ((nullptr != m_map) && (m_map->name() == name))
      return m_map;

    // Get the desired map
    SimTaDynMapPtr new_map = resourceManager().acquire(name);

    // The desired map does not exist
    if (nullptr == new_map)
      {
        if (nullptr == m_map)
          {
            LOGE("Cannot select SimTaDyn map #%s because there is no map to select",
                 name.c_str());
          }
        else
          {
            LOGE("Cannot select SimTaDyn map #%s. Keep using the current map #%s",
                 name.c_str(), m_map->name().c_str());
          }
      }
    else // Found
      {
        m_map = new_map;
        emitMapChanged();
      }

    return m_map;
  }

  inline SimTaDynMapPtr get()
  {
    return m_map;
  }

  // Can emit a nullptr as map
  inline void emitMapChanged()
  {
    LOGS("SimTaDynMapHolder emit signal_top_map_changed");
    signal_top_map_changed.emit(m_map);
  }

  sigc::signal<void, SimTaDynMapPtr> signal_top_map_changed;

protected:

  SimTaDynMapPtr m_map = nullptr;
};

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : public sigc::trackable
{
public:

  //! \brief Add, remove a mode (node, arc, zone).
  enum ActionType { Add, Remove, Select, Move, FirstAction = Add, LastAction = Move };

  //! \brief On what kind of cells action is performed.
  enum ActionOn { Node, Arc, Zone, FirstMode = Node, LastMode = Zone };

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  MapEditor(SimForth& forth);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  ~MapEditor();

public:

  //*******************************************************************
  // Widgets and classes access
  //*******************************************************************
  //------------------------------------------------------------------
  //! \brief Return the GTK+ widget to be inserted inside a GTK+ window.
  //------------------------------------------------------------------
  inline Gtk::Widget &widget()
  {
    return m_hbox;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynMapHolder& mapHolder()
  {
    return m_current_map;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynMapManager& resourceManager()
  {
    return m_current_map.resourceManager();
  }

  //*******************************************************************
  // Maps
  //*******************************************************************

  void drawCurrentMap(/*GLDrawingArea& renderer*/)
  {
    SimTaDynMapPtr map = m_current_map.get();
    if ((nullptr != map))
      {
        map->drawnBy(m_drawing_area/*renderer*/);
      }
  }

  //------------------------------------------------------------------
  //! \brief Return the current map
  //------------------------------------------------------------------
  SimTaDynMap& map()
  {
    SimTaDynMapPtr map = m_current_map.get();
    if (nullptr == map)
      {
        newMap();
        map = m_current_map.get();
      }
    return *map;
  }

  //------------------------------------------------------------------
  //! \brief Return the map by its name or nullptr if it does
  //! not exit.
  //------------------------------------------------------------------
  inline SimTaDynMapPtr map(std::string const& name)
  {
    return m_current_map.get(name);
  }

  //------------------------------------------------------------------
  //! \brief Load a new SimTaDyn map from a file through a dialog box.
  //------------------------------------------------------------------
  inline bool openMap()
  {
    return dialogLoadMap(true, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a new map from the given file.
  //------------------------------------------------------------------
  inline bool openMap(std::string const& filename)
  {
    return doOpenMap(filename, true, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a new map from a file which is not to the SimTaDynMap
  //! format through a dialog box.
  //------------------------------------------------------------------
  inline bool importMap()
  {
    return dialogLoadMap(true, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a new map from a file which is not to the SimTaDynMap
  //! format through a dialog box.
  //------------------------------------------------------------------
  inline bool importMap(std::string const& filename)
  {
    return doOpenMap(filename, true, false);
  }

  //------------------------------------------------------------------
  //! \brief Create a dummy map.
  //------------------------------------------------------------------
  void newMap();

  //------------------------------------------------------------------
  //! \brief Load a map from a file and merge it inside the current map.
  //------------------------------------------------------------------
  inline bool mergeMap()
  {
    return dialogLoadMap(false, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a map from a file and merge it inside the current map.
  //------------------------------------------------------------------
  inline bool mergeMap(std::string const& filename)
  {
    return doOpenMap(filename, false, false);
  }

  //------------------------------------------------------------------
  //! \brief Load a map and replace the current map. Load a map and replace the current map.
  //------------------------------------------------------------------
  inline bool replaceMap()
  {
    return dialogLoadMap(false, true);
  }

  //------------------------------------------------------------------
  //! \brief Load a map and replace the current map. FIXME: ca semble etre inutile: il suffit de renommer les fichiers
  //------------------------------------------------------------------
  inline bool replaceMap(std::string const& filename)
  {
    return doOpenMap(filename, false, true);
  }

  //------------------------------------------------------------------
  //! \brief Reset the current map (suppress everything)
  //------------------------------------------------------------------
  inline void clearMap()
  {
    if (nullptr != m_current_map.get())
      {
        m_current_map.get()->resetSheets();
      }
  }

  //------------------------------------------------------------------
  //! \brief Close the current map and activate the previous one (if
  //! present)
  //------------------------------------------------------------------
  void closeMap();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void saveMap() { LOGF("saveMap not yet implemented"); } //TODO

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void saveAsMap();

  //*******************************************************************
  // Sheets
  //*******************************************************************

  inline void emitSheetChanged()
  {
    LOGS("sheet_changed.emit");
    sheet_changed.emit(m_current_map.get());//, map().currentSheetPtr());
  }

  //------------------------------------------------------------------
  //! \brief Return the current sheet
  //------------------------------------------------------------------
  inline SimTaDynSheet& sheet()
  {
    return map().currentSheet();
  }

  //------------------------------------------------------------------
  //! \brief Return the current sheet by its name or nullptr if it
  //! does not exit.
  //------------------------------------------------------------------
  inline SimTaDynSheetPtr sheet(std::string const& name)
  {
    return map().sheet(name);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline bool openSheet(std::string const& filename)
  {
    LOGE("addSheet");
    return doOpenSheet(filename, true, false);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline bool openSheet()
  {
    LOGE("addSheet");
    return dialogLoadSheet(true, false);
  }

  // Request form MapExplorer
  // TODO addSisterdSheet(SceneNodePtr node) + signal sheet_added
  // TODO addChildSheet(SceneNodePtr node) + signal sheet_added
  // TODO removeSheet(SceneNodePtr node) + signal sheet_removed

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynSheetPtr newSheet(bool const directed = true)
  {
    LOGI("createSheet");
    SimTaDynSheetPtr sheet = map().createSheet(directed);
    emitSheetChanged();
    return sheet;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline bool addSheet()
  {
    return dialogLoadSheet(true, false);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline bool replaceSheet()
  {
    return dialogLoadSheet(false, true);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void clearSheet()
  {
    sheet().reset();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void resetSheets()
  {
    map().resetSheets();
    emitSheetChanged();
  }

  //! \brief Close the current map and activate the previous one (if
  //! present)
  void closeSheet()  { LOGF("closeSheet not yet implemented"); } //TODO
  void saveSheet()   { LOGF("saveSheet not yet implemented"); } //TODO
  void saveAsSheet() { LOGF("saveAsSheet not yet implemented"); } //TODO

  //*******************************************************************
  // Forth/Spreadsheet
  //*******************************************************************

  bool evalSheet();

  //*******************************************************************
  // OpenGL
  //*******************************************************************


  //*******************************************************************
  // GIS edition
  //*******************************************************************

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void button1PressEvent(const gdouble x, const gdouble y);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void button2PressEvent(const gdouble x, const gdouble y);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void button3PressEvent(const gdouble x, const gdouble y);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  ActionType actionType() const
  {
    return static_cast<ActionType>(m_action_type.button());
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  ActionOn actionOn() const
  {
    return static_cast<ActionOn>(m_action_on.button());
  }

private:

  bool doOpenMap(std::string const& filename, bool const new_map, bool const reset_map);
  bool doOpenSheet(std::string const& filename, bool const new_map, bool const reset_map);
  template <class L>
  bool dialogLoad(std::string const& title, std::string& filename);
  bool dialogLoadMap(bool const new_map, bool const reset_map);
  bool dialogLoadSheet(bool const new_sheet, bool const reset_sheet);
  bool dialogSaveAsMap(bool const closing);
  void onActionOnSelected_(const ActionOn id);
  void onActionTypeSelected_(const ActionType id);
  inline void onActionOnSelected(const uint32_t id)
  {
    onActionOnSelected_(static_cast<ActionOn>(id));
  }
  inline void onActionTypeSelected(const uint32_t id)
  {
    onActionTypeSelected_(static_cast<ActionType>(id));
  }
  void repaintMap(SimTaDynMapPtr map);

public:

  SimForth&              m_forth;
  //! \brief Model of the MVC design pattern
  SimTaDynMapHolder      m_current_map;
  //! \brief View of the MVC pattern
  GLDrawingArea          m_drawing_area;
  //! \brief View/Controler of the MVC pattern
  SimTaDynMapExplorer    m_map_explorer;
  //! \brief View/Controler of the MVC pattern
  CellEditor             m_cell_editor;

  //!
  Gtk::MenuItem         m_menuitem[simtadyn::MaxMapMenuNames + 1];

  sigc::signal<void, SimTaDynMapPtr/*, SceneNodePtr*/> sheet_changed;
  sigc::signal<void, SimTaDynMapPtr> loaded_success;
  sigc::signal<void, const std::string &, const std::string &> loaded_failure;
  sigc::signal<void, SimTaDynMapPtr> saved_success;
  sigc::signal<void, const std::string &, const std::string &> saved_failure;

protected:

  Gtk::Toolbar           m_toolbar;
  ToggleButtons          m_action_type;
  ToggleButtons          m_action_on;
  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[32];
  Gtk::Image             m_image[32];
  Gtk::SeparatorMenuItem m_menu_separator[2];
  Gtk::SeparatorToolItem m_toolbar_separator[2];
  Gtk::VBox              m_vbox;
  Gtk::HBox              m_hbox;
  std::unique_ptr<MapEditionTools> m_edition_tools[ActionType::LastAction + 1u];
  size_t m_map_id = 0_z;
};

#endif /* MAPEDITOR_HPP_ */
