#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include <gtkmm.h>
#  include "MapLoader.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class MapEditor : protected MapLoader
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
  std::vector<SimTaDynMap*> m_maps;

protected:
  virtual bool load(const std::string& filename, SimTaDynMap *oldmap = nullptr) override;

};

#endif /* MAPEDITOR_HPP_ */
