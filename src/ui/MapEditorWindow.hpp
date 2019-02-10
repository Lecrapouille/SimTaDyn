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

#ifndef MAP_EDITOR_WINDOW_HPP
#  define MAP_EDITOR_WINDOW_HPP

#  include "ToggleButtons.hpp"
#  include "SimTaDynWindow.hpp"
#  include "KeyBoardHandler.hpp"
#  include "KeyBoardCommands.hpp"
#  include "MVP.hpp"
#  include <unordered_map>

class MapEditor
{
protected:

  //! \brief Add, remove a mode (node, arc, zone).
  enum ActionType { Add, Remove, Select, Move, MaxActionType_ };

  //! \brief On what kind of cells action is performed.
  enum ActionOn { Node, Arc, Zone, MaxActionOn_ };

public:

  MapEditor() :
    m_action_type(m_toolbar),
    m_action_on(m_toolbar)
  {
    addPresenter(new MapPresenter(/*FIXME *this, createDummyMap()*/));
    bindKeyBoardCommands();
  }

#if 0
  bool evalSheet() // FIXME: Exec(typeCell, nodeID)
  {
    SimTaDynSheet& sheet = activeModel().map().currentSheet();

    // FIXME: should be called outside each cell: optimisation
    // Disable compilation mode
    m_forth.dictionary().smudge(":");
    m_forth.dictionary().smudge("INCLUDE");

    sheet.parse(m_forth);
    std::pair<bool, std::string> res = sheet.evaluate(m_forth);
    m_forth.ok(res);

    // Enable compilation mode
    m_forth.dictionary().smudge("INCLUDE");
    m_forth.dictionary().smudge(":");

    return res.first;
  }
#endif

  void moveCamera(CameraDirection const direction)
  {
    //TODO activeView().moveCamera(direction);
  }

  inline MapPresenter& activePresenter()
  {
    return *m_active_presenter;
  }

  inline SimTaDynMap& activeModel()
  {
    return m_active_presenter->model();
  }

  inline SimTaDynMapPtr activeModelPtr()
  {
    return m_active_presenter->modelPtr();
  }

  inline GLDrawingArea& activeView()
  {
    return m_active_presenter->view();
  }

  inline void closePresenter()
  {
    m_presenters.pop_back();
    if (m_presenters.size() > 0_z)
      {
        m_active_presenter = m_presenters.back();
      }
    else
      {
        // kill the window
      }
  }

  inline void addPresenter(MapPresenter* presenter)
  {
    m_active_presenter = presenter;
    m_presenters.push_back(presenter);
  }

  inline ActionType actionType() const
  {
    return m_action_type.button();
  }

  ActionOn actionOn() const
  {
    return m_action_on.button();
  }

private:

  void bindKeyBoardCommands();

protected:

  // FIXME: a qui appartient m_toolbar ? MapEditor ou MapEditorWindow ? populateToolBar() ? onRefreshKeyboard()
  Gtk::Toolbar              m_toolbar;
  ToggleButtons<ActionType>  m_action_type;
  ToggleButtons<ActionOn>    m_action_on;
  std::vector<int> m_allowed_keys;
  std::unordered_map<int, MapEditorCommand*> m_commands;

private:

  std::vector<MapPresenter*> m_presenters;
  MapPresenter*              m_active_presenter = nullptr;
};

class MapEditorWindow :
  public MapEditor,
  public ISimTaDynWindow,
  public KeyBoardHandler
{
  friend class SplitScreenCommand;

public:

  MapEditorWindow(Glib::RefPtr<Gtk::Application> application);

private:

  void populatePopovMenu();
  void populateToolBar();
  void splitView(Gtk::Orientation const orientation);

  virtual void onOpenFileClicked() override;
  virtual void onRecentFilesClicked() override;
  virtual void onHorizontalSplitClicked() override;
  virtual void onVerticalSplitClicked() override;
  virtual void onUndoClicked() override;
  virtual void onRedoClicked() override;
  virtual void onSaveFileClicked() override;
  virtual void onSaveAsFileClicked() override;
  virtual bool onExit(GdkEventAny* event) override;
  virtual bool onRefreshKeyboard() override;

  void onActionOnSelected(const ActionOn id);
  void onActionTypeSelected(const ActionType id);

  void actionReplaceCurrentMap();
  void actionMergeMap();
  void actionClearMap();
  void actionNewDirectSheet();
  void actionNewUndirectSheet();
  void actionImportSheet();
  void actionReplaceSheet();
  void actionClearSheet();

private:

  Gtk::Popover              m_menu_popov;
  Gtk::VBox                 m_vbox;
  Gtk::HBox                 m_hbox;
  Gtk::SeparatorToolItem    m_toolbar_separator[2];
};

#endif // MAP_EDITOR_WINDOW_HPP
