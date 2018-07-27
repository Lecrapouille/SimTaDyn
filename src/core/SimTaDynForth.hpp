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

#ifndef SIMFORTH_HPP_
#  define SIMFORTH_HPP_

#  include "ASpreadSheet.hpp"
#  include "Forth.hpp"
#  include "SimTaDynForthPrimitives.hpp"
//#  include "Names.hpp"

class ASpreadSheetCell;
class ASpreadSheet;

class SimForthDictionary : public forth::Dictionary
{
public:

  SimForthDictionary()
    : forth::Dictionary()
  {
    LOGI("Creating SimForthDictionary");
  }
};

class SimForth : public Forth, public Singleton<SimForth>
{
  friend class Singleton<SimForth>;

public:

  ASpreadSheet *m_spreadsheet = nullptr;

  virtual void boot() override;
  void compileCell(ASpreadSheetCell &cell);
  void evaluate(ASpreadSheet& spreadsheet);
  std::pair<bool, std::string>
  interpreteCell(ASpreadSheetCell &cell);
  bool parseCell(ASpreadSheetCell &cell);

protected:

  ASpreadSheetCell *isACell(std::string const& word);
  virtual void interpreteWordCaseInterprete(std::string const& word) override;
  virtual void interpreteWordCaseCompile(std::string const& word) override;
  bool isACell(std::string const& word, forth::cell& number);

  virtual inline uint32_t maxPrimitives() const override
  {
    return SIMFORTH_MAX_PRIMITIVES;
  }

  virtual void execPrimitive(const forth::token idPrimitive) override
  {
    switch (idPrimitive)
      {
      case SIMFORTH_PRIMITIVE_TOTO:
        std::cout << "TOTOTOTOTO\n";
        break;
      default:
        Forth::execPrimitive(idPrimitive);
        break;
      }
  }

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

protected:

  SimForthDictionary m_dictionaries;
};

#endif /* SIMFORTH_HPP_ */
