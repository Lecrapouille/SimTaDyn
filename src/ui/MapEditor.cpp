//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2017 Quentin Quadrat <lecrapouille@gmail.com>
//
// This file is part of SimTaDyn.
//
// SimTaDyn is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful, but
// WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "MapEditor.hpp"
#include <functional>

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor()
  : m_action_type(m_toolbar),
    m_action_on(m_toolbar),
    m_listener(*this)
{
  LOGI("Creating MapEditor");

  // Init map edition tool to dummy action
  m_edition_tools[ActionType::Add] = new AddCellTool();
  m_edition_tools[ActionType::Remove] = new RemoveCellTool();
  m_edition_tools[ActionType::Select] = new SelectCellTool();
  m_edition_tools[ActionType::Move] = new MoveCellTool();

  // Menu '_Map'
  {
    m_menuitem[simtadyn::MapMenu].set_label("_Map");
    m_menuitem[simtadyn::MapMenu].set_use_underline(true);
    m_menuitem[simtadyn::MapMenu].set_submenu(m_menu[simtadyn::MapMenu]);

    //
    m_submenu[1].set_label("New Map");
    m_image[1].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[1].set_image(m_image[1]);
    m_submenu[1].signal_activate().connect([this](){ MapEditor::newMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[1]);

    //
    m_submenu[2].set_label("Open Map");
    m_image[2].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[2].set_image(m_image[2]);
    m_submenu[2].signal_activate().connect([this](){ MapEditor::open(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[2]);

    //
    m_submenu[3].set_label("Save Map");
    m_image[3].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[3].set_image(m_image[3]);
    m_submenu[3].signal_activate().connect([this](){ MapEditor::save(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[3]);

    //
    m_submenu[4].set_label("Save As Map");
    m_image[4].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[4].set_image(m_image[4]);
    m_submenu[4].signal_activate().connect([this](){ MapEditor::saveAs(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[4]);

    //
    m_menu[simtadyn::MapMenu].append(m_menu_separator[0]);

    //
    m_submenu[5].set_label("Replace Map");
    m_image[5].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[5].set_image(m_image[5]);
    m_submenu[5].signal_activate().connect([this](){ MapEditor::replace(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[5]);

    //
    m_submenu[6].set_label("Add Map");
    m_image[6].set_from_icon_name("document-import", Gtk::ICON_SIZE_MENU);
    m_submenu[6].set_image(m_image[6]);
    m_submenu[6].signal_activate().connect([this](){ MapEditor::addMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[6]);

    //
    m_submenu[7].set_label("Clear Map");
    m_image[7].set_from_icon_name("user_trash", Gtk::ICON_SIZE_MENU);
    m_submenu[7].set_image(m_image[7]);
    m_submenu[7].signal_activate().connect([this](){ MapEditor::clear(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[7]);
  }

  // Map toolbar (vertical)
  {
    using namespace std::placeholders;
    m_toolbar.set_property("orientation", Gtk::ORIENTATION_VERTICAL);
    m_toolbar.set_property("toolbar-style", Gtk::TOOLBAR_ICONS);

    // Cells
    m_action_on.append(ActionOn::Node, "Switch to Node mode", Gtk::Stock::YES,
                       sigc::mem_fun(*this, &MapEditor::onActionOnSelected));
    m_action_on.append(ActionOn::Arc, "Switch to Arc mode", Gtk::Stock::NO,
                       sigc::mem_fun(*this, &MapEditor::onActionOnSelected));
    m_action_on.append(ActionOn::Zone, "Switch to Zone mode", Gtk::Stock::HOME,
                       sigc::mem_fun(*this, &MapEditor::onActionOnSelected));
    m_action_on.append(m_toolbar_separator[0]);

    // Operations on cells
    m_action_type.append(ActionType::Add, "Remove a cell", Gtk::Stock::ADD,
                         sigc::mem_fun(*this, &MapEditor::onActionTypeSelected));
    m_action_type.append(ActionType::Remove, "Remove a cell", Gtk::Stock::REMOVE,
                         sigc::mem_fun(*this, &MapEditor::onActionTypeSelected));
    m_action_type.append(ActionType::Select, "Select a cell", Gtk::Stock::JUMP_TO,
                         sigc::mem_fun(*this, &MapEditor::onActionTypeSelected));
    m_action_type.append(ActionType::Move, "Move a cell", Gtk::Stock::JUMP_TO,
                         sigc::mem_fun(*this, &MapEditor::onActionTypeSelected));
                         m_toolbar.append(m_toolbar_separator[1]);
  }

  // Pack all stuffs together
  {
    m_vbox.pack_start(m_inspector.m_scrolledwindow, Gtk::PACK_SHRINK);
    m_hbox.pack_start(m_vbox);
    m_hbox.pack_start(m_toolbar, Gtk::PACK_SHRINK);
  }
}

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::~MapEditor()
{
  LOGI("Destroying MapEditor");
  delete m_edition_tools[ActionType::Add];
  delete m_edition_tools[ActionType::Remove];
  delete m_edition_tools[ActionType::Select];
  delete m_edition_tools[ActionType::Move];

  // TODO: be sure no Forth script is running on the map before destroying mapq
}

// *************************************************************************************************
//!
// *************************************************************************************************
void MapEditor::onActionOnSelected_(const ActionOn id)
{
  LOGI("ActionOnSelected %u", id);
  // TODO: afficher les id du type de cellule selectionnee
}

// *************************************************************************************************
//!
// *************************************************************************************************
void MapEditor::onActionTypeSelected_(const ActionType id)
{
  LOGI("ActionTypeSelected %u", id);
  // TODO changer le curseur
}

// *************************************************************************************************
//!
// *************************************************************************************************
void MapEditor::close()
{
  SimTaDynMapPtr map = m_current_map.get();

  // No map currently used.
  if (nullptr == map)
    return ;

  // Look for another map
  /*for (const auto& it: SimTaDynMapManager::instance().resources())
    {
      m_current_map.set(SimTaDynMapManager::instance().acquire(it->name));
    }

  // The current map was the only one.
  if (it == SimTaDynMapManager::instance().resources().end())
  {*/
      map = nullptr;
      //  }

  m_current_map.set(map);
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::newMap()
{
  SimTaDynMapPtr map;
  std::string base_name = "NoName";
  std::string name(base_name);
  uint32_t i = 1;

  do
    {
      LOGI("Creating a dummy SimTaDyn map named '%s'", name.c_str());
      map = SimTaDynMapManager::instance().create(name);
      if (nullptr == map)
        {
          LOGI("Failed ! Try with a new name");
          name = base_name + '~' + std::to_string(i);
          ++i;
        }
    }
  while ((nullptr == map) && (i < 10));

  if (nullptr == map)
    {
      LOGI("Failed 10 times ! Abort !");
      return ;
    }

  map->addListener(m_listener);
  m_current_map.set(map);
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::dialogLoadMap(const bool new_map, const bool reset_map)
{
  Gtk::FileChooserDialog dialog("Load a SimTaDyn map",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_vbox.get_toplevel()));
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Open the dialog window and set the SimTaDyn path as current
  // folder instead of using the "smart-current-folder" strategy
  // thiugh by GTK+ developpers.
  dialog.set_current_folder(config::data_path);

  // Fill filters for selecting type of file. Use the loader manager
  // for filling these filters because its knows all loaders which
  // know file extensions they can load.
  for (auto it: LoaderManager::instance().loaders<SimTaDynMap>())
    {
      auto filter = Gtk::FileFilter::create();
      // loaders() return a map <<file extension>, <loader>>
      std::string extension("*." + it.first);
      filter->add_pattern(extension);
      filter->set_name(it.second->description() + " (" + extension + ')');
      dialog.add_filter(filter);
    }

  auto filter_any = Gtk::FileFilter::create();
  filter_any->set_name("Any files");
  filter_any->add_pattern("*");
  dialog.add_filter(filter_any);

  int result = dialog.run();
  if (Gtk::RESPONSE_OK == result)
    {
      doOpen(dialog.get_filename(), new_map, reset_map);
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::doOpen(std::string const& filename, const bool new_map, const bool reset_map)
{
  std::string name = (reset_map) ? File::baseName(filename) : filename;
  SimTaDynMapPtr map = (new_map) ? SimTaDynMapManager::instance().create(name) : m_current_map.get();

  if ((reset_map) && (nullptr != map))
    {
      map->clear();
    }

  bool res = load(filename, map);
  if (true == res)
    {
      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }
    }

  return res;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::load(std::string const& filename, SimTaDynMapPtr map)
{
  SimTaDynMapManager &rm = SimTaDynMapManager::instance();

  try
    {
      bool dummy_map = (nullptr == map);
      if ((dummy_map) || (false == rm.exist(map->name())))
        {
          LoaderManager::instance().loadFromFile(filename, map);
          if (dummy_map)
            {
              map->addListener(m_listener);
              //FIXME rm.add(map);
              m_current_map.set(map);
            }
        }
    }
  catch (LoaderException const &e)
    {
      Gtk::MessageDialog dialog((Gtk::Window&) *(m_vbox.get_toplevel()),
                                e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn map. Reason: "
                                + e.message());
      dialog.run();
      return false;
    }
  catch (std::exception const &e)
    {
      Gtk::MessageDialog dialog((Gtk::Window&) *(m_vbox.get_toplevel()),
                                e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn map.");
      dialog.run();
      return false;
    }

  return true;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::exec() // FIXME: Exec(typeCell, nodeID)
{
  SimForth &forth = SimForth::instance();
  SimTaDynSheet *sheet = map()->sheet();

  if (nullptr == sheet)
  {
    LOGE("No sheet attached to Forth");
    return false;
  }

  // FIXME: should be called outside each cell: optimisation
  // Disable compilation mode
  forth.dictionary().smudge(":");
  forth.dictionary().smudge("INCLUDE");

  sheet->parse(forth);
  std::pair<bool, std::string> res = sheet->evaluate(forth);
  forth.ok(res);

  // Enable compilation mode
  forth.dictionary().smudge("INCLUDE");
  forth.dictionary().smudge(":");

  return res.first;
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::saveAs()
{
  /*Gtk::FileChooserDialog dialog("Save a SimTaDyn map file",
                                Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for((Gtk::Window&) (*m_vbox.get_toplevel()));

  // Set to the SimTaDyn path while no longer the GTK team strategy.
  dialog.set_current_folder(config::data_path);

  // Add response buttons the the dialog:
  dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
  dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

  // Add filters, so that only certain file types can be selected:
  auto filter_shapefile = Gtk::FileFilter::create();
  filter_shapefile->set_name("Shape files"); // FIXME: should use LoaderManager informations to avoid double code
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
      MapEditor::save(dialog.get_filename());
      }*/
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::dialogSaveAsMap(const bool closing)
{
  return false;
  /*Gtk::MessageDialog dialog((Gtk::Window&) (*m_vbox.get_toplevel()),
                            "The document '" + map.m_name +
                            "' has been modified. Do you want to save it now ?",
                            false, Gtk::MESSAGE_QUESTION, Gtk::BUTTONS_YES_NO);
  dialog.add_button(Gtk::Stock::SAVE_AS, Gtk::RESPONSE_APPLY);

  int result = dialog.run();
  if (Gtk::RESPONSE_YES == result)
    {
      if (0 == doc->m_filename.compare(""))
        {
          return MapEditor::saveAs(doc);
        }
      else
        {
          return doc->save();
        }
    }
  else if (Gtk::RESPONSE_APPLY == result)
    {
      return MapEditor::saveAs(doc);
    }
  else // other button
    {
      if (closing)
        {
          doc->modified(false);
          return true;
        }
      return !doc->modified();
      }*/
}

// **************************************************************
// Interface
// **************************************************************
/*Gtk::ToolButton *MapEditor::addButon(const Gtk::BuiltinStockID icon,
                                     const std::string &script,
                                     const std::string &help)
{
  return addForthButon(MapToolbar, icon, script, help);
  }*/
