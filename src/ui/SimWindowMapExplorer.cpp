//=====================================================================
// SimTaDyn: A GIS in a spreadsheet.
// Copyright 2019 Quentin Quadrat <lecrapouille@gmail.com>
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

#include "SimWindowMapExplorer.hpp"

MapExplorerWindow::MapExplorerWindow(Glib::RefPtr<Gtk::Application> application)
  : m_application(application)
{
  add(m_explorer.widget());
  set_default_size(180, 300);
  set_position(Gtk::WIN_POS_CENTER);

  // TODO:
  //  ResourceMapManager.signal_created(sigc::mem_fun(*this, &SimTaDynMapExplorer::addMap));


  show_all();
}

void MapExplorerWindow::onMapCreated(SimTaDynMapPtr map)
{
  m_explorer.addMap(map);
}

/*
void MapExplorerWindow::onMapUpdated(SimTaDynMapPtr map)
{
  m_explorer.updateMap(map);
}

void MapExplorerWindow::onMapClosed(SimTaDynMapPtr map)
{
  m_explorer.removeMap(map);
}*/
