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

//#include "MapEditor.hpp"
#include "SimTaDynForth.hpp"
#include "PathManager.hpp"

void SimForth::boot()
{
  LOGI("Booting SimForth");

  // Start the Forth core
  Forth::boot();

  // Add specialized words for SimTaDyn
  m_dictionary.add(SIMFORTH_PRIMITIVE_TOTO, FORTH_DICO_ENTRY("TOTO"), 0);

  // Initialize basic Forth system
  std::pair<bool, std::string> res
    = Forth::interpreteFile(PathManager::instance().expand("forth/system.fs"));
  ok(res);

  if (res.first)
    {
      LOGI("Forth interpreter: %s", res.second.c_str());
    }
  else
    {
      LOGW("Forth interpreter: %s", res.second.c_str());
    }

  // Debug
  // Forth::displayDictionary(m_color);
}

void SimForth::compileCell(ASpreadSheetCell &/*cell*/)
{
  // Compile and add into the dictionary
  // forth.compile(cell.name(), cell.formulae());
}

void SimForth::evaluate(ASpreadSheet& spreadsheet)
{
  // Disable compilation mode
  //dictionary().smudge(":");
  //dictionary().smudge("INCLUDE");

  m_spreadsheet = &spreadsheet;
  spreadsheet.evaluate(*this);

  // Enable compilation mode
  //dictionary().smudge("INCLUDE");
  //dictionary().smudge(":");
}

std::pair<bool, std::string>
SimForth::interpreteCell(ASpreadSheetCell &cell)
{
  m_err_stream = 0;
  STREAM.loadString(cell.formulae(), cell.name());

  Cell32 value;
  std::pair<bool, std::string> res = parseStream();
  if (true == res.first)
    {
      try
        {
          DPOP(value);
          isStackUnderOverFlow(forth::DataStack);
          cell.value(value);
        }
      catch (ForthException const& e)
        {
          res = std::make_pair(false, e.message());
        }
    }

  std::cout << "interpretCell '" << cell.formulae() << "': "
            << res.second << std::endl;
  if (true == res.first)
    {
      std::cout << "Result " << value << std::endl;
    }
  return res;
}

bool SimForth::parseCell(ASpreadSheetCell &cell)
{
  bool res = true;
  std::string word;
  ASpreadSheetCell *c;

  LOGD("parseCell %s: %s", cell.name().c_str(),
       cell.formulae().c_str());

  //assert(nullptr != m_spreadsheet);
  if (nullptr == m_spreadsheet)
    {
      LOGD("No spreadsheet attached to a Forth interpreter");
      return false;
    }

  m_err_stream = 0;
  STREAM.loadString(cell.formulae(), cell.name());

  try
    {
      while (STREAM.hasMoreWords())
        {
          c = isACell(STREAM.nextWord());
          if (nullptr != c)
            {
              cell.addReference(*c);
            }
        }
    }
  catch (ForthException const& e)
    {
      cell.reset();
      res = false;
    }

  return res;
}

ASpreadSheetCell *SimForth::isACell(std::string const& word)
{
  if (nullptr == m_spreadsheet)
    return nullptr;
  return m_spreadsheet->isACell(word);
}

void SimForth::interpreteWordCaseInterprete(std::string const& word)
{
  ASpreadSheetCell *c;

  if ((c = isACell(word)) != nullptr)
    {
      //std::cout << "interpreteWordCaseInterprete: cell " << word << std::endl;
      auto cell = c->value();
      if (!cell.first)
        {
          abort("Cell not yet evaluated");
        }
      Cell32 number = cell.second;
      DPUSH(number);
      isStackUnderOverFlow(forth::DataStack);
    }
  else
    {
      //std::cout << "interpreteWordCaseInterprete: forth " << word << std::endl;
      Forth::interpreteWordCaseInterprete(word);
    }
}

void SimForth::interpreteWordCaseCompile(std::string const& word)
{
  ASpreadSheetCell *c;

  if ((c = isACell(word)) != nullptr)
    {
      // FIXME: temporaire car on ne va pas que gerer la fonction cout
      auto cell = c->value();
      if (!cell.first)
        {
          abort("Cell not yet evaluated");
        }
      Cell32 number = cell.second;
      if (number <= 65535U)
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_16);
          m_dictionary.appendCell16(number);
        }
      else
        {
          m_dictionary.appendCell16(FORTH_PRIMITIVE_LITERAL_32);
          m_dictionary.appendCell32(number);
        }
    }
  else
    {
      Forth::interpreteWordCaseCompile(word);
    }
}
