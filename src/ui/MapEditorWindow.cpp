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

#include "MapEditorWindow.hpp"

//------------------------------------------------------------------
MapEditorWindow::MapEditorWindow(Glib::RefPtr<Gtk::Application> application)
  : ISimTaDynWindow(application),
    m_action_type(m_toolbar),
    m_action_on(m_toolbar)
{
  populatePopovMenu();
  populateToolBar();

  m_hbox.pack_start(m_vbox);
  m_hbox.pack_start(m_toolbar, Gtk::PACK_SHRINK);

  //
  Gtk::Box* box = Gtk::make_managed<Gtk::Box>();
  m_drawing_area = createView();
  box->pack_start(*m_drawing_area);
  m_vbox.pack_start(*box); // FIXME: really ?

  add(m_hbox);
  show_all();
}

//------------------------------------------------------------------
void MapEditorWindow::populateToolBar()
{
  m_toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
  m_toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

  // Cells
  m_action_on.append(ActionOn::Node, "Switch to Node mode", Gtk::Stock::YES,
                     sigc::mem_fun(*this, &MapEditorWindow::onActionOnSelected));
  m_action_on.append(ActionOn::Arc, "Switch to Arc mode", Gtk::Stock::NO,
                     sigc::mem_fun(*this, &MapEditorWindow::onActionOnSelected));
  m_action_on.append(ActionOn::Zone, "Switch to Zone mode", Gtk::Stock::HOME,
                     sigc::mem_fun(*this, &MapEditorWindow::onActionOnSelected));
  m_action_on.append(m_toolbar_separator[0]);

  // Operations on cells
  m_action_type.append(ActionType::Add, "Remove a cell", Gtk::Stock::ADD,
                       sigc::mem_fun(*this, &MapEditorWindow::onActionTypeSelected));
  m_action_type.append(ActionType::Remove, "Remove a cell", Gtk::Stock::REMOVE,
                       sigc::mem_fun(*this, &MapEditorWindow::onActionTypeSelected));
  m_action_type.append(ActionType::Select, "Select a cell", Gtk::Stock::JUMP_TO,
                       sigc::mem_fun(*this, &MapEditorWindow::onActionTypeSelected));
  m_action_type.append(ActionType::Move, "Move a cell", Gtk::Stock::JUMP_TO,
                       sigc::mem_fun(*this, &MapEditorWindow::onActionTypeSelected));
  m_toolbar.append(m_toolbar_separator[1]);
}

//------------------------------------------------------------------
void MapEditorWindow::populatePopovMenu()
{
  Glib::RefPtr<Gio::Menu> menu = Gio::Menu::create();

  Glib::RefPtr<Gio::Menu> submenu_map = Gio::Menu::create();
  submenu_map->append("Replace Map", "win.replace-map");
  submenu_map->append("Merge Map", "win.merge-map");
  submenu_map->append("Clear Map", "win.clear-map");
  menu->append_submenu("Map", submenu_map);

  Glib::RefPtr<Gio::Menu> submenu_sheet = Gio::Menu::create();
  submenu_sheet->append("New Direct Sheet", "win.new-direct-sheet");
  submenu_sheet->append("New Undirect Sheet", "win.new-undirect-sheet");
  submenu_sheet->append("Import Sheet", "win.import-sheet");
  submenu_sheet->append("Replace Sheet", "win.replace-sheet");
  submenu_sheet->append("Clear Sheet", "win.clear-sheet");
  menu->append_submenu("Sheet", submenu_sheet);

  add_action("replace-map", sigc::mem_fun(*this, &MapEditorWindow::actionReplaceCurrentMap));
  add_action("merge-map", sigc::mem_fun(*this, &MapEditorWindow::actionMergeMap));
  add_action("clear-map", sigc::mem_fun(*this, &MapEditorWindow::actionClearMap));
  add_action("new-direct-sheet", sigc::mem_fun(*this, &MapEditorWindow::actionNewDirectSheet));
  add_action("new-undirect-sheet", sigc::mem_fun(*this, &MapEditorWindow::actionNewUndirectSheet));
  add_action("import-sheet", sigc::mem_fun(*this, &MapEditorWindow::actionImportSheet));
  add_action("replace-sheet", sigc::mem_fun(*this, &MapEditorWindow::actionReplaceSheet));
  add_action("clear-sheet", sigc::mem_fun(*this, &MapEditorWindow::actionClearSheet));

  m_menu_button.set_popover(m_menu_popov);
  m_menu_button.set_menu_model(menu);
  m_menu_popov.set_size_request(-1, -1);
}

//------------------------------------------------------------------
void MapEditorWindow::actionReplaceCurrentMap()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionMergeMap()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionClearMap()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionNewDirectSheet()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionNewUndirectSheet()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionImportSheet()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionReplaceSheet()
{
}

//------------------------------------------------------------------
void MapEditorWindow::actionClearSheet()
{
}

//------------------------------------------------------------------
void MapEditorWindow::splitView(Gtk::Orientation const orientation)
{
  GLDrawingArea* view = createView();
  findParent<Gtk::Window>(view);
  splitWidget(currentView(), *view, orientation, Pack::First);
}

//------------------------------------------------------------------
// FIXME: current Presenter
GLDrawingArea& MapEditorWindow::currentView()
{
  return *m_drawing_area;
}

//------------------------------------------------------------------
// FIXME: non complet
GLDrawingArea* MapEditorWindow::createView()
{
  return Gtk::make_managed<GLDrawingArea>();
}

//------------------------------------------------------------------
void MapEditorWindow::onOpenFileClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onRecentFilesClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onHorizontalSplitClicked()
{
  splitView(Gtk::Orientation::ORIENTATION_HORIZONTAL);
}

//------------------------------------------------------------------
void MapEditorWindow::onVerticalSplitClicked()
{
  splitView(Gtk::Orientation::ORIENTATION_VERTICAL);
}

//------------------------------------------------------------------
void MapEditorWindow::onUndoClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onRedoClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onSaveFileClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onSaveAsFileClicked()
{
}

//------------------------------------------------------------------
void MapEditorWindow::onActionOnSelected(const ActionOn id)
{
}
//------------------------------------------------------------------
bool MapEditorWindow::onExit(GdkEventAny* /*event*/)
{
  //bool res = closeAll();
  //return !res;
  return false;
}

//------------------------------------------------------------------
void MapEditorWindow::onActionTypeSelected(const ActionType id)
{
  //LOGI("ActionTypeSelected %u", id);

  // http://transit.iut2.upmf-grenoble.fr/doc/gtkmm-3.0/reference/html/group__gdkmmEnums.html
  // Change the mouse cursor
  static Gdk::CursorType cursors[] =
    {
      [ActionType::Add] = Gdk::PLUS,
      [ActionType::Remove] = Gdk::PIRATE,
      [ActionType::Select] = Gdk::HAND1,
      [ActionType::Move] = Gdk::HAND2,
    };

  get_window()->set_cursor(Gdk::Cursor::create(cursors[id]));
}
