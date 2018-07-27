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

#ifndef ASPREADSHEETCELL_HPP_
#  define ASPREADSHEETCELL_HPP_

#  include "Logger.hpp"
#  include "SimTaDynForth.hpp"
#  include "ClassCounter.tpp"
#  include <vector>

// **************************************************************
//! \brief Define an Excel-like spreadsheet cell.
// **************************************************************
class ASpreadSheetCell // FIXME: public IObserver, public Observable
{
public:

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  ASpreadSheetCell(std::string const& formulae)
    : m_formulae(formulae),
      m_evaluatedValue(0),
      m_evaluated(false),
      m_unresolvedRefs(0),
      m_dataKey(0)
  {
    // parse(); // Risque de crash si la cellule n'est pas encore connue
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  virtual ~ASpreadSheetCell()
  {
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  void formulae(std::string const& formulae)
  {
    //m_modified = true;
    m_formulae = formulae;
    parse();
  }

  void debugReferences() const
  {
    std::cout << "'" << m_formulae << "' has Ref:";
    for (const auto &it: m_references)
      {
        std::cout << "   '" << it->formulae() << "'";
      }
    std::cout << std::endl;
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  inline bool hasReferences() const
  {
    //std::cout << "hasReferences() " << m_unresolvedRefs << std::endl;
    return m_unresolvedRefs > 0;// m_references.size() > 0;
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  inline const std::vector<ASpreadSheetCell *> &references() const
  {
    return m_references;
  }

  void addReference(ASpreadSheetCell& cell)
  {
    //cell.addObserver(this);
    m_references.push_back(&cell);
  }

  void reset()
  {
    m_evaluatedValue = 0;
    m_evaluated = false;
    m_references.clear();
  }

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  inline int32_t rawValue() const
  {
    return m_evaluatedValue;
  }

  inline int32_t& rawValue()
  {
    return m_evaluatedValue;
  }

  inline std::pair<bool, int32_t> value() const
  {
    return std::make_pair(m_evaluated, m_evaluatedValue);
  }

  inline std::string& formulae()
  {
    return m_formulae;
  }

  void value(const int32_t val)
  {
    m_evaluatedValue = val;
    m_evaluated = true;
    //setChanged();
    //notifyObservers();
  }

  //virtual void update(Observable o, Object arg) override
  void update();

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  virtual const std::string name() const = 0;
  virtual Key id() const = 0;

public: // FIXME private:

  //-------------------------------------------------------------
  //! \brief
  //-------------------------------------------------------------
  void parse();
  //FIXME void parse(SimForth &forth);

private:

  std::string              m_formulae;
  int32_t                  m_evaluatedValue;
  bool                     m_evaluated;
  std::vector<ASpreadSheetCell *> m_references;

public:
  uint32_t                 m_unresolvedRefs;

protected:

  // TODO: for the moment we managed a single interger value
  //! \brief Key to database
  Key m_dataKey;
};

#endif /* ASPREADSHEETCELL_HPP_ */
