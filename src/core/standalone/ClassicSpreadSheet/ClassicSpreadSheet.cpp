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

#include "ClassicSpreadSheet.hpp"
#include "PathManager.hpp"
#include <sstream>
#include <string>

ClassicSpreadSheet::~ClassicSpreadSheet()
{
  for (uint32_t row = 0; row < m_row; ++row)
    {
      for (uint32_t col = 0; col < m_col; ++col)
        {
          delete m_cellMatrix[row][col];
        }
    }
}

// Regular expression for an Excel cell name is: ([a-zA-Z]+)(\\d+)
static bool regexpCell(std::string const& word)
{
  bool res = false;
  uint32_t i = 0;

  // Regexp: [a-zA-Z]+
  while ((word[i] != '\0') && (word[i] >= 'A') && (word[i] <= 'Z'))
    ++i;

  // Regexp: \d+
  if (i >= 1)
    {
      uint32_t j = i;
      while ((word[i] != '\0') && (word[i] >= '0') && (word[i] <= '9'))
        ++i;
      res = ((i >= j + 1) && (word[i] == '\0'));
    }

  return res;
}

ASpreadSheetCell *ClassicSpreadSheet::isACell(std::string const& word)
{
  //std::cerr << "isACell '" << word << "'" << std::endl;

  // Fast check
  if (!regexpCell(word))
    {
      //std::cerr << "KO1" << std::endl;
      return nullptr;
    }

  uint32_t row = 0;
  uint32_t col = 0;
  uint32_t i = 0;

  // Base 26
  while ((word[i] >= 'A') && (word[i] <= 'Z'))
    {
      row = row * 26U + word[i] - 'A' + 1;
      ++i;
    }

  // Base 10
  while ((word[i] != '\0') && (word[i] >= '0') && (word[i] <= '9'))
    {
      col = col * 10U + word[i] - '0';
      ++i;
    }

  --row;
  --col;
  if ((row > m_row) || (col > m_col))
    {
      //std::cerr << "KO" << word << " while is a cell: " << row << ", " << col << std::endl;
      return nullptr;
    }

  //std::cout << "OK " << word << " is a cell: " << row << ", " << col << std::endl;
  return m_cellMatrix[row][col];
}

ASpreadSheetCell* ClassicSpreadSheet::addCell(const uint32_t row,
                                              const uint32_t col,
                                              std::string const& formulae)
{
  //std::cerr << "new CellNode " << formulae << std::endl;
  ASpreadSheetCell* node = new CellNode(formulae);
  m_cellMatrix[row][col] = node;

  // Parcequ'on utilise ASpreadSheetCell * au lieu de ID la cellule
  // peut ne pas encore etre definie
  // node->parse();
  return node;
}

void ClassicSpreadSheet::readInput(std::string const filename)
{
  std::ifstream infile(filename);
  std::string line;

  // Get spreadsheet dimension array
  std::getline(infile, line);
  std::string::size_type sz;
  m_col = std::stoi(line.c_str(), &sz, 10);
  m_row = std::atoi(line.c_str() + sz);

  // Resize the size of the spreadsheet
  m_cellMatrix.resize(m_row);
  for (uint32_t i = 0; i < m_row; ++i)
    {
      m_cellMatrix[i].resize(m_col);
    }

  // Fill spreadsheet cells
  for (uint32_t row = 0; row < m_row; ++row)
    {
      for (uint32_t col = 0; col < m_col; ++col)
        {
          std::getline(infile, line);
          addCell(row, col, line); //si cellule pas trouvee => a parser plus tard
        }
    }
}

void ClassicSpreadSheet::displayResult()
{
  std::cout << "SpreadSheet result:" << std::endl;
  for (uint32_t row = 0; row < m_row; ++row)
    {
      for (uint32_t col = 0; col < m_col; ++col)
        {
          std::cout << m_cellMatrix[row][col]->value() << "  ";
        }
      std::cout << std::endl;
    }
}

int main()
{
  PathManager::instance();

  SimForth& forth = SimForth::instance();
  ClassicSpreadSheet sheet("Sheet1");

  forth.boot();
  sheet.readInput("examples/input1.txt");

  LOGI("-----");
  sheet.parse(forth);
  std::pair<bool, std::string> res = sheet.evaluate(forth);
  forth.ok(res);
  if (res.first)
    {
      sheet.displayResult();
    }

  return 0;
}
