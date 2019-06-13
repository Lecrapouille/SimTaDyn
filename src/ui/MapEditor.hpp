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

#  include "SimTaDynForth.hpp"
#  include "ToggleButtons.hpp"
#  include "MVP.hpp"
#  include "SimTaDynLoaders.hpp"

using SimTaDynMapManager = ResourceManager<std::string, SimTaDynMap>;

//! \brief Add, remove a mode (node, arc, zone).
enum ActionType { Add, Remove, Select, Move, MaxActionType_ };

//! \brief On what kind of cells action is performed.
enum ActionOn { Node, Arc, Zone, MaxActionOn_ };

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class MapEditor
{

public:

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  MapEditor(PopupException& popup_exception, SimForth& forth);

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  bool evalSheet();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void setActionOn(const ActionOn action_on)
  {
    m_action_on = action_on;
  }

  inline ActionOn actionOn() const { return m_action_on; }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void moveCamera(CameraDirection const direction)
  {
    activeView().moveCameraCommand(direction);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline MapPresenter& activePresenter()
  {
    return *m_active_presenter;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynSheet& activeSheet()
  {
    return activeModel().currentSheet();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynMap& activeModel()
  {
    return m_active_presenter->model();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void changeActiveModel(SimTaDynMapPtr map)
  {
    return m_active_presenter->model(map);
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynMapPtr activeModelPtr()
  {
    return m_active_presenter->modelPtr();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline GLDrawingArea& activeView()
  {
    return m_active_presenter->view();
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline SimTaDynMapManager& resourcesManager()
  {
    return m_resources;
  }

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void closePresenter();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  inline void addPresenter(MapPresenter* presenter)
  {
    m_active_presenter = presenter;
    m_presenters.push_back(presenter);
  }

  //------------------------------------------------------------------

  bool dialogLoadMap(Gtk::Window& win, bool const new_map, bool const reset_map);
  bool dialogLoadSheet(Gtk::Window& win, bool const new_sheet, bool const reset_sheet);//TODO ,SimTaDynSheet& sheet)
  bool doOpenMap(std::string const& filename, bool const new_map, bool const reset_map);
  bool doOpenSheet(std::string const& filename, bool const new_sheet, bool const reset_sheet);

private:

  template <class L>
  bool dialogLoad(Gtk::Window& win, std::string const& title, std::string& filename);

public:

  //sigc::signal<void, SimTaDynMapPtr/*, SceneNodePtr*/> sheet_changed;
  sigc::signal<void, SimTaDynMapPtr> loaded_success;
  sigc::signal<void, const std::string &, const std::string &> loaded_failure;
  //sigc::signal<void, SimTaDynMapPtr> saved_success;
  //sigc::signal<void, const std::string &, const std::string &> saved_failure;

private:

  SimTaDynMapManager         m_resources;
  PopupException             m_popup_exception;
  SimForth&                  m_forth;
  std::vector<MapPresenter*> m_presenters;
  MapPresenter*              m_active_presenter = nullptr;
  ActionOn                   m_action_on;
};

#endif /* MAPEDITOR_HPP_ */
