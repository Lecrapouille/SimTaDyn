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

#ifndef SIMWINDOW_MAP_EXPLORER_HPP
#  define SIMWINDOW_MAP_EXPLORER_HPP

#include "MapExplorer.hpp"
#include "SimTaDynMap.hpp"

class MapExplorerWindow :
  public Gtk::ApplicationWindow
//protected IMapEvents
{
public:

  MapExplorerWindow(Glib::RefPtr<Gtk::Application> application);

  //protected:

  void onMapCreated(SimTaDynMapPtr map);
  /*virtual void onMapFailure(SimTaDynMapPtr map) override;
  virtual void onMapUpdated(SimTaDynMapPtr map) override;
  virtual void onMapClosed(SimTaDynMapPtr map) override;*/

protected:

  Glib::RefPtr<Gtk::Application> m_application;
  SimTaDynMapExplorer m_explorer;
};

#endif // SIMWINDOW_MAP_EXPLORER_HPP
