#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include <gtkmm.h>
#  include "MapLoader.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
class MapEditor: protected MapLoader
{
public:
  MapEditor();
  ~MapEditor();
  inline void newEmptyMap()
  {
    m_maps.push_back(new SimTaDynMap());
    m_current_map = m_maps.size() - 1;
  }
  inline void openMap()
  {
    open(true, false);
  }
  inline void addMap()
  {
    open(false, false);
  }
  inline void replaceMap()
  {
    open(false, true);
  }
  inline void clearMap()
  {
    SimTaDynMap* map = MapEditor::map();
    if (nullptr != map)
      {
        map->m_graph.reset();
      }
  }
  /*Gtk::ToolButton *addMapScriptButon(const Gtk::BuiltinStockID icon,
                                     const std::string &script,
                                     const std::string &help);
  */
  void saveMap() {};
  void saveAsMap() {};
  //void saveAll();
  //void find();


  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];
  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menuseparator[2];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar           m_toolbar;
protected:
  void open(const bool new_map, const bool reset_map);

  //! \brief Return the current map document
  SimTaDynMap* map();
  //SimTaDynMap* document(const uint32_t i);

  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap) override;

  std::vector<SimTaDynMap*> m_maps;
  int m_current_map;
};

#endif /* MAPEDITOR_HPP_ */
