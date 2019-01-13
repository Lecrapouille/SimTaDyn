//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2018 Quentin Quadrat <lecrapouille@gmail.com>
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
// along with SimTaDyn.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#include "MapEditor.hpp"
#include "MapEditionTools.hpp"
#include <functional>

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::MapEditor(SimForth& forth)
  : m_forth(forth),
    m_action_type(m_toolbar),
    m_action_on(m_toolbar)
{
  LOGI("Creating MapEditor");

  // Drawing area
  {
    m_vbox.pack_start(m_drawing_area);
  }

  // Init map edition tool to dummy action
  {
    m_edition_tools[ActionType::Add] = std::make_unique<AddCellTool>(*this);
    m_edition_tools[ActionType::Remove] = std::make_unique<RemoveCellTool>(*this);
    m_edition_tools[ActionType::Select] = std::make_unique<SelectCellTool>(*this);
    m_edition_tools[ActionType::Move] = std::make_unique<MoveCellTool>(*this);
  }

  // FIXME: spliter en 2 sous menus
  // Menu '_Map'
  {
    m_menuitem[simtadyn::MapMenu].set_label("_Map");
    m_menuitem[simtadyn::MapMenu].set_use_underline(true);
    m_menuitem[simtadyn::MapMenu].set_submenu(m_menu[simtadyn::MapMenu]);

    size_t i = 0_z;

    //
    m_submenu[i].set_label("New Map");
    m_image[i].set_from_icon_name("document-new", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::newMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Open Map");
    m_image[i].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::openMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Save Map");
    m_image[i].set_from_icon_name("document-save", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::saveMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Save As Map");
    m_image[i].set_from_icon_name("document-save-as", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::saveAsMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_menu[simtadyn::MapMenu].append(m_menu_separator[0]);

    //
    m_submenu[i].set_label("Replace current map by");
    m_image[i].set_from_icon_name("gtk-delete", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::replaceMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Merge Map");
    m_image[i].set_from_icon_name("gtk-refresh", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::mergeMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Clear Map");
    m_image[i].set_from_icon_name("user-trash", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::clearMap(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_menu[simtadyn::MapMenu].append(m_menu_separator[1]);

    //
    m_submenu[i].set_label("New directed sheet");
    m_image[i].set_from_icon_name("gtk-sort-ascending", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::newSheet(true); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("New undirected sheet");
    m_image[i].set_from_icon_name("gtk-sort-descending", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::newSheet(false); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Import Sheet");
    m_image[i].set_from_icon_name("document-open", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::openSheet(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Replace current sheet by");
    m_image[i].set_from_icon_name("gtk-delete", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::replaceSheet(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    //
    m_submenu[i].set_label("Clear Sheet");
    m_image[i].set_from_icon_name("user-trash", Gtk::ICON_SIZE_MENU);
    m_submenu[i].set_image(m_image[i]);
    m_submenu[i].signal_activate().connect([this](){ MapEditor::resetSheets(); });
    m_menu[simtadyn::MapMenu].append(m_submenu[i]);
    ++i;

    // TODO
    // mergeSheet, saveSheet, saveAsSheet, closeSheet
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
    m_action_type.append(ActionType::Add, "Add a cell", Gtk::Stock::ADD,
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
    m_vbox.pack_start(m_cell_editor.widget(), Gtk::PACK_SHRINK);
    m_hbox.pack_start(m_vbox);
    m_hbox.pack_start(m_toolbar, Gtk::PACK_SHRINK);
  }

  // Signals
  loaded_success.connect(sigc::mem_fun(*this, &MapEditor::repaintMap));
  m_current_map.signal_top_map_changed.connect([this](SimTaDynMapPtr map)
  {
    LOGC("signal_top_map_changed: repaintMap");
    MapEditor::repaintMap(map);
  });


  loaded_success.connect(sigc::mem_fun(m_map_explorer, &SimTaDynMapExplorer::onSuccessMapLoaded));
  sheet_changed.connect(sigc::mem_fun(m_map_explorer, &SimTaDynMapExplorer::on_sheet_changed));
  loaded_failure.connect(sigc::mem_fun(m_map_explorer, &SimTaDynMapExplorer::onFailMapLoaded));
  saved_success.connect(sigc::mem_fun(m_map_explorer, &SimTaDynMapExplorer::onSuccessMapSaved));
  saved_failure.connect(sigc::mem_fun(m_map_explorer, &SimTaDynMapExplorer::onFailMapSaved));
}

// *************************************************************************************************
//
// *************************************************************************************************
MapEditor::~MapEditor()
{
  LOGI("Destroying MapEditor");
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

  // http://transit.iut2.upmf-grenoble.fr/doc/gtkmm-3.0/reference/html/group__gdkmmEnums.html
  // Change the mouse cursor
  static Gdk::CursorType cursors[] =
    {
      [ActionType::Add] = Gdk::PLUS,
      [ActionType::Remove] = Gdk::PIRATE,
      [ActionType::Select] = Gdk::HAND1,
      [ActionType::Move] = Gdk::HAND2,
    };

  widget().get_window()->set_cursor(Gdk::Cursor::create(cursors[id]));
}

// *************************************************************************************************
//!
// *************************************************************************************************
void MapEditor::closeMap()
{
  SimTaDynMapPtr map = m_current_map.get();

  // No map currently used.
  if (nullptr == map)
    return ;

  // TODO save sheets then save map

  // Delete all sheets
  map->resetSheets();

  // TODO get the previously used map
  m_current_map.set(nullptr);
}

// *************************************************************************************************
// FIXME m_map_id = 0 peut ne pas etre bon si on a deja une carte#10 => commencer a 11
// *************************************************************************************************
void MapEditor::newMap()
{
  SimTaDynMapPtr map = nullptr;
  do
    {
      std::string name = "NewMap_" + std::to_string(m_map_id++);
      LOGI("Creating a dummy SimTaDyn map named '%s'", name.c_str());
      try
        {
          map = SimTaDynMapManager::instance().create(name, resource::Strategy::ReturnNull, name);
        }
      catch (...)
        {
          map = nullptr;
        }
    }
  while (nullptr == map);

  loaded_success.emit(map);
  m_current_map.set(map);
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::repaintMap(SimTaDynMapPtr map)
{
  LOGI("Repainting map %s", map->name().c_str());
  map->drawnBy(m_drawing_area);
}

// *************************************************************************************************
//
// *************************************************************************************************
template <class L>
bool MapEditor::dialogLoad(std::string const& title, std::string& filename)
{
  Gtk::FileChooserDialog dialog(title,
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
  for (auto const& it: LoaderManager::instance().loaders<L>())
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

  bool res = (Gtk::RESPONSE_OK == dialog.run());
  filename = dialog.get_filename();

  return res;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::dialogLoadMap(bool const new_map, bool const reset_map)
{
  std::string filename;

  if (dialogLoad<SimTaDynMap>("Load a SimTaDyn map", filename))
    {
      return doOpenMap(filename, new_map, reset_map);
    }
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::dialogLoadSheet(bool const new_sheet, bool const reset_sheet)//SimTaDynSheet& sheet)
{
  std::string filename;

  if (dialogLoad<SimTaDynSheet>("Load a SimTaDyn sheet", filename))
    {
      return doOpenSheet(filename, new_sheet, reset_sheet);
    }
  return false;
}

// *************************************************************************************************
// TODO cacher les managers: https://openclassrooms.com/forum/sujet/petit-conseil-1#message-84682530
// *************************************************************************************************
bool MapEditor::doOpenMap(std::string const& filename, bool const new_map, bool const reset_map)
{
   try
    {
      LOGI("doOpenMap r:%u n:%u", reset_map, new_map);

      std::string name = (new_map) ? File::baseName(filename) : filename;
      SimTaDynMapPtr map = (new_map) ? SimTaDynMapManager::instance().create(name, resource::Strategy::ReturnNull, name) : m_current_map.get();

      if (nullptr == map)
        {
          throw LoaderException("map shall not be nullptr");
        }

      if (reset_map)
        {
          map->resetSheets();
        }

      LoaderManager::instance().loadFromFile(filename, *map);
      loaded_success.emit(map);
      m_current_map.set(map);

      //FIXME: attraper ce signal par le renderer puis
      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }

      return true;
    }
  catch (LoaderException const &e)
    {
      loaded_failure.emit(filename, e.message());
      Gtk::MessageDialog dialog((Gtk::Window&) *(m_vbox.get_toplevel()),
                                e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn map. Reason: "
                                + e.message());
      dialog.run();
      return false;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::doOpenSheet(std::string const& filename, bool const new_sheet, bool const reset_sheet)
{
   try
    {
      LOGI("doOpenSheet r:%u n:%u", reset_sheet, new_sheet);

      std::string name = (new_sheet) ? File::baseName(filename) : filename;
      SimTaDynSheetPtr sheet = (new_sheet) ? map().createSheet(name) : map().sheet(name);

      if (nullptr == sheet)
        {
          throw LoaderException("map shall not be nullptr");
        }

      if (reset_sheet)
        {
          sheet->reset();
        }

      LoaderManager::instance().loadFromFile(filename, *sheet);
      emitSheetChanged();

      //FIXME: attraper ce signal par le renderer puis
      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }

      return true;
    }
  catch (LoaderException const &e)
    {
      loaded_failure.emit(filename, e.message());
      Gtk::MessageDialog dialog((Gtk::Window&) *(m_vbox.get_toplevel()),
                                e.what(), false, Gtk::MESSAGE_WARNING);
      dialog.set_secondary_text("Could not load '" + filename + "' as a SimTaDyn sheet. Reason: "
                                + e.message());
      dialog.run();
      return false;
    }
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::evalSheet() // FIXME: Exec(typeCell, nodeID)
{
  SimTaDynSheet& sheet = map().currentSheet();

  // FIXME: should be called outside each cell: optimisation
  // Disable compilation mode
  m_forth.dictionary().smudge(":");
  m_forth.dictionary().smudge("INCLUDE");

  sheet.parse(m_forth);
  std::pair<bool, std::string> res = sheet.evaluate(m_forth);
  m_forth.ok(res);

  // Enable compilation mode
  m_forth.dictionary().smudge("INCLUDE");
  m_forth.dictionary().smudge(":");

  return res.first;
}

// *************************************************************************************************
//
// *************************************************************************************************
void MapEditor::saveAsMap()
{
  LOGF("saveAsMap not yet implemented");
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
bool MapEditor::dialogSaveAsMap(bool const closing)
{
  LOGF("dialogSaveAsMap not yet implemented");
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

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
void MapEditor::button1PressEvent(const gdouble x, const gdouble y)
{
  std::cout << "MapEditor::Bouton1 click " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
  m_edition_tools[actionType()]->exec1(x, y);
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
void MapEditor::button2PressEvent(const gdouble x, const gdouble y)
{
  std::cout << "MapEditor::Bouton2 click " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
  m_edition_tools[actionType()]->exec2(x, y);
}

//------------------------------------------------------------------
//! \brief
//------------------------------------------------------------------
void MapEditor::button3PressEvent(const gdouble x, const gdouble y)
{
  std::cout << "MapEditor::Bouton3 click " << static_cast<int>(x) << " " << static_cast<int>(y) << std::endl;
  m_edition_tools[actionType()]->exec3(x, y);
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
