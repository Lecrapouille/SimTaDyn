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

MapEditor::MapEditor()
  : m_action_type(m_toolbar),
    m_action_on(m_toolbar)
{
  addPresenter(new MapPresenter(/*FIXME *this, createDummyMap()*/));
}

bool MapEditor::evalSheet() // FIXME: Exec(typeCell, nodeID)
{
#if 0
  SimTaDynSheet& sheet = activeModel().map().currentSheet();

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
#endif
  return false;
}

void MapEditor::closePresenter()
{
  m_presenters.pop_back();
  if (m_presenters.size() > 0_z)
    {
      m_active_presenter = m_presenters.back();
    }
  else
    {
      // TODO kill the window
    }
}

bool MapEditor::dialogLoadMap(bool const new_map, bool const reset_map)
{
  std::string filename;

  if (dialogLoad<SimTaDynMap>("Load a SimTaDyn map", filename))
    {
      return doOpenMap(filename, new_map, reset_map);
    }
  return false;
}

bool MapEditor::dialogLoadSheet(bool const new_sheet, bool const reset_sheet)//SimTaDynSheet& sheet)
{
  std::string filename;

  if (dialogLoad<SimTaDynSheet>("Load a SimTaDyn sheet", filename))
    {
      return doOpenSheet(filename, new_sheet, reset_sheet);
    }
  return false;
}

bool MapEditor::doOpenMap(std::string const& filename, bool const new_map, bool const reset_map)
{
#if 0
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
#endif
  return false;
}

// *************************************************************************************************
//
// *************************************************************************************************
bool MapEditor::doOpenSheet(std::string const& filename, bool const new_sheet, bool const reset_sheet)
{
#if 0
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
#endif
  return false;
}



#if 0

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

#endif
