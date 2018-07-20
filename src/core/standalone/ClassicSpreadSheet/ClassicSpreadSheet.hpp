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

#ifndef CLASSIC_SPREADSHEET_HPP_
#  define CLASSIC_SPREADSHEET_HPP_

#  include "ASpreadSheet.hpp"
#  include "SimTaDynCell.hpp"

// **************************************************************
// A mettre dans un dossier standalone comme petit Excel light
// **************************************************************
class ClassicSpreadSheet: public ASpreadSheet
{
public:

  ClassicSpreadSheet(std::string const& name)
    : ASpreadSheet(), m_name(name), m_row(0), m_col(0),
      m_itrow(0), m_itcol(0)
  {
  }

  ~ClassicSpreadSheet();

  virtual const std::string& name() const override
  {
    return m_name;
  }

  virtual ASpreadSheetCell *isACell(std::string const& word) override;
  bool readInput(std::string const filename);
  void displayResult();

  std::vector<std::vector<ASpreadSheetCell*>> const&
  cells() const
  {
    return m_cellMatrix;
  }

  inline int rawValue(const uint32_t row, const uint32_t col) const
  {
    return m_cellMatrix[row][col]->rawValue();
  }

  inline  std::pair<bool, int32_t> value(const uint32_t row, const uint32_t col) const
  {
    return m_cellMatrix[row][col]->value();
  }

protected:

  ASpreadSheetCell* addCell(const uint32_t row,
                            const uint32_t col,
                            std::string const& formulae);

  virtual void resetCellIterator() override
  {
    m_itrow = 0;
    m_itcol = 0;
  }

  virtual bool hasCell() const override
  {
    return (m_itrow != m_row) && (m_itcol != m_col);
  }

  virtual ASpreadSheetCell* nextCell() /*const*/ override
  {
    ASpreadSheetCell* cell = m_cellMatrix[m_itrow][m_itcol];
    //std::cout << "nextCell: " << cell->name() << ": '" << cell->formulae() << "'" << std::endl;
    ++m_itcol;
    if (m_itcol >= m_col)
      {
        m_itcol = 0;
        ++m_itrow;
      }

    return cell;
  }

  virtual uint32_t howManyCells() const override
  {
    return m_row * m_col;
  }

  //private:
public:

  std::string m_name;
  std::vector<std::vector<ASpreadSheetCell*>> m_cellMatrix;
  uint32_t m_row;
  uint32_t m_col;
  mutable uint32_t m_itrow;
  mutable uint32_t m_itcol;
};

#endif
