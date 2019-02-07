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

#ifndef FORTHEDITORWINDOW_HPP
#  define FORTHEDITORWINDOW_HPP

#  include "SimTaDynWindow.hpp"

class ForthEditorWindow: public ISimTaDynWindow
{
public:

  ForthEditorWindow(Glib::RefPtr<Gtk::Application> application);
  Gtk::ToolButton& addForthButton(const Gtk::BuiltinStockID icon,
                                  const std::string &script,
                                  const std::string &help);

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

  void execForthScript();
  void onForthButtonClicked(Gtk::ToolButton* button);

private:

  Gtk::HBox              m_hbox;
  Gtk::VBox              m_vbox;
  Gtk::Notebook          m_notebooks[2];
  Gtk::Toolbar           m_toolbars[2];
  Gtk::Statusbar         m_statusbar;
  Gtk::SeparatorToolItem m_separator[2];
  //Gtk::TextView          m_results;
  //Gtk::TextView          m_history;
  //Gtk::TextView          m_messages;
};


#endif
