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

#include "SimWindow.hpp"
#include "PathManager.hpp"

//------------------------------------------------------------------
About::About()
{
  // About window
  set_program_name(config::project_name);
  set_version(std::to_string(config::major_version) + '.' + std::to_string(config::minor_version));
  set_copyright("Copyright ©2004, 2017-2019 Quentin Quadrat");
  set_comments("A GIS in a spreadsheet");
  set_license_type(Gtk::LICENSE_GPL_3_0);
  set_wrap_license(false);
  set_website("https://github.com/Lecrapouille/SimTaDyn");
  set_website_label("Visit " + config::project_name + " github site");
  set_authors({"Quentin Quadrat <lecrapouille@gmail.com>"});

  try
    {
      set_logo(Gdk::Pixbuf::create_from_file(PathManager::instance().expand("icons/SimTaDyn-logo.jpg")));
    }
  catch (...)
    {
      LOGW("SimTaDyn logo not found");
    }

  signal_response().connect([&](const int&) { hide(); });
}

//------------------------------------------------------------------
ISimTaDynWindow::ISimTaDynWindow(Glib::RefPtr<Gtk::Application> application)
  : m_application(application),
    m_popup_exception(*this)
{
  LOGI("Creating the SimTaDyn");

  set_default_size(1400, 800);
  set_position(Gtk::WIN_POS_CENTER);
  setTitleIcon("icons/SimTaDyn.png");
  populateHeaderBar();

  signal_delete_event().connect(sigc::mem_fun(this, &ISimTaDynWindow::onExit));
}

//------------------------------------------------------------------
void ISimTaDynWindow::setTitle(const Glib::ustring& title)
{
  m_header_bar.set_title(title);
}

//------------------------------------------------------------------
void ISimTaDynWindow::setSubtitle(const Glib::ustring& subtitle)
{
  m_header_bar.set_subtitle(subtitle);
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
  set_titlebar(m_header_bar);

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
