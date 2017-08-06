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

#include "Inspector.hpp"
#include "MapEditor.hpp"

Inspector::Inspector()
{
  m_scrolledwindow.add(m_tree);
  m_ref_tree_model = Gtk::ListStore::create(m_columns);
  m_tree.set_model(m_ref_tree_model);

  m_tree.append_column("Word", m_columns.m_word);
  m_tree.append_column("Token", m_columns.m_token);
  m_tree.set_headers_visible(false);
  showEmpty();
}

void Inspector::showEmpty()
{
  m_ref_tree_model->clear();

  Gtk::TreeModel::Row row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Name";
  row[m_columns.m_token] = "";

  row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Forth";
  row[m_columns.m_token] = "";

  row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Cost";
  row[m_columns.m_token] = "";
}

void Inspector::showCell(const Key nodeID)
{
  SimTaDynMap* map = MapEditor::instance().map();
  if (nullptr == map)
    return ;

  if (!map->m_graph.hasNode(nodeID))
    return ;
  SimTaDynNode& node = map->m_graph.getNode(nodeID);

  m_ref_tree_model->clear();

  Gtk::TreeModel::Row row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Name";
  row[m_columns.m_token] = node.name();

  row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Forth";
  row[m_columns.m_token] = node.forthWord();

  row = *(m_ref_tree_model->append());
  row[m_columns.m_word] = "Cost";
  row[m_columns.m_token] = std::to_string(node.m_cost);
}
