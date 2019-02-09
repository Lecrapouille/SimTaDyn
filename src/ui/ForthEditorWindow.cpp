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

ForthEditorWindow::ForthEditorWindow(SimForth& forth, Glib::RefPtr<Gtk::Application> application)
  : ISimTaDynWindow(application),
    m_forth_editor(forth)
{
  populatePopovMenu();
  populateToolBar();

  m_forth_editor.statusBarSays("Welcome to SimTaDyn !");
  add(m_forth_editor.widget());
  show_all();
}

//------------------------------------------------------------------
// Forth toolbar (horizontal)
void ForthEditorWindow::populateToolBar()
{
  //FIXME
#if 0
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
#endif
}

//------------------------------------------------------------------
void ForthEditorWindow::addForthActionMenu(Glib::ustring const& /*icon_name*/,
                                           std::string const& script_name,
                                           std::string const& script_code,
                                           std::string const& /*help*/)
{
  //FIXME: how to insert help as tooltip ?
  m_submenu_forth_plugins->append(script_name, "win.script-" + script_name);
  add_action("script-" + script_name, sigc::bind<std::string const&, std::string const&>
             (sigc::mem_fun(*this, &ForthEditorWindow::onForthActionMenuClicked),
              script_code, script_name));
}

//------------------------------------------------------------------
Gtk::ToolButton& ForthEditorWindow::addForthButton(Gtk::BuiltinStockID const icon,
                                                   std::string const& script,
                                                   std::string const& help)
{
  return m_forth_editor.addForthButton(icon, script, help);
}

//------------------------------------------------------------------
void ForthEditorWindow::populatePopovMenu()
{
  Glib::RefPtr<Gio::Menu> menu = m_forth_editor.populatePopovMenu(*this);
  m_submenu_forth_plugins = Gio::Menu::create();
  menu->append_submenu("Forth Plugins", m_submenu_forth_plugins);

  menu->append("About", "win.about");
  add_action("about", sigc::mem_fun(m_about, &About::show));

  addForthActionMenu("a", "jjhj", "broken", "help");

  m_menu_button.set_popover(m_menu_popov);
  m_menu_button.set_menu_model(menu);
  m_menu_popov.set_size_request(-1, -1);
}

//------------------------------------------------------------------
void ForthEditorWindow::onOpenFileClicked()
{
  m_forth_editor.open();
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
  m_forth_editor.save();
}

//------------------------------------------------------------------
void ForthEditorWindow::onSaveAsFileClicked()
{
  m_forth_editor.saveAs();
}

//------------------------------------------------------------------
bool ForthEditorWindow::onExit(GdkEventAny* /*event*/)
{
  bool res = m_forth_editor.closeAll();
  return !res;
}

//------------------------------------------------------------------
void ForthEditorWindow::onForthActionMenuClicked(std::string const& script_code,
                                                 std::string const& script_name)
{
  if (m_forth_editor.interpreteScript(script_code, script_name))
    {
      // In case of failure open the code source
      // FIXME: quand on sauvegarde ne pas stocker dans un fichier mais dans le menu
      TextDocument *doc = m_forth_editor.addTab(script_name);
      doc->clear();
      doc->appendText(script_code);
    }
}
