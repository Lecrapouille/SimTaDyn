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

#ifndef FORTH_EDITOR_WINDOW_HPP
#  define FORTH_EDITOR_WINDOW_HPP

#  include "SimWindow.hpp"
#  include "ForthEditor.hpp"

class ForthEditorWindow : public ISimTaDynWindow
{
public:

  ForthEditorWindow(SimForth& forth, Glib::RefPtr<Gtk::Application> application);
  Gtk::ToolButton& addForthButton(Gtk::BuiltinStockID const icon,
                                  std::string const& script,
                                  std::string const& help);
  void addForthActionMenu(Glib::ustring const& icon_name,
                          std::string const& script_name,
                          std::string const& script_code,
                          std::string const& help);

private:

  void populatePopovMenu();
  void populateToolBar();
  //void splitView(Gtk::Orientation const orientation);
  //GLDrawingArea* createView();
  //GLDrawingArea& currentView();

  virtual void onOpenFileClicked() override;
  virtual void onRecentFilesClicked() override;
  virtual void onHorizontalSplitClicked() override;
  virtual void onVerticalSplitClicked() override;
  virtual void onUndoClicked() override;
  virtual void onRedoClicked() override;
  virtual void onSaveFileClicked() override;
  virtual void onSaveAsFileClicked() override;
  virtual bool onExit(GdkEventAny* event) override;
  void onForthActionMenuClicked(std::string const& script_code,
                                std::string const& script_name);

private:

  ForthEditor  m_forth_editor;
  Gtk::Popover m_menu_popov;
  Glib::RefPtr<Gio::Menu> m_submenu_forth_plugins;
  /* FIXME static */ About   m_about;
};


#endif // FORTH_EDITOR_WINDOW_HPP
