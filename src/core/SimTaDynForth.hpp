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
  : public Forth,
    public Singleton<SimForth>
{
  friend class Singleton<SimForth>;

public:

  ASpreadSheet *m_spreadsheet = nullptr;

  virtual void ok(std::pair<bool, std::string> const& res) override
  {
    Forth::ok(res);
    emitForthInterpreteDone();
  }

  virtual void boot() override;
  void compileCell(ASpreadSheetCell &cell);
  void evaluate(ASpreadSheet& spreadsheet);
  std::pair<bool, std::string>
  interpreteCell(ASpreadSheetCell &cell);
  bool parseCell(ASpreadSheetCell &cell);

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

  SimForth()
    : Forth(m_dictionaries)
  {
    LOGI("Creating SimForth");
  }

  virtual ~SimForth()
  {
    LOGI("Destroying SimForth");
  }

private:

  SimForthDictionary m_dictionaries;

public:

  sigc::signal<void/*, SimForth&*/> signal_forth_interprete_done;
};

#endif /* SIMFORTH_HPP_ */
