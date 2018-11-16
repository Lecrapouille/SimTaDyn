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
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"
#  include "ToggleButtons.hpp"

class LoaderManager;
class MapEditionTools;

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

  void set(SimTaDynMapPtr p)
  {
    m_map = p;
    if (nullptr != m_map)
      {
        m_map->signal_changed.emit(m_map);
        // TODO: notify to SimForth to get the address of the scenegraph<SimTaDynSheet>
      }
  }

  SimTaDynMapPtr get(std::string const& name)
  {
    // The desired map is already currently used
    if ((nullptr != m_map) && (m_map->name() == name))
      return m_map;

    // Get the desired map
    SimTaDynMapPtr new_map = SimTaDynMapManager::instance().acquire(name);

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
        m_map->signal_changed.emit(m_map);
      }

    return m_map;
  }

  inline SimTaDynMapPtr get()
  {
    return m_map;
  }

public:

  sigc::signal<void, SimTaDynMapPtr> signal_changed;

protected:

  SimTaDynMapPtr m_map;
};

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : public Singleton<MapEditor>
{
public:

  //! \brief Add, remove a mode (node, arc, zone).
  enum ActionType { Add, Remove, Select, Move, FirstAction = Add, LastAction = Move };

  //! \brief On what kind of cells action is performed.
  enum ActionOn { Node, Arc, Zone, FirstMode = Node, LastMode = Zone };

private:

  friend class Singleton<MapEditor>;

  //! \brief Private because of Singleton.
  MapEditor();

  //! \brief Private because of Singleton.
  virtual ~MapEditor();

public:

  inline Gtk::Widget &widget()
  {
    return m_hbox;
  }

  //! \brief Return the current map
  inline SimTaDynMapPtr map()
  {
    return m_current_map.get();
  }

  inline SimTaDynMapPtr map(std::string const& name)
  {
    return m_current_map.get(name);
  }

  //! \brief Load a new SimTaDyn map from a file through a dialog box.
  inline bool open()
  {
    return dialogLoadMap(false, true);
  }

  //! \brief Load a new map from a file.
  inline bool open(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //! \brief Load a new map from a file through a dialog box.
  inline bool import()
  {
    return dialogLoadMap(true, false);
  }

    //! \brief Load a new map from a file through a dialog box.
  inline bool import(std::string const& filename)
  {
    return doOpen(filename, true, false);
  }

  //! \brief Create a dummy map.
  void newMap();

  //! \brief Load a map from a file in the current map.
  inline bool addMap()
  {
    return dialogLoadMap(false, false);
  }

  //! \brief Load a map from a file in the current map.
  inline bool addMap(std::string const& filename)
  {
    return doOpen(filename, false, false);
  }

  //! \brief Load a map and replace the current map.
  inline bool replace()
  {
    return dialogLoadMap(false, true);
  }

  //! \brief Load a map and replace the current map.
  inline bool replace(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //! \brief Reset the map (suppres everything)
  inline void clear()
  {
    if (nullptr != m_current_map.get())
      {
        m_current_map.get()->clear();
      }
  }

  void button1PressEvent(const gdouble x, const gdouble y);
  void button2PressEvent(const gdouble x, const gdouble y);
  void button3PressEvent(const gdouble x, const gdouble y);


  //! \brief Close the current map and activate the previous one (if
  //! present)
  void close();
  void save() {} //TODO
  void saveAs();
  bool exec();

  //! \brief Attach a the MVC view to this MVC controller.
  void attachView(GLDrawingArea& drawing_area)
  {
    m_drawing_area = &drawing_area;
    //m_drawing_area->attachController(this);
    m_vbox.pack_start(drawing_area);
  }

  ActionType actionType() const
  {
    return static_cast<ActionType>(m_action_type.button());
  }

  ActionOn actionOn() const
  {
    return static_cast<ActionOn>(m_action_on.button());
  }

protected:

  virtual bool load(const std::string& filename, SimTaDynMapPtr oldmap);
  bool doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  bool dialogLoadMap(const bool new_map, const bool reset_map);
  bool dialogSaveAsMap(const bool closing);
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
  void add(const Key name, SimTaDynMapPtr map);
  void remove(const Key name);

public:

  //! \brief Current model of the MVC design pattern
  SimTaDynMapHolder     m_current_map;
  //! \brief View implementation of the MVC
  GLDrawingArea         *m_drawing_area = nullptr;
  //!
  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];

  sigc::signal<void, SimTaDynMapPtr> loaded_success;
  sigc::signal<void, const std::string &, const std::string &> loaded_failure;
  sigc::signal<void, SimTaDynMapPtr> saved_success;
  sigc::signal<void, const std::string &, const std::string &> saved_failure;

protected:

  Gtk::Toolbar           m_toolbar;
  ToggleButtons          m_action_type;
  ToggleButtons          m_action_on;
  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menu_separator[2];
  Gtk::SeparatorToolItem m_toolbar_separator[2];
  Inspector              m_inspector;
  Gtk::VBox              m_vbox;
  Gtk::HBox              m_hbox;
  MapEditionTools       *m_edition_tools[ActionType::LastAction + 1u];
};

#endif /* MAPEDITOR_HPP_ */
