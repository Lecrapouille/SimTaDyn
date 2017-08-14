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

#include "SimTaDynFile.hpp"
#include "File.hpp"

void SimTaDynFileLoader::loadFromFile(std::string const& filename, SimTaDynMap* &current_map)
{
  bool dummy_map = (nullptr == current_map);

  LOGI("Loading the SimTaDynFile '%s' in an %s",
       filename.c_str(), (dummy_map ? "dummy map" : "already opened map"));
}
