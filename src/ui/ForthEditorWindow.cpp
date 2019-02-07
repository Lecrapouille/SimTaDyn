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

#include "ForthEditorWindow.hpp"

// FIXME: temporary
#define FORTH_NOTEBOOK_CODE 0
#define FORTH_NOTEBOOK_RES 1
#define FORTH_TOOLBAR_PLUGINS 0
#define FORTH_TOOLBAR_CMDS 1

ForthEditorWindow::ForthEditorWindow(Glib::RefPtr<Gtk::Application> application)
  : ISimTaDynWindow(application)
{
  populatePopovMenu();
  populateToolBar();

  m_statusbar.push("Welcome to SimTaDyn !");

  add(m_hbox);
  m_hbox.pack_start(m_toolbars[FORTH_TOOLBAR_PLUGINS], Gtk::PACK_SHRINK);
  m_hbox.pack_start(m_vbox);
  m_vbox.pack_start(m_notebooks[FORTH_NOTEBOOK_CODE], Gtk::PACK_EXPAND_WIDGET);
  m_vbox.pack_start(m_toolbars[FORTH_TOOLBAR_CMDS], Gtk::PACK_SHRINK);
  m_vbox.pack_start(m_statusbar, Gtk::PACK_SHRINK);
  m_vbox.pack_start(m_notebooks[FORTH_NOTEBOOK_RES], Gtk::PACK_EXPAND_WIDGET);

  show_all();
}

//------------------------------------------------------------------
// Forth toolbar (horizontal)
void ForthEditorWindow::populateToolBar()
{
  // Horizontal toolbar: Forth commands
  {
    Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_CMDS];
    toolbar.set_property("orientation", Gtk::ORIENTATION_HORIZONTAL);
    toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    // Forth exec button
    {
      Gtk::ToolButton *button = Gtk::make_managed<Gtk::ToolButton>();
      button->set_label("Exec");
      button->set_stock_id(Gtk::Stock::EXECUTE);
      button->set_tooltip_text("Run Forth script");
      toolbar.append(*button, sigc::mem_fun(*this, &ForthEditorWindow::execForthScript));
      toolbar.append(m_separator[1]);
    }
  }

  // Vertical toolbar: Forth plugins
  {
    Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_PLUGINS];
    toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
    toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    // FIXME temporary plugin
    {
      addForthButton(Gtk::Stock::EXECUTE, "42 42 + .", "Plugin example");
    }
  }
}

// **************************************************************
// FIXME const Cell16 ForthToken)
// **************************************************************
// FIXME: si pile vide ou pas le bon nombre d'elements alors fenetre popup qui demande les param
// FIXME: ajouter le postip avec la definiton du mot "WORD ( n1 n2 -- n3 n4 )"
// FIXME ne pas autoriser a compiler
// **************************************************************

Gtk::ToolButton& ForthEditorWindow::addForthButton(const Gtk::BuiltinStockID icon,
                                                   const std::string &script,
                                                   const std::string &help)
{
  Gtk::ToolButton *button = Gtk::make_managed<Gtk::ToolButton>();

  if (nullptr != button)
    {
      Gtk::Toolbar& toolbar = m_toolbars[FORTH_TOOLBAR_PLUGINS];

      button->set_label(script);
      button->set_stock_id(icon);
      button->set_tooltip_text(help);
      toolbar.append(*button, sigc::bind<Gtk::ToolButton*>
        (sigc::mem_fun(*this, &ForthEditorWindow::onForthButtonClicked), button));
      toolbar.show_all_children();
    }
  else
    {
      //FIXME Gtk::MessageDialog dialog(*this, "Failed creating a Forth button");
      //dialog.run();
    }
  return *button;
}

//------------------------------------------------------------------
void ForthEditorWindow::onForthButtonClicked(Gtk::ToolButton* button)
{
}

//------------------------------------------------------------------
void ForthEditorWindow::populatePopovMenu()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onOpenFileClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onRecentFilesClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onHorizontalSplitClicked()
{
  //splitView(Gtk::Orientation::ORIENTATION_HORIZONTAL);
}

//------------------------------------------------------------------
void ForthEditorWindow::onVerticalSplitClicked()
{
  //splitView(Gtk::Orientation::ORIENTATION_VERTICAL);
}

//------------------------------------------------------------------
void ForthEditorWindow::onUndoClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onRedoClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onSaveFileClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::onSaveAsFileClicked()
{
}

//------------------------------------------------------------------
void ForthEditorWindow::execForthScript()
{
}
