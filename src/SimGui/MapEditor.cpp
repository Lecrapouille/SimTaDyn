#include "MapEditor.hpp"
#include "MapLoaders.hpp"

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
  : m_root(nullptr)
{
  // Menus '_Map'
  {
    m_menuitem[0].set_label("Map");
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
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::open()
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
      load(dialog.get_filename(), m_graph);
      // FIXME return not taken into account
      std::cout << m_graph.name << std::endl;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(const std::string& filename, SimTaDynGraph& graph)
{
  bool res = false;
  std::string extension = filename.substr(filename.find_last_of(".") + 1);
  if (0 == extension.compare("shp"))
    {
      ShapefileLoader loader;
      res = loader.load(filename, graph);
    }
  // else if ... other extensions
  return res;
}
