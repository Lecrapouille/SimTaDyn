#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include <gtkmm.h>
#  include "SimTaDynGraphs.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class MapEditor
{
public:
  MapEditor();
  ~MapEditor();
  void open();

  Gtk::MenuItem       m_menuitem[2];
  Gtk::Menu           m_menu[2];
  Gtk::ImageMenuItem  m_submenu[4];
  Gtk::Image          m_image[2];
  Gtk::SeparatorMenuItem m_menuseparator[2];

protected:
  bool load(const std::string& filename, SimTaDynGraph& graph);

  RTreeNode* m_root; // FIXME: a attacher dans le graph
  SimTaDynGraph m_graph; // FIXME: a renommer en maps
};

#endif /* MAPEDITOR_HPP_ */
