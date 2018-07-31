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

#include "ClassicSpreadSheet.hpp"
#include <sstream>
#include <string>

ClassicSpreadSheet::~ClassicSpreadSheet()
{
  for (size_t row = 0; row < m_row; ++row)
    {
      for (size_t col = 0; col < m_col; ++col)
        {
          delete m_cellMatrix[row][col];
        }
    }
}

// Regular expression for an Excel cell name is: ([a-zA-Z]+)(\\d+)
static bool regexpCell(std::string const& word)
{
  bool res = false;
  size_t i = 0;

  // Regexp: [a-zA-Z]+
  while ((word[i] != '\0') && (word[i] >= 'A') && (word[i] <= 'Z'))
    ++i;

  // Regexp: \d+
  if (i >= 1)
    {
      size_t j = i;
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

  size_t row = 0;
  size_t col = 0;
  size_t i = 0;

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

ASpreadSheetCell* ClassicSpreadSheet::addCell(const size_t row,
                                              const size_t col,
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

bool ClassicSpreadSheet::readInput(std::string const filename)
{
  LOGD("Read file '%s'", filename.c_str());
  std::ifstream infile(filename);
  if (infile.fail())
    {
      std::cerr << "Failed opening the file '" << filename
                << "'" << std::endl;
      return false;
    }
  std::string line;

  // Get spreadsheet dimension array
  std::getline(infile, line);
  std::string::size_type sz;
  m_col = std::stoi(line.c_str(), &sz, 10);
  m_row = std::atoi(line.c_str() + sz);

  // Resize the size of the spreadsheet
  m_cellMatrix.resize(m_row);
  for (size_t i = 0; i < m_row; ++i)
    {
      m_cellMatrix[i].resize(m_col);
    }

  // Fill spreadsheet cells
  for (size_t row = 0; row < m_row; ++row)
    {
      for (size_t col = 0; col < m_col; ++col)
        {
          try
            {
              std::getline(infile, line);
            }
          catch (std::exception const& e)
            {
              std::cerr << "Failed reading the file '" << filename
                        << "'" << std::endl;
              return false;
            }
          addCell(row, col, line); //si cellule pas trouvee => a parser plus tard
        }
    }
  return true;
}

void ClassicSpreadSheet::displayResult()
{
  std::cout << "SpreadSheet result:" << std::endl;
  for (size_t row = 0; row < m_row; ++row)
    {
      for (size_t col = 0; col < m_col; ++col)
        {
          std::cout << m_cellMatrix[row][col]->rawValue() << "  ";
        }
      std::cout << std::endl;
    }
}
