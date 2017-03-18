#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include "MapLoader.hpp"
#  include "ResourceManager.hpp"
#  include "Namespaces.hpp"
#  include <gtkmm.h>

// *************************************************************************************************
//
// *************************************************************************************************
class MapEditor: protected MapLoader, public Singleton<MapEditor>
{
  friend class Singleton<MapEditor>;

public:

  inline void newEmptyMap()
  {
    SimTaDynMap *m_current_map = new SimTaDynMap();
    m_maps.insert(m_current_map);
  }
  inline void openMap()
  {
    openDialog(true, false);
  }
  inline void addMap()
  {
    openDialog(false, false);
  }
  inline void replaceMap()
  {
    openDialog(false, true);
  }
  inline void clearMap()
  {
    if (nullptr != m_current_map)
      {
        m_current_map->m_graph.BasicGraph::reset();
      }
  }
  inline void saveMap()
  {
    // TODO
  }
  inline void saveAsMap()
  {
    // TODO
  }
  bool execMap();
  void execMap2() { execMap(); } // FIXME temporary
  void closeMap();
  bool selectMap(const Key id);

  SimTaDynMap* map()
  {
    return m_current_map;
  }

  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];
  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menuseparator[2];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar           m_toolbar;

protected:

  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap) override;
  void doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  void openDialog(const bool new_map, const bool reset_map);
  void add(const Key id, SimTaDynMap* map);
  void remove(const Key id);
  //SimTaDynMap* map();

  ResourceManager<Key> m_maps;
  SimTaDynMap* m_current_map = nullptr;

private:

  MapEditor();
  virtual ~MapEditor();
};

#endif /* MAPEDITOR_HPP_ */
