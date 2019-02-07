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

#include "SimTaDynWindow.hpp"
#include "PathManager.hpp"
#include "Gtkmm.tpp"

//------------------------------------------------------------------
ISimTaDynWindow::ISimTaDynWindow(Glib::RefPtr<Gtk::Application> application)
  : m_application(application)
{
  LOGI("Creating the SimTaDyn");
  set_default_size(1400, 800);
  set_position(Gtk::WIN_POS_CENTER);
  setTitleIcon("icons/SimTaDyn.png");
  populateHeaderBar();
}

//------------------------------------------------------------------
void ISimTaDynWindow::setTitle(const Glib::ustring& title)
{
  m_header_bar.set_subtitle(title);
}

//------------------------------------------------------------------
void ISimTaDynWindow::setTitleIcon(std::string const &icon_name)
{
  std::pair<std::string, bool> res = PathManager::instance().find(icon_name);

  if (res.second)
    {
      set_icon_from_file(res.first);
    }
  else
    {
      LOGW("SimTaDynWindow: Icon '%s' does not exist\n", icon_name.c_str());
    }
}

//------------------------------------------------------------------
void ISimTaDynWindow::populateHeaderBar()
{
  m_header_bar.set_show_close_button(true);
  m_header_bar.set_title (config::project_name);
  this->set_titlebar(m_header_bar);

  m_open_file_button.set_label("Open");
  m_recent_files_button.set_image_from_icon_name("pan-down-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_horizontal_split_button.set_image_from_icon_name("go-last-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_vertical_split_button.set_image_from_icon_name("go-bottom-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_undo_button.set_image_from_icon_name("edit-undo-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_redo_button.set_image_from_icon_name("edit-redo-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_save_file_button.set_label("Save");
  m_saveas_file_button.set_image_from_icon_name("document-save-as-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_menu_button.set_image_from_icon_name("open-menu-symbolic", Gtk::ICON_SIZE_BUTTON, true);

  m_header_bar.pack_start(m_menu_button);
  m_header_bar.pack_start(m_open_file_button);
  m_header_bar.pack_start(m_recent_files_button);
  m_header_bar.pack_start(m_horizontal_split_button);
  m_header_bar.pack_start(m_vertical_split_button);
  m_header_bar.pack_start(m_undo_button);
  m_header_bar.pack_start(m_redo_button);
  m_header_bar.pack_end(m_saveas_file_button);
  m_header_bar.pack_end(m_save_file_button);

  m_open_file_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onOpenFileClicked));
  m_recent_files_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onRecentFilesClicked));
  m_horizontal_split_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onHorizontalSplitClicked));
  m_vertical_split_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onVerticalSplitClicked));
  m_undo_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onUndoClicked));
  m_redo_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onRedoClicked));
  m_saveas_file_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onSaveAsFileClicked));
  m_save_file_button.signal_clicked().connect(sigc::mem_fun(*this, &ISimTaDynWindow::onSaveFileClicked));
}




// *************************************************************************************************
// SimTaDyn main window
// *************************************************************************************************
SimTaDynWindow::SimTaDynWindow(ForthEditor& forth_editor,
                               MapEditor& map_editor)
  : Gtk::Window(),
    m_forth_editor(forth_editor),
    m_map_editor(map_editor)
{


  // Main window
  {
    set_title(config::project_name);
    set_default_size(1400, 800);
    set_position(Gtk::WIN_POS_CENTER);
    //setTitleIcon("icons/SimTaDyn.png");

    // Connect signals
    add_events(Gdk::KEY_RELEASE_MASK);
    signal_key_press_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyPressed));
    signal_key_release_event().connect_notify(sigc::mem_fun(*this, &SimTaDynWindow::onKeyReleased));
    signal_delete_event().connect(sigc::mem_fun(this, &SimTaDynWindow::onExitClicked));
  }

  // Menus:
  // * _Map: Import/export/save/load/... geographic maps.
  // * _Forth: Import/export/save/load/... Forth scripts
  // * _Text: search, replace, goto line
  // * _Plugins: TBD: Let the user to add an menu calling it's on fprth scripts.
  // * _Help: TBD: add About/help/interactive tutorials
  {
    // Menu '_Map'
    m_menubar.append(m_map_editor.m_menuitem[simtadyn::MapMenu]);

    // Menu '_Forth'
    m_menubar.append(m_forth_editor.m_menuitem[simtadyn::ForthMenu]);
    m_menubar.append(m_forth_editor.m_menuitem[simtadyn::TextMenu]);
    m_menubar.append(m_forth_editor.m_menuitem[simtadyn::PlugginsMenu]);
    m_forth_editor.addPluggin("text-x-generic-template", "41 1 + . CR", "test");

    // Menu '_Help'
    // TODO: submenus Tuto, Help, Example
    m_menuitem[simtadyn::HelpMenu].set_label("_Help");
    m_menuitem[simtadyn::HelpMenu].set_use_underline(true);
    m_menubar.append(m_menuitem[simtadyn::HelpMenu]);
    m_menuitem[simtadyn::HelpMenu].set_submenu(m_menu[simtadyn::HelpMenu]);

    // About
    m_submenu[0].set_label("About");
    m_image[0].set_from_icon_name("gtk-about", Gtk::ICON_SIZE_MENU);
    m_submenu[0].set_image(m_image[0]);
    m_submenu[0].signal_activate().connect([this](){ m_about.show(); });
    m_menu[simtadyn::HelpMenu].append(m_submenu[0]);
  }

  // Split verticaly the main window:
  // -- On the left: the map editor
  // -- On the right: the menubar and Forth editor
  {
    // Left:
    m_hpaned.pack1(m_map_editor.widget());

    // Right:
    m_box.pack_start(m_menubar, Gtk::PACK_SHRINK);
    m_box.pack_start(m_forth_editor.m_vpaned);
    m_hpaned.pack2(m_box);
    m_box.pack_start(map_editor.m_map_explorer.widget());

    //
    add(m_hpaned);
    m_hpaned.set_position(800);

    m_about.set_transient_for(*this);
  }

  show_all_children();
}

// *************************************************************************************************
//! \return a boolean to allow destroying the main windows or not in the case some documents have
//! not been saved.
// *************************************************************************************************
bool SimTaDynWindow::onExitClicked(GdkEventAny*)
{
  // FIXME: do the same for MapEditor
  bool res = m_forth_editor.closeAll();
  return !res;
}



// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::onKeyPressed(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
    case GDK_KEY_Escape:
      break;
    case GDK_KEY_F1:
      m_map_editor.splitView(Gtk::Orientation::ORIENTATION_HORIZONTAL);
      break;
    case GDK_KEY_F2:
      m_map_editor.splitView(Gtk::Orientation::ORIENTATION_VERTICAL);
      break;
#if 0
    case GDK_KEY_F11:
      {
        //SimTaDynContext& simtadyn = SimTaDynContext::getInstance();
        //zoomFitPage(simtadyn.m_graph);
      }
      break;
    case GDK_KEY_Page_Up:
      m_map_editor.keyPressed(GLDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_map_editor.keyPressed(GLDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_map_editor.keyPressed(GLDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_map_editor.keyPressed(GLDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_map_editor.keyPressed(GLDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_map_editor.keyPressed(GLDrawingArea::Left);
      break;
#endif
    default:
      break;
    }
  m_forth_editor.autoCompleteWord(evenement->keyval);
}

// *************************************************************************************************
//
// *************************************************************************************************
void SimTaDynWindow::onKeyReleased(GdkEventKey* evenement)
{
  switch (evenement->keyval)
    {
#if 0
    case GDK_KEY_Page_Up:
      m_map_editor.keyReleased(GLDrawingArea::Forward);
      break;
    case GDK_KEY_Page_Down:
      m_map_editor.keyReleased(GLDrawingArea::Backward);
      break;
    case GDK_KEY_Up:
      m_map_editor.keyReleased(GLDrawingArea::Up);
      break;
    case GDK_KEY_Down:
      m_map_editor.keyReleased(GLDrawingArea::Down);
      break;
    case GDK_KEY_Right:
      m_map_editor.keyReleased(GLDrawingArea::Right);
      break;
    case GDK_KEY_Left:
      m_map_editor.keyReleased(GLDrawingArea::Left);
      break;
#endif
    default:
      break;
    }
}
