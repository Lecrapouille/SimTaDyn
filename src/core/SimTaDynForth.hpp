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

#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "ASpreadSheet.hpp"
#  include "Forth.hpp"
#  include "SimTaDynForthPrimitives.hpp"
#  include <sigc++/sigc++.h>

class ASpreadSheetCell;
class ASpreadSheet;

// *************************************************************************************************
//! \brief
// *************************************************************************************************
class SimForthDictionary : public ForthDictionary
{
public:

  SimForthDictionary()
    : ForthDictionary()
  {
    LOGI("Creating SimForthDictionary");
  }
};

// *************************************************************************************************
//! \brief SimTaDynForth a classic Forth modified for GIS and spreadsheet
// *************************************************************************************************
class SimForth
  : public Forth
{
public:

  SimForth()
    : Forth(m_dictionaries)
  {
    LOGI("Creating SimForth");
  }

  virtual ~SimForth()
  {
    LOGI("Destroying SimForth");
  }

  virtual void ok(std::pair<bool, std::string> const& res) override
  {
    Forth::ok(res);
    emitForthInterpreteDone();
  }

  virtual void boot() override;
  void compileCell(ASpreadSheetCell &cell);
  std::pair<bool, std::string> evaluate(ASpreadSheet& spreadsheet);
  std::pair<bool, std::string> interpreteCell(ASpreadSheetCell &cell);
  bool parseCell(ASpreadSheetCell &cell);

  // Save temporary pathes of unziped SimTaDynMap files.
  // Like this PathManager does not have to get them this
  // avoids conflict with 2 resources with the same name
  // in two unziped locations.
  void pushPath(std::string const& path)
  {
    m_pathes.push_back(path);
  }
  void popPath()
  {
    if (!m_pathes.empty())
      {
        m_pathes.pop_back();
      }
  }
  std::string const& path()
  {
    return m_pathes.back();
  }

  virtual inline uint32_t maxPrimitives() const override
  {
    return SIMFORTH_MAX_PRIMITIVES;
  }


protected:

  void emitForthInterpreteDone()
  {
    LOGS("emit signal Forth interprete done");
    signal_forth_interprete_done.emit(/**this*/);
  }

  ASpreadSheetCell *isACell(std::string const& word);
  virtual void interpreteWordCaseInterprete(std::string const& word) override;
  virtual void interpreteWordCaseCompile(std::string const& word) override;
  bool isACell(std::string const& word, Cell32& number);
  virtual void execPrimitive(const Cell16 idPrimitive) override;

private:

  SimForthDictionary m_dictionaries;
  std::vector<std::string> m_pathes;

public:

  ASpreadSheet *m_spreadsheet = nullptr;
  sigc::signal<void/*, SimForth&*/> signal_forth_interprete_done;
};

#endif /* SIMFORTH_HPP_ */
