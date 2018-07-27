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

#ifndef FORTH_DICTIONARY_HPP_
#  define FORTH_DICTIONARY_HPP_

//------------------------------------------------------------------
//! \file This file defines the Forth dictionary which is virtual
//! machine containing the byte code.
//------------------------------------------------------------------

#  include "ForthTypes.hpp"

namespace forth
{

//------------------------------------------------------------------
//! \brief Memory size of the Forth dictionnary. Because addresses
//! are coded with forth::token (so 16 bits) the maximal reachable address
//! is 2^16 bits (= 64 ko).
//------------------------------------------------------------------
#  define FORTH_DICTIONARY_MAX_SIZE (1u << (8u * sizeof (forth::token)))
#  define FORTH_DICTIONARY_SIZE FORTH_DICTIONARY_MAX_SIZE

// **************************************************************
//! \brief Save information before starting compiling a new Forth
//! word. In case of error we can restore the context.
// **************************************************************
struct BackupContext
{
  //! Save the depth of the data stack.
  uint32_t                  depth;
  //! The Forth word currently in creation.
  std::string               name;
  //! Save the last valid dictionary entry (LASTEST word).
  forth::token              last;
  //! Save the last dictionary free slot before (HERE word).
  forth::token              here;
  //! \brief Save file line and column position.
  size_t                    line, column;
};

// **************************************************************
//! \brief The Forth dictionnary is a block of memory storing
//! known Forth words as well as their byt codes. it can be seen
//! as a virtual machine.
// **************************************************************
class Dictionary
{
private:

  friend class Forth;

public:

  //------------------------------------------------------------------
  //! \brief Constructor. Start with an empty dictionary.
  //------------------------------------------------------------------
  Dictionary();

  //------------------------------------------------------------------
  //! \brief Destructor.
  //------------------------------------------------------------------
  ~Dictionary();

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void save(std::string const& filename) const;

  //------------------------------------------------------------------
  //! \brief
  //------------------------------------------------------------------
  void load(std::string const& filename, const bool replace);

  void display(const int max_primitives) {} // FIXME: a finir

  //------------------------------------------------------------------
  //! \brief Read a byte at given address in the dictionnary.
  //------------------------------------------------------------------
  inline forth::cell read8at(forth::token const addr) const
  {
    return m_dictionary[addr];
  }

  //------------------------------------------------------------------
  //! \brief Store a byte at given address in the dictionnary.
  //------------------------------------------------------------------
  inline void write8at(forth::token const addr, forth::cell const data)
  {
    m_dictionary[addr] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Store a byte at the end of the dictionnary and m_here is
  //! moved. No check is made to check if m_here is outside the dictionary
  //! bounds.
  //! \param data is a 32-bits data (casted into Cell8) to store at
  //! location Dictionary::m_here
  //------------------------------------------------------------------
  inline void appendCell8(forth::cell const data)
  {
    m_dictionary[m_here++] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Read two consecutive bytes at given address in the
  //! dictionnary. Endianess is hiden.
  //------------------------------------------------------------------
  forth::cell read16at(forth::token const addr) const
  {
    return
      (m_dictionary[addr + 0U] << 8U) |
      (m_dictionary[addr + 1U] << 0U);
  }

  //------------------------------------------------------------------
  //! \brief Store two consecutive bytes at given address in the
  //! dictionnary. Endianess is hiden.
  //------------------------------------------------------------------
  inline void write16at(forth::token const addr, forth::cell const data)
  {
    m_dictionary[addr + 0U] = (data >> 8) & 0xFF;
    m_dictionary[addr + 1U] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Store two bytes at the end of the dictionnary and m_here is
  //! moved. No check is made to check if m_here is outside the dictionary
  //! bounds. Endianess is hiden.
  //! \param data is a 32-bits data (casted into forth::token) to store at
  //! location Dictionary::m_here
  //------------------------------------------------------------------
  inline void appendCell16(forth::cell const data)
  {
    m_dictionary[m_here++] = (data >> 8) & 0xFF;
    m_dictionary[m_here++] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Read four consecutive bytes at given address in the
  //! dictionnary. Endianess is hiden.
  //------------------------------------------------------------------
  inline forth::cell read32at(forth::token const addr) const
  {
    return
      (m_dictionary[addr + 0U] << 24UL) |
      (m_dictionary[addr + 1U] << 16UL) |
      (m_dictionary[addr + 2U] << 8UL) |
      (m_dictionary[addr + 3U] << 0UL);
  }

  //------------------------------------------------------------------
  //! \brief Store four consecutive bytes at given address in the
  //! dictionnary. Endianess is hiden.
  //------------------------------------------------------------------
  inline void write32at(forth::token const addr, forth::cell const data)
  {
    m_dictionary[addr + 0U] = (data >> 24) & 0xFF;
    m_dictionary[addr + 1U] = (data >> 16) & 0xFF;
    m_dictionary[addr + 2U] = (data >> 8) & 0xFF;
    m_dictionary[addr + 3U] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Store four bytes at the end of the dictionnary and m_here is
  //! moved. No check is made to check if m_here is outside the dictionary
  //! bounds. Endianess is hiden.
  //! \param data is a 32-bits data to store at location
  //! Dictionary::m_here
  //------------------------------------------------------------------
  inline void appendCell32(forth::cell const data)
  {
    m_dictionary[m_here++] = (data >> 24) & 0xFF;
    m_dictionary[m_here++] = (data >> 16) & 0xFF;
    m_dictionary[m_here++] = (data >> 8) & 0xFF;
    m_dictionary[m_here++] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Reserve or release a chunk of memory in the dictionary.
  //------------------------------------------------------------------
  void allot(const int32_t nb_bytes);

  //------------------------------------------------------------------
  //------------------------------------------------------------------
  std::pair<bool, int32_t>
  findToken(const forth::token token, const bool even_smudge) const;

  //------------------------------------------------------------------
  //! \brief Append a new Forth entry in the dictionary.
  //------------------------------------------------------------------
  void compileWord(const forth::token token, char const* name,
                   const uint32_t length, const bool immediate);

  //------------------------------------------------------------------
  //! \brief Append a new Forth entry in the dictionary.
  //------------------------------------------------------------------
  void compileWord(std::string const& word);

  //------------------------------------------------------------------
  //! \brief Append a literal value in the current word definition
  //------------------------------------------------------------------
  void compileLiteral(forth::cell const value);

  //------------------------------------------------------------------
  //! \brief Append a token value in the current word definition
  //------------------------------------------------------------------
  void compileToken(forth::token const token);

  //------------------------------------------------------------------
  //! \brief Look for a word and if found get its informations.
  //------------------------------------------------------------------
  bool find(std::string const& name, forth::token& token, bool& immediate) const;

  //------------------------------------------------------------------
  //! \brief Look for a word exits.
  //------------------------------------------------------------------
  inline bool find(std::string const& name) const
  {
    forth::token token; bool immediate;
    return find(name, token, immediate);
  }

  //------------------------------------------------------------------
  //! Look for a word name match form the partial name passed in param.
  //! Use for completing the begining of a Forth word.
  //------------------------------------------------------------------
  const char* completeWordName(std::string const& partial_name) const;

bool smudge(std::string const& name);

private:

  //------------------------------------------------------------------
  //! \brief Auxilary routine for Dictionary::completeWordName()
  //------------------------------------------------------------------
  const char* findPartialWord(forth::token& last, std::string const& partial_name) const;

  //FIXME
public:

  //------------------------------------------------------------------
  //! \brief Safe dictionnary context when starting the compilation of
  //! a new word. in case of failure the dictionary context can be
  //! restored.
  //------------------------------------------------------------------
  void saveContext(size_t line, size_t column);

  //------------------------------------------------------------------
  //! \brief Restore the dictionary context (to be called after a
  //! compilation failure).
  //------------------------------------------------------------------
  void restoreContext();

protected:

  //------------------------------------------------------------------
  //! \brief The memory of the dictionary containing the byte code.
  //------------------------------------------------------------------
  uint8_t   m_dictionary[FORTH_DICTIONARY_SIZE];

  //------------------------------------------------------------------
  //! \brief Name Field Address (NFA) of the most recently entry (Forth
  //! word: LAST).
  //------------------------------------------------------------------
  forth::token  m_last;

  //------------------------------------------------------------------
  //! \brief Address of the first free location in the dictionary (Forth
  //! word: HERE, DP).
  //------------------------------------------------------------------
  forth::token  m_here;

  //------------------------------------------------------------------
  //! \brief Save dictionary states before compiling a new word. In
  //! case of failure, we can restore dictionary in the previous state.
  //------------------------------------------------------------------
  BackupContext m_save;
};

} // namespace

#endif // FORTH_DICTIONARY_HPP_
