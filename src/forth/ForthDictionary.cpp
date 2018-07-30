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

#include "Logger.hpp"
#include "ForthStream.hpp"
#include "ForthDictionary.hpp"
#include "ForthExceptions.hpp"
#include "ForthTerminalColor.hpp"
#include <cstring> // for memset

namespace forth
{

//------------------------------------------------------------------
//! Original Forth structure is:
//! - 1 byte for flags:
//!   - 8th bit: always set to 1 for finding the begining of the word
//!              definition in the dictionary.
//!   - 7th bit: precedence bit: when set to 1 the word is immediatly
//!              computed even if the interpreter is in compilation
//!              mode.
//!   - 6th bit: smudge bit: when set to 1 that means the word is valid
//!   - 5th bit -- 1st bit: are used for storing the word name length.
//------------------------------------------------------------------
#define STARTING_BIT          (0x80) // A word has always this bit set (historical)
#define IMMEDIATE_BIT         (0x40) // A word immediate is interpreted during the compilation
#define SMUDGE_BIT            (0x20) // When set this word is forgotten by the dictionnary
#define MASK_FORTH_NAME_SIZE  (0x1F) // 5 first bits are used to store the word name length
#define MASK_FORTH_FLAGS      (0x7F) // Mask for removing STARTING_BIT and word name length

//------------------------------------------------------------------
//! Memory operation helpers
//------------------------------------------------------------------
//! \brief Bit swapping for changing the endianness of a 32-bit word
#define BSWAP32(x) (((x) & 0xff) << 24 | ((x) & 0xff00) << 8 | ((x) & 0xff0000) >> 8 | ((x) >> 24) & 0xff)
//! \brief Bit swapping for changing the endianness of a 16-bit word
#define BSWAP16(x) (((x) & 0xff) << 8 | (((x) & 0xff00) >> 8))
//! \brief Used for aligning 32-bits addresses
#define NEXT_MULTIPLE_OF_4(x) (((x) + 3) & ~0x03)
//! \brief Used for aligning 16-bits addresses
#define NEXT_MULTIPLE_OF_2(x) (((x) + 1) & ~0x01)

//------------------------------------------------------------------
Dictionary::Dictionary()
{
  LOGI("Creating Forth dictionnary");
  static_assert(FORTH_DICTIONARY_SIZE <= FORTH_DICTIONARY_MAX_SIZE);
  m_here = 0U;
  m_last = 0U;
  std::memset(m_dictionary, 0u, FORTH_DICTIONARY_SIZE);
}

Dictionary::~Dictionary()
{
  LOGI("Destroying Forth dictionnary");
}

//------------------------------------------------------------------
//! Save the dictionnary in a binary file. The user can use the
//! command hexdump -C filename for debuging the dictionary.
//! \param filename the file name where the dictionary will be stored.
//! \return a boolean indicating if the process succeeded.
//------------------------------------------------------------------
void Dictionary::save(std::string const& filename) const
{
  std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);

  if (!out.is_open())
    throw ForthException(MSG_EXCEPTION_FORTH_SAVE_DICO_NO_FILE(filename));

  // Store all the dictionary
  out.write((char*) m_dictionary, m_here + forth::token_size);
  out.close();

  // Append LAST word
  uint8_t buffer[2] = { uint8_t(m_last >> 8), uint8_t(m_last) }; // FIXME: casse si token != Cell16
  out.write((char*) buffer, sizeof (buffer));
}

//------------------------------------------------------------------
//! Load a dictionnary from a binary file.
//! \param filename the filename containing a dictionary. Note: no
//! verification is made for checking if the dictionary image is well
//! formed or is a real dictionary file.
//! \param replace if true the old dictionary is smashed else the
//! new dictionary is appened to the old one.
//! \return a boolean indicating if the process succeeded.
//------------------------------------------------------------------
void Dictionary::load(std::string const& filename, const bool replace)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);

  if (!in.is_open())
    throw ForthException(MSG_EXCEPTION_FORTH_LOAD_DICO_NO_FILE(filename));

  // Get the length of file
  in.seekg(0, in.end);
  uint32_t length(in.tellg());
  in.seekg(0, in.beg);

  // Empty file ?
  if (length == 0)
    return ;

  // Failure ?
  if (((int) length) < 0)
    throw ForthException(MSG_EXCEPTION_FORTH_FAILED_LOADING_DICO(filename));

  // Load the dictionary containing an additional token: the content
  // of Forth word LAST.
  if (replace)
    {
      // Check for space
      if (length > FORTH_DICTIONARY_SIZE)
        throw ForthException(MSG_EXCEPTION_FORTH_LOAD_DICO_NO_SPACE(filename));

      // Smash the old dictionary
      in.read((char*) m_dictionary, length);

      // Update Forth words LAST and HERE.
      // Remove token_size because LAST was stored in file.
      m_here = m_here - forth::token_size;
      m_last = read16at(m_here);
    }
  else
    {
      // Check for space
      if (m_here + length > FORTH_DICTIONARY_SIZE)
        throw ForthException(MSG_EXCEPTION_FORTH_LOAD_DICO_NO_SPACE(filename));

      // Append the dictionary
      in.read((char*) m_dictionary + m_here, length);

      // Link the PFA of 1st entry of the new dictionary to
      // the PFA of the last entry of the previous dictionary
      forth::cell word_length = m_dictionary[m_here] & MASK_FORTH_NAME_SIZE;
      write16at(m_here + word_length + 1U, m_here - m_last);

      // Update Forth words LAST and HERE.
      // Remove token_size because LAST was stored in file.
      m_here = m_here + length - forth::token_size;
      m_last = length - forth::token_size + read16at(m_here);
    }
}

//------------------------------------------------------------------
//! The new Forth word entry is stored at the end of the dictionary
//! (the location given Dictionary::m_here which is updated as
//! well Dictionary::m_last).
//!
//! \param token either the CFA of a none primitive word or the enum
//! of a Forth primitive.
//!
//! \param name of the Forth word.
//!
//! \param immediate a boolean indicating if the word shall be interpreted
//! during the compilation.
//!
//! \throw MalformedForthWord is the name of the word length is not <= 31
//! characters.
//! \throw NoSpaceDictionary if the dictionary is full.
//------------------------------------------------------------------
void Dictionary::compileWord(const forth::token token, char const* name,
                                  const uint32_t length, const bool immediate)
{
  // Forth words are max 31 bytes long
  if (unlikely((MASK_FORTH_NAME_SIZE < length) || (0U == length)))
    throw ForthException(MSG_EXCEPTION_FORTH_NAME_TOO_LONG(name));

  // No more space in the m_dictionary ?
  // 8U = padding_of(1 bit: flags + 2 bits: NFA + 2 bits: token + 2: EXIT) // FIXME: casse si token != Cell16
  if (unlikely(m_here + length + 8U > FORTH_DICTIONARY_SIZE))
    throw ForthException(MSG_EXCEPTION_FORTH_DICTIONARY_IS_FULL);

  // Words are stored as list link
  forth::cell lfa = m_here - m_last;
  m_last = m_here;

  // Store the word header
  appendCell8(STARTING_BIT | (immediate ? IMMEDIATE_BIT : 0) | length);

  // Store the word name
  for (uint8_t i = 0; i < length; ++i)
    appendCell8(name[i]);

  // Store the link with the preceding word
  appendToken(lfa);

  // Store the execution token (allow to distinguish between primitive
  // and user word
  appendToken(token);
}

//------------------------------------------------------------------
//! The new Forth word entry is stored at the end of the dictionary
//! (the location given Dictionary::m_here which is updated as
//! well Dictionary::m_last).
//!
//! \param name of the Forth word.
//------------------------------------------------------------------
void Dictionary::compileWord(std::string const& word)
{
  forth::token size = m_here + word.size() + 3U; // 3U: flags + NFA // FIXME: casse si token != Cell16
  compileWord(size, word.c_str(), word.size(), false);
}

//------------------------------------------------------------------
//! \param token.
//------------------------------------------------------------------
void Dictionary::compileToken(forth::token const token)
{
  appendToken(token);
}

//------------------------------------------------------------------
//! \param value.
//------------------------------------------------------------------
void Dictionary::compileLiteral(forth::cell const value)
{
  // Optimize dictionary size
  if (value <= 65535U)
    {
      appendToken(FORTH_PRIMITIVE_LITERAL_16);
      appendCell16(value);
    }
  else
    {
      appendToken(FORTH_PRIMITIVE_LITERAL_32);
      appendCell32(value);
    }
}

//------------------------------------------------------------------
//! Look for a word and if found get its informations: token, immediate.
//! \param name (in) the name of the Forth word to look for.
//! \param token (out) returns the token value if the word was found.
//! \param immediate (out) returns if the found word is immediate.
//! \return true if the word was found in the dictionary, else return false.
//! and in this case do use paramtoken and immediate their values are
//! undefined.
//------------------------------------------------------------------
bool Dictionary::find(std::string const& name, forth::token& token, bool& immediate) const
{
  LOGI("find %s\n", name.c_str());
  forth::cell nfa;

  // m_last contains the NFA of the last inserted word. So it is a
  // forth::token but for computation with minus operator we need 32bits
  int32_t ptr = m_last;

  // 0 (aka NULL) meaning the last m_dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  do
    {
      LOGI("find ptr %d", ptr);

      // Get the length of the forth name
      forth::token length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit
      if (likely(0 == (m_dictionary[ptr] & SMUDGE_BIT)))
        {
          // Compare name lengths before comparing strings
          if (unlikely(length == name.size()))
            {
              // Same length, check if names match
              if (unlikely(0 == std::strncmp(name.c_str(), (char*) &m_dictionary[ptr + 1U], length)))
                {
                  // Set the param if the word is immediate
                  immediate = (m_dictionary[ptr] & IMMEDIATE_BIT);

                  // Success: word found in the dictionnary
                  const forth::token cfa = ptr + length + 3U;
                  token = read16at(cfa);
                  return true;
                }
            }
        }

      // Not found: go to the NFA of previous word by reading the LFA
      const forth::token lfa = ptr + length + 1U;
      nfa = read16at(lfa);
      ptr = ptr - nfa;
    } while (nfa);

  // Not found !
  return false;
}

//------------------------------------------------------------------
//! Look for a word name match form the partial name passed in param.
//! \param partial_name (in) the name of the Forth word to look for.
//! \param last (in/out) initaly m_last but then the last position of
//! the NFA of the word matching param partial_name.
//! \return if found return the Forth name completed else return nullptr.
//------------------------------------------------------------------
const char* Dictionary::findPartialWord(forth::token& last, std::string const& partial_name) const
{
  forth::cell nfa;
  forth::cell length;
  int32_t ptr = last;

  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit
      if (likely(0 == (m_dictionary[ptr] & SMUDGE_BIT)))
        {
          // Check if names mismatch
          if (unlikely(0 == std::strncmp(partial_name.c_str(),
                                         (char*) &m_dictionary[ptr + 1U],
                                         partial_name.length())))
            {
              // Return the match
              nfa = read16at(ptr + length + 1U);
              last = (forth::token) ptr - nfa;

              return (char*) &m_dictionary[ptr + 1U];
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  last = (forth::token) ptr;
  return nullptr;
}

//------------------------------------------------------------------
//! \param last (in/out) initaly m_last but then the last position of
//! the NFA of the word matching param partial_name.
//! \return if found return the Forth name completed else return nullptr.
//------------------------------------------------------------------
const char* Dictionary::completeWordName(std::string const& partial_name) const
{
  static forth::token last = m_last;
  const bool initial_position = (last == m_last);
  const char* complete_name = findPartialWord(last, partial_name);
  if (unlikely(nullptr == complete_name))
    {
      last = m_last;
      if (!initial_position)
        complete_name = findPartialWord(last, partial_name);
    }
  return complete_name;
}

//------------------------------------------------------------------
//!
std::pair<bool, int32_t>
Dictionary::findToken(const forth::token token, const bool even_smudge) const
{
  forth::cell nfa;
  int32_t ptr = m_last;

  do
    {
      // Get the length of the forth name
      forth::cell length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit (if not explicitly asked)
      if (unlikely((0 == (m_dictionary[ptr] & SMUDGE_BIT)) || (even_smudge)))
        {
          // Success: found the token !
          const forth::token cfa = ptr + length + 3U;
          if (unlikely(token == read16at(cfa)))
            return std::make_pair(true, ptr);

          // Not found: go to the previous word
          const forth::token lfa = ptr + length + 1U;
          nfa = read16at(lfa);
          // Relative address
          ptr = ptr - nfa;
        }
    } while (nfa);

  // Not found !
  return std::make_pair(false, 0);
}

//------------------------------------------------------------------
//! TODO: ajouter des securites (m_last defini) comme sur gofth ?
void Dictionary::immediate()
{
  m_dictionary[m_dictionary.m_last] |= FLAG_IMMEDIATE;
}

//------------------------------------------------------------------
//!
bool Dictionary::smudge(std::string const& name)
{
  forth::cell nfa;
  forth::cell length;
  int32_t ptr = m_last;

  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Compare name lengths before comparing strings
      if (unlikely(((size_t) length) == name.size()))
        {
          // Same length, check if names mismatch
          if (unlikely(0 == std::strncmp(name.c_str(),
                                         (char*) &m_dictionary[ptr + 1U],
                                         length)))
            {
              // Toogle the smudge bit
              m_dictionary[ptr] ^= SMUDGE_BIT;
              std::cout << FORTH_WARNING_COLOR
                        << ((m_dictionary[ptr] & SMUDGE_BIT) ? "smudged " : "unsmudged ")
                        << name << "'" << FORTH_NORMAL_COLOR << std::endl;
              return true;
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  std::cout << FORTH_WARNING_COLOR << "[WARNING] Cannot toogle smudge bit of "
    " unknown word " << name << FORTH_NORMAL_COLOR << std::endl;
  return false;
}

//------------------------------------------------------------------
//! Reserve or release a consecutive number of bytes starting at
//! Dictionary::m_here. Then Dictionary::m_here is updated. Values
//! inside the reserved memory are not cleared.
//! \param nb_bytes the number of consecutive bytes needed: if > 0
//! memory is reserved, else if < 0 release the memory. if = 0 nothing
//! is made.
//! \throw OutOfBoundDictionary when attempting to go outside the dictionary bounds.
//------------------------------------------------------------------
void Dictionary::allot(const int32_t nb_bytes)
{
  if (unlikely(nb_bytes == 0))
    return ;

  // checkBounds(m_here, nb_bytes);
  if (likely(nb_bytes > 0))
    {
      m_here += nb_bytes;
    }
  else // (nb_bytes < 0)
    {
      m_here -= nb_bytes;
    }
}

//------------------------------------------------------------------
void Dictionary::saveContext(size_t line, size_t column)
{
  //m_save.depth = m_data_stack.depth();
  //m_save.name = word;
  m_save.last = m_last;
  m_save.here = m_here;
  m_save.line = line;//STREAM.position().first;
  m_save.column = column;//STREAM.position().second;
}

//------------------------------------------------------------------
void Dictionary::restoreContext()
{
  //m_data_stack.depth() = m_save.depth;
  m_last = m_save.last;
  m_here = m_save.here;
}

} // namespace
