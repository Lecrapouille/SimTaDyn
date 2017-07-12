#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include "Names.hpp"
#  include "MapLoader.hpp"
#  include "ResourceManager.hpp"
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : public Gtk::HBox,
    protected MapLoader,
    public Singleton<MapEditor>
{
private:

  friend class Singleton<MapEditor>;

  //! \brief Private because of Singleton.
  MapEditor();

  //! \brief Private because of Singleton.
  virtual ~MapEditor();

protected:

  // ***********************************************************************************************
  //! \brief Implements the SimTaDynMap::ISimTaDynMapListener interface.
  // ***********************************************************************************************
  class SimTaDynMapListener: public SimTaDynMap::ISimTaDynMapListener
  {
  public:

    //! \brief
    SimTaDynMapListener(MapEditor &editor)
      : m_editor(editor)
    {
    };

    //! \brief Callback when the map changed: draw it.
    virtual void onChanged(SimTaDynMap* map) override
    {
      LOGI("MapEditor::SimTaDynMapListener has detected that Map %s has changed 0x%x",
           map->m_name.c_str(), map);
      if (nullptr == m_editor.m_drawing_area)
        return ;
      //FIXME m_editor.m_drawing_area->drawThat(*map);
    }

    MapEditor& m_editor;
  };

public:

  //! \brief Attach a the MVC view to this MVC controller.
  void attachView(GLDrawingArea& drawing_area)
  {
    m_drawing_area = &drawing_area;
    //m_drawing_area->attachController(this);
    m_vbox.pack_start(drawing_area);
  }

  SimTaDynMap *newMap()
  {
    LOGI("Creating a new SimTaDyn map");
    SimTaDynMap *map = new SimTaDynMap();
    map->addListener(m_listener);
    //FIXME m_drawing_area->attachModel(*map);
    m_maps.insert(map);
    m_current_map.set(map);
    return map;
  }

  SimTaDynMap* map()
  {
    return m_current_map.get();
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
    if (nullptr != m_current_map.get())
      {
        m_current_map.get()->clear();
      }
  }

  void closeMap();
  bool selectMap(const Key id);

  inline void saveMap()
  {
    // TODO
  }

  inline void saveAsMap()
  {
    // TODO
  }

  bool execMap();
  //void foo() { m_inspector.showCell(m_foo); ++m_foo; } //FIXME

protected:

  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap) override;
  void doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  void openDialog(const bool new_map, const bool reset_map);
  void add(const Key id, SimTaDynMap* map);
  void remove(const Key id);

public:

  //! \brief Models of the MVC design pattern
  ResourceManager<Key>   m_maps;
  //! \brief Current model of the MVC design pattern
  CurrentSimTaDynMap     m_current_map;
  //! \brief View implementation of the MVC
  GLDrawingArea         *m_drawing_area = nullptr;
  //!
  Gtk::MenuItem          m_menuitem[simtadyn::MaxMapMenuNames + 1];

protected:

  Gtk::Menu              m_menu[simtadyn::MaxMapMenuNames + 1];
  Gtk::ImageMenuItem     m_submenu[8];
  Gtk::Image             m_image[8];
  Gtk::SeparatorMenuItem m_menuseparator[2];
  Gtk::SeparatorToolItem m_separator[2];
  Gtk::Toolbar           m_toolbar;
  Inspector              m_inspector;
  Gtk::VBox              m_vbox;
  SimTaDynMapListener    m_listener;
};

#endif /* MAPEDITOR_HPP_ */
