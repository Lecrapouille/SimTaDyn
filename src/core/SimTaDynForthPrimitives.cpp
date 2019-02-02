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

#include "SimTaDynForthPrimitives.hpp"
#include "SimTaDynForth.hpp"
#include "MapEditor.hpp"
#include "PathManager.hpp"

void SimForth::execPrimitive(const Cell16 idPrimitive)
{
  switch (idPrimitive)
    {
    case SIMFORTH_PRIMITIVE_SHEET:
      {
        std::string filename = PathManager::instance().top() + nextWord();
        LOGD("SIMFORTH_PRIMITIVE_SHEET: '%s'", filename.c_str());
        MapEditor::instance().openSheet(filename);
      }
      break;
    case SIMFORTH_PRIMITIVE_FORTH:
      Forth::execPrimitive(FORTH_PRIMITIVE_INCLUDE); // FIXME path() +
      break;
    default:
      Forth::execPrimitive(idPrimitive);
      break;
    }
}
