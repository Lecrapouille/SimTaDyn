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
    KeyBoardHandler(*this, 10u) // FIXME 10u is mandatory
{
  populatePopovMenu();
  populateToolBar();

  m_hbox.pack_start(m_vbox);
  m_hbox.pack_start(m_toolbar, Gtk::PACK_SHRINK);

  //
  Gtk::Box* box = Gtk::make_managed<Gtk::Box>();

  MapPresenter* presenter = new MapPresenter(/*FIXME *this, createDummy*/);
  addPresenter(presenter);
  box->pack_start(presenter->view());
  m_vbox.pack_start(*box); // FIXME: m_vbox really needed ?

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
  MapPresenter* presenter = new MapPresenter(/*FIXME *this,*/ activeModelPtr());
  splitWidget(activeView(), presenter->view(), orientation, Pack::First);
  addPresenter(presenter);
}

//------------------------------------------------------------------
void MapEditorWindow::onOpenFileClicked()
{
  //TODO if (dialogLoadMap(true, false)) {}
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
  LOGF("saveAsMap not yet implemented");
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
  LOGF("dialogSaveAsMap not yet implemented");
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
bool MapEditorWindow::onRefreshKeyboard()
{
  // For all mapped keyboard key
  for (int key: m_allowed_keys)
    {
      // if the key was pressed then execute
      // the function associated to it.
      if (isKeyPressed(key))
        {
          m_commands[key]->execute(*this);
        }
    }
  return true;
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

//------------------------------------------------------------------
#if 0
template <class L>
bool MapEditor::dialogLoad(std::string const& title, std::string& filename)
{
  Gtk::FileChooserDialog dialog(title, Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(getRootWindow());
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Open the dialog window and set the SimTaDyn path as current
  // folder instead of using the "smart-current-folder" strategy
  // thiugh by GTK+ developpers.
  dialog.set_current_folder(config::data_path);

  // Fill filters for selecting type of file. Use the loader manager
  // for filling these filters because its knows all loaders which
  // know file extensions they can load.
  for (auto const& it: LoaderManager::instance().loaders<L>())
    {
      auto filter = Gtk::FileFilter::create();
      // loaders() return a map <<file extension>, <loader>>
      std::string extension("*." + it.first);
      filter->add_pattern(extension);
      filter->set_name(it.second->description() + " (" + extension + ')');
      dialog.add_filter(filter);
    }

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  bool res = (Gtk::RESPONSE_OK == dialog.run());
  filename = dialog.get_filename();

  return res;
}
#endif

void MoveCameraCommand::execute(MapEditorWindow& editor)
{
  std::cout << "Move Camera" << std::endl;
  editor.moveCamera(m_direction);
}

void SplitScreenCommand::execute(MapEditorWindow& editor)
{
  std::cout << "Split View" << std::endl;
  editor.splitView(m_oritentation);
}


  void MapEditor::bindKeyBoardCommands()
  {
    std::cout << "bindKeyBoardCommands" << std::endl;
    m_allowed_keys = { GDK_KEY_F1, GDK_KEY_F2, GDK_KEY_Page_Up, GDK_KEY_Page_Down, GDK_KEY_Left, GDK_KEY_Right };

    // FIXME: memory leak
    m_commands[GDK_KEY_F1] = new SplitScreenCommand(Gtk::Orientation::ORIENTATION_HORIZONTAL);
    m_commands[GDK_KEY_F2] = new SplitScreenCommand(Gtk::Orientation::ORIENTATION_VERTICAL);
    m_commands[GDK_KEY_Page_Up] = new MoveCameraCommand(CameraDirection::Up);
    m_commands[GDK_KEY_Page_Down] = new MoveCameraCommand(CameraDirection::Down);
    m_commands[GDK_KEY_Left] = new MoveCameraCommand(CameraDirection::Left);
    m_commands[GDK_KEY_Right] = new MoveCameraCommand(CameraDirection::Right);
  }

