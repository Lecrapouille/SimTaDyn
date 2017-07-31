#ifndef MAPEDITOR_HPP_
#  define MAPEDITOR_HPP_

#  include "Names.hpp"
#  include "SimTaDynLoaders.hpp"
#  include "Inspector.hpp"
#  include "DrawingArea.hpp"

class LoaderManager;

// *************************************************************************************************
// This class implements a Controler pattern of the Model-View-Controler (MVC) design pattern.
// *************************************************************************************************
class MapEditor
  : public Gtk::HBox,
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
  class SimTaDynMapListener: public ISimTaDynMapListener
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

  //! \brief Return the current map
  inline SimTaDynMap* map()
  {
    return m_current_map.get();
  }
  SimTaDynMap* map(const Key id);

  //! \brief Load a new map from a file through a dialog box.
  inline bool open()
  {
    return dialogLoadMap(true, false);
  }

  //! \brief Load a new map from a file.
  inline bool open(std::string const& filename)
  {
    return doOpen(filename, true, false);
  }

  //! \brief Create a dummy map.
  void newMap();

  //! \brief Load a map from a file in the current map.
  inline bool addMap()
  {
    return dialogLoadMap(false, false);
  }

  //! \brief Load a map from a file in the current map.
  inline bool addMap(std::string const& filename)
  {
    return doOpen(filename, false, false);
  }

  //! \brief Load a map and replace the current map.
  inline bool replace()
  {
    return dialogLoadMap(false, true);
  }

  //! \brief Load a map and replace the current map.
  inline bool replace(std::string const& filename)
  {
    return doOpen(filename, false, true);
  }

  //! \brief Reset the map (suppres everything)
  inline void clear()
  {
    if (nullptr != m_current_map.get())
      {
        m_current_map.get()->clear();
      }
  }

  //! \brief Close the current map and activate the previous one (if
  //! present)
  void close();
  void save() {} //TODO
  void saveAs();
  bool exec();

  //! \brief Attach a the MVC view to this MVC controller.
  void attachView(GLDrawingArea& drawing_area)
  {
    m_drawing_area = &drawing_area;
    //m_drawing_area->attachController(this);
    m_vbox.pack_start(drawing_area);
  }

protected:

  void registerLoaders();
  virtual bool load(const std::string& filename, SimTaDynMap* &oldmap);
  bool doOpen(std::string const& filename, const bool new_map, const bool reset_map);
  bool dialogLoadMap(const bool new_map, const bool reset_map);
  bool dialogSaveAsMap(const bool closing);
  void add(const Key id, SimTaDynMap* map);
  void remove(const Key id);

public:

  //! \brief Current model of the MVC design pattern
  SimTaDynMapHolder     m_current_map;
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
