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

#include "Windows.hpp"
#include "PathManager.hpp"

//------------------------------------------------------------------
MainWindow::MainWindow(Glib::RefPtr<Gtk::Application> application)
  : m_application(application),
    m_exception_dialog(*this)
{
  LOGI("Creating the SimTaDyn");

  set_default_size(1400, 800);
  set_position(Gtk::WIN_POS_CENTER);
  setTitleIcon("icons/SimTaDyn.png");
  populateHeaderBar();

  // Group buttons together
  group(0, m_open_file_button, m_recent_files_button);
  group(1, m_save_file_button, m_saveas_file_button);
  group(2, m_undo_button, m_redo_button);
  group(3, m_horizontal_split_button, m_vertical_split_button);

  //
  signal_delete_event().connect(sigc::mem_fun(this, &MainWindow::onExit));
}

//------------------------------------------------------------------
void MainWindow::group(uint8_t const id, Gtk::Button& left_button, Gtk::Button& right_button)
{
  Glib::RefPtr<Gtk::StyleContext> stylecontext;
  m_boxes[id].pack_start(left_button);
  m_boxes[id].pack_start(right_button);
  stylecontext = m_boxes[id].get_style_context();
  stylecontext->add_class("linked");
}

//------------------------------------------------------------------
static void setPersonalIcon(Gtk::Button& button, const Glib::ustring& icon_name)
{
  Gtk::Image* image = Gtk::manage(new Gtk::Image(PathManager::instance().expand("icons/" + icon_name + ".png")));
  image->property_use_fallback() = true;
  button.set_image(*image);
}

//------------------------------------------------------------------
void MainWindow::setTitle(const Glib::ustring& title)
{
  m_header_bar.set_title(title);
}

//------------------------------------------------------------------
void MainWindow::setSubtitle(const Glib::ustring& subtitle)
{
  m_header_bar.set_subtitle(subtitle);
}

//------------------------------------------------------------------
void MainWindow::setTitleIcon(std::string const &icon_name)
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
void MainWindow::populateHeaderBar()
{
  m_header_bar.set_show_close_button(true);
  m_header_bar.set_title (config::project_name);
  set_titlebar(m_header_bar);

  m_open_file_button.set_label("Open");
  m_recent_files_button.set_image_from_icon_name("pan-down-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  setPersonalIcon(m_horizontal_split_button, "split-horizontal");
  setPersonalIcon(m_vertical_split_button, "split-vertical");
  m_undo_button.set_image_from_icon_name("edit-undo-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_redo_button.set_image_from_icon_name("edit-redo-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_save_file_button.set_label("Save");
  m_saveas_file_button.set_image_from_icon_name("document-save-as-symbolic", Gtk::ICON_SIZE_BUTTON, true);
  m_menu_button.set_image_from_icon_name("open-menu-symbolic", Gtk::ICON_SIZE_BUTTON, true);

  m_header_bar.pack_start(m_menu_button);
  m_header_bar.pack_start(m_boxes[0]);
  m_header_bar.pack_start(m_boxes[3]);
  m_header_bar.pack_end(m_boxes[1]);
  m_header_bar.pack_end(m_boxes[2]);

  m_open_file_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onOpenFileClicked));
  m_recent_files_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onRecentFilesClicked));
  m_horizontal_split_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onHorizontalSplitClicked));
  m_vertical_split_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onVerticalSplitClicked));
  m_undo_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onUndoClicked));
  m_redo_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onRedoClicked));
  m_saveas_file_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onSaveAsFileClicked));
  m_save_file_button.signal_clicked().connect(sigc::mem_fun(*this, &MainWindow::onSaveFileClicked));
}
