#include "MapEditor.hpp"
#include "ShapeFile.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
  : MapLoader()
{
  // Menus '_Map'
  {
    m_menuitem[0].set_label("_Map");
    m_menuitem[0].set_use_underline(true);
    m_menuitem[0].set_submenu(m_menu[0]);

    // TODO: New Map

    //
    m_submenu[1].set_label("Open Map");
    m_image[1].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect(sigc::mem_fun(*this, &MapEditor::open));
    m_menu[0].append(m_submenu[1]);

    // TODO: Save (As) Map
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
void MapEditor::open() // FIXME: bool: true == charge une map dans un ancienne
{
  Gtk::FileChooserDialog dialog("Choose a binary file to save Forth dictionary",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_menu[0].get_toplevel()));

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_simtadyn = Gtk::FileFilter::create();
  filter_simtadyn->set_name("SimTaDyn files");
  filter_simtadyn->add_pattern("*.simtadyn");
  dialog.add_filter(filter_simtadyn);

  auto filter_shapefile = Gtk::FileFilter::create();
  filter_shapefile->set_name("Shape files");
  filter_shapefile->add_pattern("*.shp");
  dialog.add_filter(filter_shapefile);

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      SimTaDynMap* map = nullptr; // FIXME map = (bool) ? getCurrentMap() : nullptr;
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
          m_maps.push_back(map);
          std::cout << "Successfully loaded map '" << map->m_name << "'" << std::endl;
          //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
          //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }
        }
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(std::string const& filename, SimTaDynMap *map)
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
