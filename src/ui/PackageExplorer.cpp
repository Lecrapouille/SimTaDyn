//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "PackageExplorer.hpp"

PackageExplorer::PackageExplorer()
{
  // Preload some icons
  try
    {
      Glib::RefPtr<Gtk::IconTheme> icon_theme = Gtk::IconTheme::get_for_screen(m_tree_view.get_screen());
      m_icon_folder = icon_theme->load_icon("folder", 16);
      m_icon_document = icon_theme->load_icon("document", 16);
      m_icon_status_ok = icon_theme->load_icon("starred", 16);
      m_icon_status_warning = icon_theme->load_icon("important", 16);
      m_icon_status_ko = icon_theme->load_icon("error", 16);
    }
  catch (Gtk::IconThemeError const &e)
    {
      LOGE("Failed loading GTK+ icon theme. Reason '%s'", e.what().c_str());
    }

  // Create the Tree model
  m_tree_store = Gtk::TreeStore::create(m_columns);
  m_tree_view.set_model(m_tree_store);

  // Empty folders // TODO a appeller dans loadPak() quand on charge une nouvelle carte
  addFolder(Maps, "Maps");
  addFolder(Scripts, "Scripts");
  addFolder(Resources, "Resources");
  addIndex();

  // Add the TreeView's view columns
  m_tree_view.append_column("T", m_columns.m_icon_type);
  m_tree_view.append_column("F", m_columns.m_filename);
  m_tree_view.append_column("S", m_columns.m_icon_status);
  //m_tree_view.append_column("?", m_columns.m_is_folder);
  m_tree_view.set_enable_tree_lines();

  // Signals
  m_tree_view.signal_button_press_event().connect(sigc::mem_fun(*this, &PackageExplorer::on_mytreeview_button_press_event));
}

PackageExplorer::~PackageExplorer()
{
}

Gtk::TreeModel::Row const *PackageExplorer::addDocument(PackageExplorer::Category const type, Glib::ustring const &filename)
{
  Gtk::TreeModel::Row childrow = *(m_tree_store->append(m_folders[type].children()));
  childrow[m_columns.m_icon_type] = m_icon_document;
  childrow[m_columns.m_filename] = filename;
  childrow[m_columns.m_icon_status] = m_icon_status_ok;
  childrow[m_columns.m_is_folder] = false;
  return nullptr;
}

// FIXME: ne pas appeller deux fois
void PackageExplorer::addIndex()
{
  Gtk::TreeModel::Row row = *(m_tree_store->prepend());
  row[m_columns.m_icon_type] = m_icon_document;
  row[m_columns.m_filename] = "Index";
  row[m_columns.m_is_folder] = false;
  m_folders[Index] = row;
}

void PackageExplorer::addFolder(PackageExplorer::Category const type, Glib::ustring const &name)
{
  Gtk::TreeModel::Row row = *(m_tree_store->append());
  row[m_columns.m_icon_type] = m_icon_folder;
  row[m_columns.m_filename] = name;
  row[m_columns.m_is_folder] = true;
  m_folders[type] = row;
}

bool PackageExplorer::on_mytreeview_button_press_event(GdkEventButton *ev)
{
  if (ev->type == GDK_2BUTTON_PRESS)
    {
      Glib::RefPtr<Gtk::TreeSelection> selection = m_tree_view.get_selection();
      Gtk::TreeModel::iterator selectedRow = selection->get_selected();
      Gtk::TreeModel::Row row = *selectedRow;
      if (!row.get_value(m_columns.m_is_folder))
        {
          Glib::ustring name = row.get_value(m_columns.m_filename);
          std::cout << "pressed " << name << std::endl;
          // TODO appeller Loader
        }
    }
  return false;
}

void PackageExplorer::onLoadFailure(std::string const& filename, std::string const& msg)
{
  std::cout << "PackageExplorer::onLoadFailure '" << filename
            << "' Reason : '" << msg << "'" << std::endl;
}

void PackageExplorer::onLoadSucess(std::string const& filename)
{
  std::cout << "PackageExplorer::onLoadSucess '" << filename << "'" << std::endl;
  addMap(filename);
}

void PackageExplorer::onSaveFailure(std::string const& filename, std::string const& msg)
{
  std::cout << "PackageExplorer::onSaveFailure '" << filename
            << "' Reason : '" << msg << "'" << std::endl;
}

void PackageExplorer::onSaveSucess(std::string const& filename)
{
  std::cout << "PackageExplorer::onSaveSucess '" << filename << "'" << std::endl;
}
