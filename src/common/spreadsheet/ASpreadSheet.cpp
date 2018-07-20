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

#include "ASpreadSheet.hpp"

void ASpreadSheet::parse(SimForth &forth)
{
  forth.m_spreadsheet = this;
  LOGI("ASpreadSheet::parse %p", forth.m_spreadsheet);

  resetCellIterator();
  while (hasCell())
    {
      ASpreadSheetCell* cell = nextCell();
      assert(cell != nullptr);

      cell->parse();//FIXME forth);
    }

  //TODO m_parsed = true;
  //TODO effacer ce flag quand on modifie une cellule
}

std::pair<bool, std::string>
ASpreadSheet::evaluate(SimForth &forth)
{
  LOGI("ASpreadSheet::evaluate %p", forth.m_spreadsheet);
  //TODO if (!m_parsed) { parse(); }
  forth.m_spreadsheet = this;

  // Empty containers
  resetCellIterator();
  clearQueue(m_topologicalList);

  // Filter cells: which ones can be directly evaluated
  // formulae with cell references versus. formulaes with
  // literals or operations without references on cells.
  //std::cout << "1st step -------------" << std::endl;
  while (hasCell())
    {
      ASpreadSheetCell* cell = nextCell();
      assert(cell != nullptr);

      if (cell->hasReferences())
        {
          //std::cout << "  add Dep " << cell->name()
          //          << " '" << cell->formulae()
          //          << "'" << std::endl;
          addToDependenciesMap(*cell); // FIXME: pas mieux si cell->addDep(this); qui du coup est le tableau des Observers
          //debugDependenciesMap();
        }
      else
        {
          //std::cout << "  ************** add topo " << cell->name()
          //          << " '" << cell->formulae()
          //          << "'" << std::endl;
          m_topologicalList.push(cell);
        }
      //cell->modified = false;
    }
  //debugDependenciesMap();

  // Evaluate cells which does not contain references on
  // other cells.
  uint32_t unsolvedCells = howManyCells();
  //std::cout << std::endl << "2nd step -------------" << std::endl;
  //std::cout << "  unsolved cells " << unsolvedCells << std::endl;
  try
    {
      while (!m_topologicalList.empty())
        {
          std::cout << std::endl;

          // Pop the cell
          //std::cout << "Topo queue " << m_topologicalList.size() << std::endl;
          if (0u == m_topologicalList.size())
            forth.abort("Topological list shall not be empty");

          ASpreadSheetCell* cell = m_topologicalList.front();
          m_topologicalList.pop();
          assert(cell != nullptr);

          // Interprete the cell
          //std::cout << "interpret " << cell->name() << ": " << cell->formulae() << std::endl;
          std::pair<bool, std::string> res = forth.interpreteCell(*cell);

          // Failure: cell has a bad formulae
          if (res.first == false)
            {
              std::cout << "  interpret ko" << std::endl;
              return res;
            }

          // Success
          --unsolvedCells;
          //std::cout << "  unsolved cells " << unsolvedCells << std::endl;
          resolveDependencies(*cell);
        }

      if (unsolvedCells != 0)
        {
          //setCircularDependent(true);
          // FIXME: faudrait afficher le nom du spreadsheet dans le message d'erreur du forth
          forth.abort("CircularDependencyFound: Unable to solve the spreadsheet");
        }
    }
  catch (ForthException const& e)
    {
      return std::make_pair(false, e.message());
    }
  return std::make_pair(true, "ok");
}

void ASpreadSheet::debugDependenciesMap()
{
  std::cout << "debugDependenciesMap():" << std::endl;
  for (const auto& c: m_dependencies)
    {
      std::cout << c.first << ":";
      for (const auto& c1: c.second)
        {
          std::cout << "    '" << c1.second->name()  << "'";
        }
      std::cout << std::endl;
    }
}

void ASpreadSheet::addToDependenciesMap(ASpreadSheetCell& cell)
{
  //std::cout << "addToDependenciesMap " << cell.name() << std::endl;
  for (const auto& c: cell.references())
    {
      //std::cout << "  addToDependenciesMap cell.references " << c->name()
      //          << " '" << c->formulae() << "'" << std::endl;

      const auto& it = m_dependencies.find(c->name());
      if (it != m_dependencies.end())
        {
          m_dependencies[c->name()][cell.name()] = &cell;
          //std::cout << "  ************** Nieme fois: " << c->name()
          //        << std::endl;
        }
      else
        {
          m_dependencies[c->name()][c->name()] = &cell;
          //std::cout << "  ************** 1ere fois: '" << c->name()
          //  << std::endl;
        }
      //debugDependenciesMap();
    }
}

void ASpreadSheet::resolveDependencies(ASpreadSheetCell& cell)
{
  //std::cout << "  resolveDependencies. find " << cell.name() << std::endl;
  //debugDependenciesMap();

  const auto& curCellDeps = m_dependencies.find(cell.name());
  if (curCellDeps == m_dependencies.end())
    {
      //std::cout << "    resolveDependencies return" << std::endl;
      return ;
    }

  //std::cout << "   resolveDependencies found: "
  //  << curCellDeps->second.size() << std::endl;

  // Get all the cells dependent on this cell
  for (auto& depCell: curCellDeps->second)
    {
      assert(depCell.second != nullptr);

      /*std::cout << "resolveDependencies looping "
        << depCell.second->name() << " "
        << depCell.second->formulae() << " "
        << std::endl;*/

      --depCell.second->m_unresolvedRefs; // FIXME: utiliser methode
      //std::cout << "hasRef " << depCell.second->hasReferences() << std::endl;
      if (!depCell.second->hasReferences())
        {
          //std::cout << "insert in topo list" << std::endl;
          m_topologicalList.push(depCell.second);
        }
      else
        {
          //depCell.second->debugReferences();
        }
    }
}
