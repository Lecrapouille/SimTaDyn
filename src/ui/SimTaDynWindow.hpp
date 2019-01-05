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
#  include "SimTaDynMapExplorer.hpp"

class SimTaDynWindow: public Gtk::Window
{
public:

  //! \brief Constructor.
  SimTaDynWindow();

protected:

  bool onExitClicked(GdkEventAny* event);

  //! \brief Initialize the OpenGL context
  inline void onCreate()
  {
    m_drawing_area.onCreate();
  }
  //! \brief Clean up
  inline void onRelease()
  {
    m_drawing_area.onRelease();
  }
  //! \brief Draw the scene
  inline bool onRender(const Glib::RefPtr<Gdk::GLContext>& /* context */)
  {
    return m_drawing_area.onRender();
  }

  void onKeyPressed(GdkEventKey* evenement);
  void onKeyReleased(GdkEventKey* evenement);

private:

  void setTitleIcon(std::string const &icon_name);

protected:

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

  Gtk::AboutDialog             m_about;
  SimTaDynMapExplorer          m_map_explorer;
  GLDrawingArea                m_drawing_area;
  FindWindow                  *m_findwin = nullptr;
  ReplaceWindow               *m_replacewin = nullptr;
  GotoLineWindow              *m_gotolinewindow = nullptr; // FIXME nom:  ajouter des _
  // SceneNodeEditor             *m_node_editor = nullptr;
};

#endif /* SIMTADYNWINDOW_HPP_ */
