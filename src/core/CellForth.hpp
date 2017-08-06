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

#ifndef CELLFORTH_HPP_
#  define CELLFORTH_HPP_

#  include "SimForth.hpp"

// **************************************************************
//
// **************************************************************
class CellForth
{
public:

  CellForth()
     : m_cost(0)
  {
  }

  //! \brief Accessor. Update the Forth word
  // FIXME: forth.compile(":" + m_name + " " + m_word);
  // FIXME dans un autre dico ?
  // FIXME: peut etre retarder la compile
  inline void forthWord(std::string const& word)
  {
    m_word = word;
    // m_compiled = false;
    // Marquer smudged l'ancienne def dans le dico ? Je ne pense pas
  }

  //! \brief Accessor. Update the Forth word
  inline void forthWord(const char *word)
  {
    m_word = word;
    // m_compiled = false;
  }

  //! \brief Accessor. Return the current Forth word.
  inline const std::string& forthWord() const
  {
    return m_word;
    // m_compiled = false;
  }

  //! \brief Clear the current Forth word
  inline void clearForthWord()
  {
    m_word.clear();
  }

  //! Interprete the current Forth word.
  virtual bool interprete(SimForth& forth)
  {
    LOGI("CellForth: interpreting the word '%s'. Cost function = %d",
         m_word.c_str(), m_cost);

    if (m_word.empty())
      return true;

    // FIXME: compiler le script dans le dico ?
    auto res = forth.interpreteCell(m_word, name(), m_cost);
    forth.ok(res);

    LOGI("Result %s. Cost function = %d", res.second.c_str(), m_cost);
    return res.first;
  }

  //virtual bool compile(SimForth& forth)
  //{
  //  forth.compile(":" + m_name + " " + m_word);
  //  m_compiled = true;
  //}

  //inline bool operator()()
  //{
  //  return interpreteWord();
  //}

  //! \brief Return the name of the cell.
  const std::string& name() const
  {
    return m_name;
  }

  // TODO: Ajouter dans le dictionnaire un alias sur l'ancien nom
  // (penser au cas particulier
  virtual void name(std::string const& name)
  {
    m_name = name;
  }

protected:

  virtual void setName() = 0;

public:

  Cell32 m_cost;

protected:

  //! \brief the Forth word to interprete
  std::string m_word;
  // FIXME: Cell32 m_token = FORTH_PRIMITIVE_NOP;
  //! \brief Forth name of the cell
  std::string m_name;
};

#endif /* CELLFORTH_HPP_ */
