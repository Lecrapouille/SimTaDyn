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

#include "SimTaDynSheet.hpp"

ASpreadSheetCell *SimTaDynSheet::isACell(std::string const& word)
{
  if (('A' != word[0]) && ('N' != word[0]) && ('Z' != word[0]))
    return nullptr;

  // Get the unique ID
  Key id = 0u;
  uint32_t i = 1u;
  while ((word[i] != '\0') && (word[i] >= '0') && (word[i] <= '9'))
    {
      id = id * 10U + word[i] - '0';
      ++i;
    }

  // End of the word
  if (word[i] == '\0')
    {
      if ('N' == word[0])
        {
          LOGI("Found Node %u", id);
          return &getNode(id);
        }
      if ('A' == word[0])
        {
          LOGI("Found Arc %u", id);
          return &getArc(id);
        }
      if ('Z' == word[0])
        {
          std::cerr << "Found Zone TODO " << id << std::endl;
          LOGI("Found Zone %u", id);
          return nullptr; // &getZone(id); //FIXME a finir
        }
    }

  return nullptr;
}
