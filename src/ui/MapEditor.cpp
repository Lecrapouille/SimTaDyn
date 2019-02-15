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
#include "Config.hpp"
#include "SimTaDynLoaders.hpp"

MapEditor::MapEditor(PopupException& popup_exception, SimForth& forth)
  : m_popup_exception(popup_exception),
    m_forth(forth)
{
  addPresenter(new MapPresenter(popup_exception /*FIXME *this, createDummyMap()*/));
}

bool MapEditor::evalSheet()
{
  SimTaDynSheet& sheet = activeSheet();
  std::pair<bool, std::string> res = m_forth.evaluate(sheet);
  m_forth.ok(res);
  // if (!res.first) popupexception(res.second)
  return res.first;
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

template <class L>
bool MapEditor::dialogLoad(Gtk::Window& win, std::string const& title, std::string& filename)
{
  Gtk::FileChooserDialog dialog(title, Gtk::FILE_CHOOSER_ACTION_OPEN);
  dialog.set_transient_for(win);
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

bool MapEditor::dialogLoadMap(Gtk::Window& win, bool const new_map, bool const reset_map)
{
  std::string filename;

  if (dialogLoad<SimTaDynMap>(win, "Load a SimTaDyn map", filename))
    {
      return doOpenMap(filename, new_map, reset_map);
    }
  return false;
}

bool MapEditor::dialogLoadSheet(Gtk::Window& win, bool const new_sheet, bool const reset_sheet)//SimTaDynSheet& sheet)
{
  std::string filename;

  if (dialogLoad<SimTaDynSheet>(win, "Load a SimTaDyn sheet", filename))
    {
      return doOpenSheet(filename, new_sheet, reset_sheet);
    }
  return false;
}

bool MapEditor::doOpenMap(std::string const& filename, bool const new_map, bool const reset_map)
{
  try
    {
      LOGI("doOpenMap r:%u n:%u", reset_map, new_map);

      std::string name = (new_map) ? File::baseName(filename) : filename;
      SimTaDynMapPtr map = (new_map) ? SimTaDynMapManager::instance().create(name, resource::Strategy::ReturnNull, name)
                                     : activeModelPtr();

      if (nullptr == map)
        {
          //throw LoaderException("map shall not be nullptr");
          return false; // Map already loaded
        }

      if (reset_map)
        {
          map->resetSheets();
        }

      LoaderManager::instance().loadFromFile(filename, *map);
      loaded_success.emit(map);
      changeActiveModel(map);

      //FIXME: attraper ce signal par le renderer puis
      //FIXME if (bool) { selectionner toutes la map pour permettre a l'utilisateur de la placer la ou il vaut }
      //FIXME zoomer sur la fusion des deux bounding box de l'ancinne et nouvelle map }

      return true;
    }
  catch (LoaderException const &e)
    {
      //loaded_failure.emit(filename, e.message());

      m_popup_exception.popupException(e, "Could not load '" + filename + "' as a SimTaDyn map");
      return false;
    }

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
