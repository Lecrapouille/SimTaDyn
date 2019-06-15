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

#ifndef WINDOWS_HPP
#  define WINDOWS_HPP

#  include "Dialogs.hpp"
#  include "Config.hpp"
#  include "Logger.hpp"

// *************************************************************************************************
//! \brief Facade class for creating windows with the same look
//! (MapEditor, ForthEditor).
// *************************************************************************************************
class MainWindow :
  public Gtk::ApplicationWindow
{
public:

  MainWindow(Glib::RefPtr<Gtk::Application> application);
  void setTitle(Glib::ustring const& title);
  void setSubtitle(Glib::ustring const& subtitle);

private:

  // TODO Ajouter ici splitView mettre les methodes virtuels dans la
  // class derivee

  void populateHeaderBar();
  void setTitleIcon(std::string const& icon_name);
  virtual void onOpenFileClicked() = 0;
  virtual void onRecentFilesClicked() = 0;
  virtual void onHorizontalSplitClicked() = 0;
  virtual void onVerticalSplitClicked() = 0;
  virtual void onUndoClicked() = 0;
  virtual void onRedoClicked() = 0;
  virtual void onSaveFileClicked() = 0;
  virtual void onSaveAsFileClicked() = 0;
  virtual bool onExit(GdkEventAny* event) = 0;
  void group(uint8_t const id, Gtk::Button& left, Gtk::Button& right);

protected:

  Glib::RefPtr<Gtk::Application> m_application;
  ExceptionDialog                m_exception_dialog;
  Gtk::MenuButton                m_menu_button;

private:

  Gtk::HeaderBar  m_header_bar;
  Gtk::Button     m_open_file_button;
  Gtk::MenuButton m_recent_files_button;
  Gtk::Button     m_horizontal_split_button;
  Gtk::Button     m_vertical_split_button;
  Gtk::Button     m_undo_button;
  Gtk::Button     m_redo_button;
  Gtk::Button     m_save_file_button;
  Gtk::Button     m_saveas_file_button;
  Gtk::HBox       m_boxes[4];
};

#endif // WINDOWS_HPP
