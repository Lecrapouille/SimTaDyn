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
#include "SimTaDyn.hpp"

//------------------------------------------------------------------
MapEditorWindow::MapEditorWindow()
  : MainWindow(SimTaDyn::application()),
    m_editor(m_exception_dialog, SimTaDyn::forth()),
    m_explorer(SimTaDyn::application()),
    m_action_type(m_toolbar),
    m_action_on(m_toolbar)
{
  m_application->signal_startup().connect([&] {
      m_application->add_window(m_explorer);
    });

  initMouseHandler(*this);
  initKeyBoardHandler(*this);
  populatePopovMenu();
  populateToolBar();
  bindKeyBoardCommands();
  bindToolbarCommands();
  bindMapExplorerSignals();

  MapPresenter* presenter = new MapPresenter(m_exception_dialog/*FIXME *this, createDummy*/);
  m_editor.addPresenter(presenter);
  Gtk::Box* box = Gtk::make_managed<Gtk::Box>();
  box->pack_start(presenter->view());

  m_hbox.pack_start(*box);
  m_hbox.pack_start(m_toolbar, Gtk::PACK_SHRINK);
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
void MapEditorWindow::bindKeyBoardCommands()
{
  std::cout << "bindKeyBoardCommands" << std::endl;
  m_allowed_keys = { GDK_KEY_F1, GDK_KEY_F2, GDK_KEY_F3, GDK_KEY_F4,
                     GDK_KEY_Page_Up, GDK_KEY_Page_Down, GDK_KEY_Left,
                     GDK_KEY_Right };

  m_keyboard_commands[GDK_KEY_F1] = std::make_unique<SplitScreenCommand>(Gtk::Orientation::ORIENTATION_HORIZONTAL);
  m_keyboard_commands[GDK_KEY_F2] = std::make_unique<SplitScreenCommand>(Gtk::Orientation::ORIENTATION_VERTICAL);
  m_keyboard_commands[GDK_KEY_Page_Up] = std::make_unique<MoveCameraCommand>(CameraDirection::Up);
  m_keyboard_commands[GDK_KEY_Page_Down] = std::make_unique<MoveCameraCommand>(CameraDirection::Down);
  m_keyboard_commands[GDK_KEY_Left] = std::make_unique<MoveCameraCommand>(CameraDirection::Left);
  m_keyboard_commands[GDK_KEY_Right] = std::make_unique<MoveCameraCommand>(CameraDirection::Right);
  m_keyboard_commands[GDK_KEY_F3] = std::make_unique<MoveCameraCommand>(CameraDirection::Forward);
  m_keyboard_commands[GDK_KEY_F4] = std::make_unique<MoveCameraCommand>(CameraDirection::Backward);
}

//------------------------------------------------------------------
void MapEditorWindow::bindToolbarCommands()
{
  std::cout << "bindToolbarCommands" << std::endl;
  m_mouse_commands[ActionType::Add] = std::make_unique<AddCellTool>(m_editor);
  m_mouse_commands[ActionType::Remove] = std::make_unique<RemoveCellTool>(m_editor);
  m_mouse_commands[ActionType::Select] = std::make_unique<SelectCellTool>(m_editor);
  m_mouse_commands[ActionType::Move] = std::make_unique<MoveCellTool>(m_editor);
}

//------------------------------------------------------------------
void MapEditorWindow::bindMapExplorerSignals()
{
  m_editor.loaded_success.connect(
     sigc::mem_fun(m_explorer, &MapExplorerWindow::onMapCreated));
  //m_editor.loaded_failure.connect(
  //   sigc::mem_fun(m_explorer, &MapExplorerWindow::onMapFailure));
}

//------------------------------------------------------------------
bool MapEditorWindow::onMousePressed(GdkEventButton* event)
{
  std::cout << "MapEditorWindow::onMousePressed" << std::endl;
  if (event->type == GDK_BUTTON_PRESS)
    {
      switch (event->button)
        {
        case 1:
          std::cout << "GLDrawingArea::on_button_press_event button1" << std::endl;
          m_mouse_commands[actionType()]->button1PressEvent(event->x, event->y); // TODO position relative a la fenetre courante
          break;
        case 2:
          std::cout << "GLDrawingArea::on_button_press_event button2" << std::endl;
          m_mouse_commands[actionType()]->button2PressEvent(event->x, event->y);
          break;
        case 3:
          std::cout << "GLDrawingArea::on_button_press_event button3" << std::endl;
          m_mouse_commands[actionType()]->button3PressEvent(event->x, event->y);
          break;
        }
    }

  return false;
}

//------------------------------------------------------------------
bool MapEditorWindow::onRefreshKeyboard()
{
  // For all mapped keyboard key
  for (int key: m_allowed_keys)
    {
      // if the key was pressed then execute
      // the function associated to it.
      if (isKeyPressed(key))
        {
          m_keyboard_commands[key]->execute(*this);
        }
    }
  return true;
}

//------------------------------------------------------------------
void MapEditorWindow::actionReplaceCurrentMap()
{
  /*return*/ m_editor.dialogLoadMap(*this, false, true);
}

//------------------------------------------------------------------
void MapEditorWindow::actionMergeMap()
{
  /*return*/ m_editor.dialogLoadMap(*this, false, false);
}

//------------------------------------------------------------------
void MapEditorWindow::actionClearMap()
{
  m_editor.activeModel().resetSheets();
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
  /*return*/ m_editor.dialogLoadSheet(*this, true, false);
}

//------------------------------------------------------------------
void MapEditorWindow::actionReplaceSheet()
{
  /*return*/ m_editor.dialogLoadSheet(*this, false, true);
}

//------------------------------------------------------------------
void MapEditorWindow::actionClearSheet()
{
  m_editor.activeSheet().reset();
}

//------------------------------------------------------------------
void MapEditorWindow::splitView(Gtk::Orientation const orientation)
{
  MapPresenter* presenter = new MapPresenter(m_exception_dialog, m_editor.activeModelPtr());
  splitWidget(m_editor.activeView(), presenter->view(), orientation, Pack::First);
  m_editor.addPresenter(presenter);
}

//------------------------------------------------------------------
void MapEditorWindow::onOpenFileClicked()
{
  /*return*/ m_editor.dialogLoadMap(*this, true, false);
}

//------------------------------------------------------------------
void MapEditorWindow::onRecentFilesClicked()
{
  std::string filename = "NOT_YET_IMPLEMENTED";
  /*return*/ m_editor.doOpenSheet(filename, true, false);
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
  LOGF("%s", "saveAsMap not yet implemented");
  /*Gtk::FileChooserDialog dialog("Save a SimTaDyn map file",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_vbox.get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_shapefile = Gtk::FileFilter::create();
  filter_shapefile->set_name("Shape files"); // FIXME: should use LoaderManager informations to avoid double code
  filter_shapefile->add_pattern("*.shp");
  dialog.add_filter(filter_shapefile);

  auto filter_simtadyn = Gtk::FileFilter::create();
  filter_simtadyn->set_name("SimTaDyn files");
  filter_simtadyn->add_pattern("*.simtadyn");
  dialog.add_filter(filter_simtadyn);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      MapEditor::save(dialog.get_filename());
      }*/
}

//------------------------------------------------------------------
void MapEditorWindow::onSaveAsFileClicked()
{
  LOGF("%s", "dialogSaveAsMap not yet implemented");
  //return false;
  /*Gtk::MessageDialog dialog((Gtk::Window&) (*m_vbox.get_toplevel()),
                            "The document '" + map.m_name +
                            "' has been modified. Do you want to save it now ?",
                            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);

  int result = dialog.run();
  if (Gtk::RESPONSE_YES == result)
    {
      if (0 == doc->m_filename.compare(""))
        {
          return MapEditor::saveAs(doc);
        }
      else
        {
          return doc->save();
        }
    }
  else if (Gtk::RESPONSE_APPLY == result)
    {
      return MapEditor::saveAs(doc);
    }
  else // other button
    {
      if (closing)
        {
          doc->modified(false);
          return true;
        }
      return !doc->modified();
      }*/
}

//------------------------------------------------------------------
bool MapEditorWindow::onExit(GdkEventAny* /*event*/)
{
  //bool res = closeAll();
  //return !res;
  return false;
}

//------------------------------------------------------------------
void MapEditorWindow::onActionOnSelected(const ActionOn id)
{
  m_editor.setActionOn(id);
}

//------------------------------------------------------------------
void MapEditorWindow::onActionTypeSelected(const ActionType id)
{
  //LOGI("%s", "ActionTypeSelected %u", id);

  // http://transit.iut2.upmf-grenoble.fr/doc/gtkmm-3.0/reference/html/group__gdkmmEnums.html
  // Change the mouse cursor
  static Gdk::CursorType cursors[] =
    {
      [static_cast<uint32_t>(ActionType::Add)] = Gdk::PLUS,
      [static_cast<uint32_t>(ActionType::Remove)] = Gdk::PIRATE,
      [static_cast<uint32_t>(ActionType::Select)] = Gdk::HAND1,
      [static_cast<uint32_t>(ActionType::Move)] = Gdk::HAND2,
    };

  get_window()->set_cursor(Gdk::Cursor::create(cursors[static_cast<uint32_t>(id)]));
}
