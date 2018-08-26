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
//! \file ForthDictionary.hpp
//! \brief This file defines the class for the Forth dictionary: a
//! continuous block of memory used for saving Forth definitions as
//! byte code.
//------------------------------------------------------------------

#  include "ForthTypes.hpp"

namespace forth
{

//------------------------------------------------------------------
//! \brief Memory maximum size for the Forth dictionnary. Because
//! addresses are coded with forth::token (16 bits) the maximal
//! reachable address is 2^16 bits = 64 KB.
//------------------------------------------------------------------
#  define FORTH_DICTIONARY_MAX_SIZE (1u << (8u * sizeof (forth::token)))

//------------------------------------------------------------------
//! \brief Allow to define a customized dictionary size. Requested size
//! is checked during the compilation for avoiding pathological case.
//------------------------------------------------------------------
#  ifndef FORTH_DICTIONARY_SIZE
#    define FORTH_DICTIONARY_SIZE FORTH_DICTIONARY_MAX_SIZE
#  endif

// **************************************************************
//! \brief Save some dictionary states before compiling a new Forth
//! word. In case of error, the dictionary state can be restored.
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
//! \brief The Forth dictionary is a block of memory for storing
//! Forth word definitions. Forth words are compiled into tokens
//! (also known as byte code for Java). Dictionary can be seen
//! as a virtual machine for the whole Forth system.
// **************************************************************
class Dictionary
{
private:

  friend class Forth;

public:

  //------------------------------------------------------------------
  //! \brief Constructor. Start with a totaly dummy dictionary. You
  //! have to call boot() for having minimalist Forth environement.
  //------------------------------------------------------------------
  Dictionary();

  //------------------------------------------------------------------
  //! \brief Destructor.
  //------------------------------------------------------------------
  ~Dictionary();

  //------------------------------------------------------------------
  //! \brief Save the dictionnary in a binary file. The user can use
  //! the command hexdump -C filename for debuging the dictionary.
  //------------------------------------------------------------------
  void save(std::string const& filename) const;

  //------------------------------------------------------------------
  //! \brief Load a Forth dictionnary from a binary file.
  //------------------------------------------------------------------
  void load(std::string const& filename, const bool replace);

  //------------------------------------------------------------------
  //! \brief Pretty print the Forth dictionary in the console. Colors
  //! are used for helping understanding the developer but can be
  //! disabled with std::cout << termcolor::state::Off
  //------------------------------------------------------------------
  void display();

  //------------------------------------------------------------------
  //! \brief Read a byte at given address in the dictionnary.
  //! No dictionary out of bound security is performed against incorrect
  //! address.
  //! \param addr the address of the dictionary.
  //! \return the read byte converted as Forth cell.
  //------------------------------------------------------------------
  inline forth::cell read8at(forth::token const addr) const
  {
    return m_dictionary[addr];
  }

  //------------------------------------------------------------------
  //! \brief Store a byte at given address in the dictionnary.
  //! No dictionary out of bound security is performed against incorrect
  //! address.
  //! \param addr the address of the dictionary.
  //------------------------------------------------------------------
  inline void write8at(forth::token const addr, forth::cell const data)
  {
    m_dictionary[addr] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Store a byte at the end of the dictionnary indicated by
  //! m_here, then move m_here of one byte. No check is made to check
  //! if m_here is outside the dictionary bounds.
  //! \param data is a 32-bits data (casted into Cell8) to store at
  //! location Dictionary::m_here
  //------------------------------------------------------------------
  inline void appendCell8(forth::cell const data)
  {
    m_dictionary[m_here++] = (data >> 0) & 0xFF;
  }

  //------------------------------------------------------------------
  //! \brief Read two consecutive bytes at given address in the
  //! dictionnary. Endianess and memory alignement are hiden.
  //! No dictionary out of bound security is performed against incorrect
  //! address.
  //! \param addr the address of the dictionary.
  //! \return the read bytes converted as Forth cell.
  //------------------------------------------------------------------
  forth::cell read16at(forth::token const addr) const
  {
    return
      (m_dictionary[addr + 0U] << 8U) |
      (m_dictionary[addr + 1U] << 0U);
  }

  //------------------------------------------------------------------
  //! \brief Store two consecutive bytes at given address in the
  //! dictionnary. Endianess and memory alignement are hiden.
  //! \param addr the address of the dictionary.
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
  //! dictionnary. Endianess and memory alignement are hiden.
  //! No dictionary out of bound security is performed against incorrect
  //! address.
  //! \param addr the address of the dictionary.
  //! \return the read bytes converted as Forth cell.
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
  //! Bytes are keep intact. This method just displace m_here.
  //------------------------------------------------------------------
  void allot(const int32_t nb_bytes);

  //------------------------------------------------------------------
  //! \brief Look for the
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
  //void compileLiteral(forth::cell const value);

  //------------------------------------------------------------------
  //! \brief Append a token value in the current word definition.
  //! This is an alias method for appendToken().
  //------------------------------------------------------------------
  void compileToken(forth::token const token);

  //------------------------------------------------------------------
  //! \brief Look for a word and if found get its informations.
  //------------------------------------------------------------------
  bool find(std::string const& name, forth::token& token, bool& immediate) const;

  //------------------------------------------------------------------
  //! \brief Look for a word exits. This method is equivalent to find()
  //! but result of the method are ignored.
  //------------------------------------------------------------------
  inline bool find(std::string const& name) const
  {
    forth::token token; bool immediate;
    return find(name, token, immediate);
  }

  //------------------------------------------------------------------
  //! \brief Look for if the partial Forth name given in param matches
  //! an word entry in the dictionnary. This function is used for
  //! completing a forth name when the developper is writting a Forth
  //! script.
  //------------------------------------------------------------------
  const char* completeWordName(std::string const& partial_name) const;

  //------------------------------------------------------------------
  //! \brief Hide the forth definition (if present in the dictionnary)
  //! or active it if the definition was hidden. This function allows
  //! the Forth developper to discard and restore a Forth definition.
  //! The definition is not deleted from the dictionnary but just hide.
  //------------------------------------------------------------------
  bool smudge(std::string const& name);

  //------------------------------------------------------------------
  //! \brief Set immediate the last valid forth definition inserted in
  //! the dictionnary.
  //------------------------------------------------------------------
  void immediate();

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

  //! \brief Accessor. Return the most recent entry in the dictionary.
  inline forth::token last() const { return m_last; }

  //! \brief Accessor. Return the address of the first free location in the dictionary.
  inline forth::token here() const { return m_here; }

protected:

  //------------------------------------------------------------------
  //! \brief The memory of the dictionary containing Forth definitions
  //! compiled as byte code.
  //------------------------------------------------------------------
  uint8_t   m_dictionary[FORTH_DICTIONARY_SIZE];

  //------------------------------------------------------------------
  //! \brief Name Field Address (NFA) of the most recently entry (Forth
  //! word: LAST). This is the head of the linked list of Forth word
  //! names used for searching a word.
  //------------------------------------------------------------------
  forth::token  m_last;

  //------------------------------------------------------------------
  //! \brief Address of the first free location in the dictionary (Forth
  //! word: HERE, DP). It is used for appendind byte code in the dictionary.
  //------------------------------------------------------------------
  forth::token  m_here;

  //------------------------------------------------------------------
  //! \brief Save dictionary states before compiling a new word. In
  //! case of failure, we can restore dictionary to its nominal state.
  //------------------------------------------------------------------
  BackupContext m_save;
};

} // namespace

#endif // FORTH_DICTIONARY_HPP_
