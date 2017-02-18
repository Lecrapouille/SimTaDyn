#include "MapEditor.hpp"
#include "ShapeFile.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
  : MapLoader(), m_current_map(-1)
{
  // Menus '_Map'
  {
    m_menuitem[0].set_label("_Map");
    m_menuitem[0].set_use_underline(true);
    m_menuitem[0].set_submenu(m_menu[0]);

    //
    m_submenu[1].set_label("New Map");
    m_image[1].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::newEmptyMap));
    m_menu[0].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Open Map");
    m_image[2].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::openMap));
    m_menu[0].append(m_submenu[2]);

    //
    m_submenu[3].set_label("Save Map");
    m_image[3].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::saveMap));
    m_menu[0].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save As Map");
    m_image[4].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::saveAsMap));
    m_menu[0].append(m_submenu[4]);

    //
    m_menu[0].append(m_menuseparator[0]);

    //
    m_submenu[5].set_label("Replace Map");
    m_image[5].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::replaceMap));
    m_menu[0].append(m_submenu[5]);

    //
    m_submenu[6].set_label("Add Map");
    m_image[6].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::addMap));
    m_menu[0].append(m_submenu[6]);

    //
    m_submenu[7].set_label("Clear Map");
    m_image[7].set_from_icon_name("user_trash", Gtk::ICON_SIZE_MENU);
    m_submenu[7].set_image(m_image[7]);
    m_submenu[7].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::clearMap));
    m_menu[0].append(m_submenu[7]);
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::~MapEditor()
{
  auto it = m_maps.begin();
  auto end = m_maps.end();

  for (; it != end; ++it)
    {
      delete (*it);
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::open(const bool new_map, const bool reset_map)
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_menu[0].get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_shapefile = Gtk::FileFilter::create();
  filter_shapefile->set_name("Shape files");
  filter_shapefile->add_pattern("*.shp");
  dialog.add_filter(filter_shapefile);

  auto filter_simtadyn = Gtk::FileFilter::create();
  filter_simtadyn->set_name("SimTaDyn files");
  filter_simtadyn->add_pattern("*.simtadyn");
  dialog.add_filter(filter_simtadyn);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimTaDynMap* map = (new_map) ? nullptr : MapEditor::map();
      SimTaDynMap* oldmap = map;

      if ((reset_map) && (nullptr != map))
        {
          map->m_graph.reset();
        }

      bool ret = load(dialog.get_filename(), map);
      if (false == ret)
        {
          Gtk::MessageDialog d("Could not load '" + dialog.get_filename() + "' as a map.",
                               false, Gtk::MESSAGE_WARNING);
          d.set_transient_for((Gtk::Window&) (*m_menu[0].get_toplevel()));
          d.set_secondary_text("Reason was: " + error());
          d.run();
        }
      else
        {
          if (reset_map)
            {
              map->m_name = SimTaDynMap::shortName(dialog.get_filename());
            }
          if (nullptr == oldmap)
            {
              m_maps.push_back(map);
              m_current_map = m_maps.size() - 1;
            }
          std::cerr << "Successfully loaded map '" << map->m_name << "'" << std::endl;
          //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
          //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }
        }
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(std::string const& filename, SimTaDynMap* &map)
{
  std::string extension = filename.substr(filename.find_last_of(".") + 1);

  if (0 == extension.compare("shp"))
    {
      ShapefileLoader loader;
      return loader.load(filename, map);
    }

  m_error = "Unknown extension file";
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
SimTaDynMap* MapEditor::map()
{
  if (m_current_map < 0)
    return nullptr;
  return m_maps[m_current_map];
}
