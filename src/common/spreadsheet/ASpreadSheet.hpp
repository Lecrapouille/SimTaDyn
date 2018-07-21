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
// along with GNU Emacs.  If not, see <http://www.gnu.org/licenses/>.
//=====================================================================

#ifndef SIMTADYN_SPREADSHEET_HPP_
#  define SIMTADYN_SPREADSHEET_HPP_

#  include "ASpreadSheetCell.hpp"
#  include <queue>
#  include <map>

class SimForth;
class ASpreadSheetCell;

// **************************************************************
//
// **************************************************************
class ASpreadSheet
{
public:

  ASpreadSheet()
  {
    //LOGI("New ASpreadSheet");
  }

  virtual ~ASpreadSheet()
  {
  }

  void debugDependenciesMap();
  virtual ASpreadSheetCell *isACell(std::string const& word) = 0;
  std::pair<bool, std::string> evaluate(SimForth &forth); // FIXME: Forth et mauvais nom
  void parse(SimForth &forth);
  virtual const std::string& name() const = 0;

protected:

  virtual void resetCellIterator() = 0;
  virtual bool hasCell() const = 0;
  virtual ASpreadSheetCell* nextCell() /*const*/ = 0; // FIXME
  virtual uint32_t howManyCells() const = 0;

private:

  void addToDependenciesMap(ASpreadSheetCell& cell);
  void resolveDependencies(ASpreadSheetCell& cell);

  inline void clearQueue(std::queue<ASpreadSheetCell*> &q)
  {
    std::queue<ASpreadSheetCell*> empty;
    std::swap(q, empty);
  }

  //using hashCell = std::map<std::string, ASpreadSheetCell*>;
  std::map<std::string, std::map<std::string, ASpreadSheetCell*>> m_dependencies;
  std::queue<ASpreadSheetCell*> m_topologicalList;
};

#endif /* SIMTADYN_SPREADSHEET_HPP_ */
