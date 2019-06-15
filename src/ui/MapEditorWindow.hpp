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

#  include "Windows.hpp"
#  include "MapEditor.hpp"
#  include "InputsHandler.hpp"
#  include "MapEditorCommands.hpp"
#  include "MapEditionTools.hpp"
#  include "MapExplorerWindow.hpp"
#  include <unordered_map>

class IMapEditorCommand;
class MapEditionTools;

// **************************************************************
//! \brief
// **************************************************************
class MapEditorWindow :
  public MainWindow,
  public MouseHandler,
  public KeyBoardHandler
{
  friend class SplitScreenCommand;

public:

  MapEditorWindow();
  inline MapEditor& mapEditor() { return m_editor; }
  inline ActionType actionType() const { return m_action_type.button(); }
  inline ActionOn actionOn() const { return m_action_on.button(); }

private:

  void populatePopovMenu();
  void populateToolBar();
  void splitView(Gtk::Orientation const orientation);
  void bindKeyBoardCommands();
  void bindToolbarCommands();
  void bindMapExplorerSignals();

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
  virtual bool onMousePressed(GdkEventButton* event) override;

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

  template <class L>
  bool dialogLoad(std::string const& title, std::string& filename);

protected:

  using KeyBoardCommandContainer = std::unordered_map<int, std::unique_ptr<IMapEditorCommand>>;
  using ToolCommandContainer = std::unordered_map<ActionType, std::unique_ptr<MapEditionTools>>;

  std::vector<int>           m_allowed_keys;
  KeyBoardCommandContainer   m_keyboard_commands;
  ToolCommandContainer       m_mouse_commands;

private:

  MapEditor                  m_editor; // FIXME: Non shared by all windows
  MapExplorerWindow          m_explorer;
  Gtk::Popover               m_menu_popov;
  Gtk::HBox                  m_hbox;
  Gtk::SeparatorToolItem     m_toolbar_separator[2];
  Gtk::Toolbar               m_toolbar;
  ToggleButtons<ActionType>  m_action_type;
  ToggleButtons<ActionOn>    m_action_on;
};

#endif // MAP_EDITOR_WINDOW_HPP
