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

#ifndef SIMTADYNWINDOW_HPP_
#  define SIMTADYNWINDOW_HPP_

#  include "ForthEditor.hpp"
#  include "MapEditor.hpp"
#  include "About.hpp"
#  include "ToggleButtons.hpp"

class ISimTaDynWindow: public Gtk::ApplicationWindow
{
public:

  ISimTaDynWindow(Glib::RefPtr<Gtk::Application> application);
  void setTitle(const Glib::ustring& title);

private:

  // Ajouter ici splitView mettre les methodes virtuels dans la class derivee

  void populateHeaderBar();
  void setTitleIcon(std::string const &icon_name);
  virtual void onOpenFileClicked() = 0;
  virtual void onRecentFilesClicked() = 0;
  virtual void onHorizontalSplitClicked() = 0;
  virtual void onVerticalSplitClicked() = 0;
  virtual void onUndoClicked() = 0;
  virtual void onRedoClicked() = 0;
  virtual void onSaveFileClicked() = 0;
  virtual void onSaveAsFileClicked() = 0;

protected:

  Gtk::MenuButton m_menu_button;
  Glib::RefPtr<Gtk::Application> m_application;

private:

  Gtk::HeaderBar m_header_bar;
  Gtk::Button    m_open_file_button;
  Gtk::Button    m_recent_files_button;
  Gtk::Button    m_horizontal_split_button;
  Gtk::Button    m_vertical_split_button;
  Gtk::Button    m_undo_button;
  Gtk::Button    m_redo_button;
  Gtk::Button    m_save_file_button;
  Gtk::Button    m_saveas_file_button;
};




class SimTaDynWindow: public Gtk::Window
{
public:

  //! \brief Constructor.
  SimTaDynWindow(ForthEditor& forth_editor,
                 MapEditor& map_editor);

protected:

  bool onExitClicked(GdkEventAny* event);
  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

private:

  //void setTitleIcon(std::string const &icon_name);

protected:

  ForthEditor&                 m_forth_editor;
  MapEditor&                   m_map_editor;
  Gtk::HPaned                  m_hpaned;
  Gtk::VBox                    m_box;
  Gtk::MenuBar                 m_menubar;
  Gtk::Menu                    m_menu[simtadyn::MaxMapMenuNames +
                                      simtadyn::MaxForthMenuNames +
                                      simtadyn::MaxGeneralMenuNames + 1];
  Gtk::MenuItem                m_menuitem[simtadyn::MaxMapMenuNames +
                                          simtadyn::MaxForthMenuNames +
                                          simtadyn::MaxGeneralMenuNames + 1];
  Gtk::ImageMenuItem           m_submenu[1];
  Gtk::Image                   m_image[1];
  std::vector<Gtk::ToolButton> m_toolbuttons; // FIXME resize
  About                        m_about;
};

#endif /* SIMTADYNWINDOW_HPP_ */
