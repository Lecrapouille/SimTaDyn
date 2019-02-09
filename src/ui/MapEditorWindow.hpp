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

#ifndef MAPEDITORWINDOW_HPP
#  define MAPEDITORWINDOW_HPP

#  include "SimTaDynWindow.hpp"

class MapEditorWindow: public ISimTaDynWindow
{
  //! \brief Add, remove a mode (node, arc, zone).
  enum ActionType { Add, Remove, Select, Move, MaxActionType_ };

  //! \brief On what kind of cells action is performed.
  enum ActionOn { Node, Arc, Zone, MaxActionOn_ };

public:

  MapEditorWindow(Glib::RefPtr<Gtk::Application> application);

private:

  void populatePopovMenu();
  void populateToolBar();
  void splitView(Gtk::Orientation const orientation);
  GLDrawingArea* createView();
  GLDrawingArea& currentView();

  virtual void onOpenFileClicked() override;
  virtual void onRecentFilesClicked() override;
  virtual void onHorizontalSplitClicked() override;
  virtual void onVerticalSplitClicked() override;
  virtual void onUndoClicked() override;
  virtual void onRedoClicked() override;
  virtual void onSaveFileClicked() override;
  virtual void onSaveAsFileClicked() override;
  virtual bool onExit(GdkEventAny* event) override;

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
  GLDrawingArea*            m_drawing_area;
  Gtk::Toolbar              m_toolbar;
  ToggleButtons<ActionType> m_action_type;
  ToggleButtons<ActionOn>   m_action_on;
  Gtk::SeparatorToolItem    m_toolbar_separator[2];
};

#endif
