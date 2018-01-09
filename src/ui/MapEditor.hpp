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

#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include "Names.hpp"
#  include "SimTaDynLoaders.hpp"
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"
#  include "ToggleButtons.hpp"
#  include "MapEditionTools.hpp"

class LoaderManager;

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

protected:

  // ***********************************************************************************************
  //! \brief Implements the SimTaDynMap::ISimTaDynMapListener interface.
  // ***********************************************************************************************
  class SimTaDynMapListener: public ISimTaDynMapListener
  {
  public:

    //! \brief
    SimTaDynMapListener(MapEditor &editor)
      : m_editor(editor)
    {
    }

    //! \brief Callback when the map changed: draw it.
    virtual void onChanged(SimTaDynMap* map) override
    {
      LOGI("MapEditor::SimTaDynMapListener has detected that Map %s has changed 0x%x",
           map->m_name.c_str(), map);
      if (nullptr == m_editor.m_drawing_area)
        return ;
      //FIXME m_editor.m_drawing_area->drawThat(*map);
    }

    MapEditor& m_editor;
  };

public:

  inline Gtk::Widget &widget()
  {
    return m_hbox;
  }

  //! \brief Return the current map
  inline SimTaDynMap* map()
  {
    return m_current_map.get();
  }
  SimTaDynMap* map(const Key id);

  //! \brief Load a new map from a file through a dialog box.
  inline bool open()
  {
    return dialogLoadMap(true, false);
  }

  //! \brief Load a new map from a file.
  inline bool open(std::string const& filename)
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

  void button1PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton1 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->exec1(x, y);
  }

  void button2PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton2 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->exec2(x, y);
  }

  void button3PressEvent(const gdouble x, const gdouble y)
  {
    LOGD("Bouton3 click %d %d", (int) x, (int) y);
    m_edition_tools[actionType()]->exec3(x, y);
  }

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

  void registerLoaders();
  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap);
  bool doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  bool dialogLoadMap(const bool new_map, const bool reset_map);
  bool dialogSaveAsMap(const bool closing);
  void add(const Key id, SimTaDynMap* map);
  void remove(const Key id);
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

public:

  //! \brief Current 'Model' of the MVC design pattern.
  SimTaDynMapHolder     m_current_map;
  //! \brief View implementation of the MVC design pattern
  GLDrawingArea         *m_drawing_area = nullptr;
  //!
  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];

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
  SimTaDynMapListener    m_listener;
  MapEditionTools       *m_edition_tools[ActionType::LastAction + 1u];
};

#endif /* MAPEDITOR_HPP_ */
