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
