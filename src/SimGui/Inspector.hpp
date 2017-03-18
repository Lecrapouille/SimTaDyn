#ifndef INSPECTOR_HPP_
#  define INSPECTOR_HPP_

#  include <gtkmm.h>
#  include "Types.hpp"

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
