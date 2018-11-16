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

#ifndef INSPECTOR_HPP_
#  define INSPECTOR_HPP_

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wold-style-cast"
#pragma GCC diagnostic ignored "-Wredundant-decls"
#pragma GCC diagnostic ignored "-Wsign-conversion"
#  include <gtkmm.h>
#pragma GCC diagnostic pop

class Inspector
{
public:

  Inspector();
  void showCell(const Key nodeID);
  void showEmpty();

protected:

  class ModelColumns : public Gtk::TreeModelColumnRecord
  {
  public:
    ModelColumns()
    {
      add(m_word);
      add(m_token);
    }

    Gtk::TreeModelColumn<Glib::ustring> m_word;
    Gtk::TreeModelColumn<Glib::ustring> m_token;
  };

  Glib::RefPtr<Gtk::ListStore> m_ref_tree_model;

public:

  Gtk::TreeView m_tree;
  ModelColumns m_columns;
  Gtk::ScrolledWindow    m_scrolledwindow;
};

#endif /* INSPECTOR_HPP_ */
