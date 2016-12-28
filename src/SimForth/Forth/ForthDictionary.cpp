#include "ForthDictionary.hpp"

// **************************************************************
//! Initialize dictionary states to obtain an empty dictionary
//! (with no Forth words inside).
// **************************************************************
ForthDictionary::ForthDictionary()
{
  m_here = 0U;
  m_last = 0U;
}

// **************************************************************
//! The new entry is stored at the end of the dictionary (the
//! location given ForthDictionary::m_here which is updated as well
//! ForthDictionary::m_last).
//! \param token either the CFA of a none primitive word or the value
//! of enum ForthPrimitives.
//! \param name of the Forth word.
//! \param immediate a boolean indicating if the word shall be interpreted when compiled.
//! \throw MalformedForthWord is the name of the word length is not <= 31
//! characters.
//! \throw NoSpaceDictionary if the dictionary is full.
// **************************************************************
void ForthDictionary::add(const Cell16 token, std::string const& name, const bool immediate)
{
  // Forth words are max 31 bytes long
  Cell32 length = name.size();
  if ((length > 31U) || (0U == length))
    {
      MalformedForthWord e(name); throw e;
    }

  // No more space in the m_dictionary ?
  if (length + 8U > DICTIONARY_SIZE) // 8U = padded(1: flags, 2: NFA, 2: token, 2: EXIT)
    {
      NoSpaceDictionary e; throw e;
    }

  Cell32 nfa = m_here - m_last;
  m_last = m_here;

  // Store the word header
  appendCell8((1U << 7U) | (immediate << 6U) | length);

  // Store the word name
  for (uint8_t i = 0; i < length; ++i)
    {
      appendCell8(name[i]);
    }

  // Store the NFA of the preceding word
  appendCell16(nfa);

  // Store the token
  appendCell16(token);
}

// **************************************************************
//! Convert a string into a token.
//! \param name (in) the name of the Forth word.
//! \param token (out) returns the token value if the word was found.
//! \param immediate (out) returns if the found word is immediate.
//! \return true if the word was found in the dictionary, else return false.
// **************************************************************
bool ForthDictionary::find(std::string const& name, Cell16& token, bool& immediate) const
{
  Cell32 nfa;
  Cell32 length;

  // last is a Cell16 but for computation with minus operator we need 32bits
  int32_t ptr = m_last;

  // 0 (aka NULL) meaning the last m_dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit */
      if (!(m_dictionary[ptr] & FLAG_SMUDGE))
        {
          // Compare name lengths before comparing strings
          if (length == name.size())
            {
              // Same length, check if names mismatch
              if (0 == std::strncmp(name.c_str(), (char*) &m_dictionary[ptr + 1U], length))
                {
                  // Set the param if the word is immediate
                  immediate = (m_dictionary[ptr] & FLAG_IMMEDIATE);

                  // Word found in dictionnary
                  token = read16at(ptr + length + 3U);
                  return true;
                }
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  return false;
}

// **************************************************************
//! \param last start the search at the LFA of a word (can be m_last for example).
//! \param partial_name the begining of a forth word.
//! \return the pair true + address of the first matching name, else return false + NULL.
// **************************************************************
std::pair<bool, const char*> ForthDictionary::completion(Cell16& last, std::string const& partial_name) const
{
  Cell32 nfa;
  Cell32 length;

  // last is a Cell16 but for computation with minus operator we need 32bits
  int32_t ptr = last;

  // 0 (aka NULL) meaning the last m_dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit */
      if (!(m_dictionary[ptr] & FLAG_SMUDGE))
        {
          // Check if names mismatch
          if (0 == std::strncmp(partial_name.c_str(),
                                (char*) &m_dictionary[ptr + 1U],
                                partial_name.length()))
            {
              // Go to the previous word
              nfa = read16at(ptr + length + 1U);
              ptr = ptr - nfa;
              last = (Cell16) ptr;

              return std::make_pair(true, (char*) &m_dictionary[ptr + 1U]);
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  last = (Cell16) ptr;
  return std::make_pair(false, nullptr);
}

// **************************************************************
//! Toggle the Smudge bit in the flags location.
//! \param name (in) the name of the Forth word.
//! \return true if the word exists (even hidden), else false.
// **************************************************************
bool ForthDictionary::smudge(std::string const& name)
{
  Cell32 nfa;
  Cell32 length;

  // last is a Cell16 but for computation with minus operator we need 32bits
  int32_t ptr = m_last;

  // 0 (aka NULL) meaning the last m_dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Compare name lengths before comparing strings
      if (length == name.size())
        {
          // Same length, check if names mismatch
          if (0 == std::strncmp(name.c_str(), (char*) &m_dictionary[ptr + 1U], length))
            {
              // Toogle the smudge bit
              m_dictionary[ptr] ^= FLAG_SMUDGE;
              return true;
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  return false;
}

// **************************************************************
//! \param name (in) the name of the Forth word.
//! \return ForthDictionary::find.
// **************************************************************
bool ForthDictionary::exists(std::string const& name) const
{
  Cell16 token;
  bool immediate;

  return ForthDictionary::find(name, token, immediate);
}

// **************************************************************
//! Looking for the Forth word name from its token.
//! \param token the CFA to look for in the dictionary.
//! \param even_smudge if false ignore smudged definitions.
//! \return the pair a boolean if the token was found and the address
//! of the name.
// **************************************************************
std::pair<bool, int32_t> ForthDictionary::find(const Cell16 token, const bool even_smudge) const
{
  Cell32 nfa;
  Cell32 length;

  // last is a Cell16 but for computation with minus operator we need 32bits
  int32_t ptr = m_last;

  // 0 (aka NULL) meaning the last m_dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  do
    {
      // Get the length of the forth name
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit */
      if ((0 == (m_dictionary[ptr] & FLAG_SMUDGE)) || (even_smudge))
        {
          // Compare name lengths before comparing strings
          if (token == read16at(ptr + length + 3U))
            {
              // Word found in dictionnary
              return std::make_pair(true, ptr);
            }
        }

      // Not found: go to the previous word
      nfa = read16at(ptr + length + 1U);
      ptr = ptr - nfa;
    } while (nfa);

  return std::make_pair(false, ptr);
}

// **************************************************************
//! Save the dictionnary in a binary file. The user can use the
//! command hexdump -C filename for debuging the dictionary.
//! \param filename the file name where the dictionary will be stored.
//! \return a boolean indicating if the process succeeded.
// **************************************************************
bool ForthDictionary::dump(std::string const& filename) // FIXME const
{
  std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);

  if (out.is_open())
    {
      // Hack: store LAST word at the end of the dictionnary to
      // be sure that LAST will be splited in correct endian but
      // break the const-ness of the function.
      // TODO: ajouter un param ou commence la sauvegarde dans le dico (ex: skip primitives)
      write16at(m_here, m_last);

      // Store all the dictionary including LAST
      out.write((char*) m_dictionary, (m_here + 2U) * sizeof (Cell8));
      out.close();
      return true;
    }
  else
    {
      std::cerr << "Cannot save the dictionary in file '"
                << filename
                << "'. Reason is '" << std::strerror(errno) << "'"
                << std::endl;
      return false;
    }
}

// **************************************************************
//! Load a dictionnary from a binary file.
//! \param the filename containing a dictionary. Note: no verification
//! is made for checking if the dictionary image is well formed or is
//! a real dictionary file.
//! \param replace if true the old dictionary is smashed else the
//! new dictionary is appened to the old one.
//! \return a boolean indicating if the process succeeded.
// **************************************************************
bool ForthDictionary::load(std::string const& filename, const bool replace)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);

  if (in.is_open())
    {
      // Get the length of file
      in.seekg(0, in.end);
      Cell32 length = in.tellg();
      in.seekg(0, in.beg);

      // Load the dictionary with LAST
      if (replace)
        {
          // Smash the old dictionary
          in.read((char*) m_dictionary, length);

          // Update Forth words LAST and HERE
          m_here = m_here - 2U; // 2U because LAST was stored in file
          m_last = read16at(m_here);
        }
      else
        {
          // Append the dictionary
          in.read((char*) m_dictionary + m_here, length);

          // Link the PFA of 1st entry of the new dictionary to
          // the PFA of the last entry of the previous dictionary
          Cell32 word_length = m_dictionary[m_here] & MASK_FORTH_NAME_SIZE;
          try
            {
              write16at(m_here + word_length + 1U, m_here - m_last);
            }
          catch (const OutOfBoundDictionary& e)
            {
              std::cerr << "Cannot load the dictionary from the file '"
                        << filename
                        << "'. Reason is the image is bigger than the dictionary size."
                        << std::endl;
              in.close();
              return false;
            }

          // Update Forth words LAST and HERE
          m_here = m_here + length - 2U; // 2U because LAST was stored in file
          m_last = length - 2U + read16at(m_here);
        }

      in.close();
      return true;
    }
  else
    {
      std::cerr << "Cannot load the dictionary from the file '"
                << filename
                << "'. Reason is '" << strerror(errno) << "'"
                << std::endl;
      return false;
    }
}

// **************************************************************
//! \param token
// **************************************************************
void ForthDictionary::displayToken(const Cell16 token) const
{
  std::pair<bool, int32_t> res = find(token);
  if (res.first)
    {
      std::cout << "'" << (char *) &m_dictionary[res.second + 1U] << "' ("
                << std::hex << (int) token << ')';
    }
  else
    {
      std::cout << "Bad token (" << std::setfill('0') << std::setw(4) << std::hex << token << ") ";
    }
}

// **************************************************************
//
// **************************************************************
void ForthDictionary::display(TextColor& color) const
{
  int def_length, length, token, nfa, ptr, code, prev, d, dd, grouping, skip;
  bool smudge, immediate;
  bool compiled = false;
  bool truncated = false;
  bool creat = false;

  std::cout << "Address                          Word  Token    Definition (tokens)    ";
  std::cout << std::setw(11 + 5 * (WORD_GROUPING - 4)) << "Translation" << std::endl;
  std::cout << std::setfill('=') << std::setw(83 + 5 * (WORD_GROUPING - 4));
  std::cout << " " << std::endl;

  prev = m_here;
  ptr = m_last;
  do
    {
      creat = false;

      // Display dictionary addresses in hex
      color.grey();
      std::cout << color << std::setfill('0') << std::setw(4) << std::hex << ptr << " ";

      // Get word flags
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;
      smudge = m_dictionary[ptr] & FLAG_SMUDGE;
      immediate = m_dictionary[ptr] & FLAG_IMMEDIATE;

      // Next word in the dictionary (relative address)
      nfa = read16at(ptr + length + 1U);

      // Code Pointer (Exec token)
      code = read16at(ptr + length + 3U);

      // Select color depending on word flag bits
      if (immediate)
        color.yellow();
      else if (smudge)
        color.grey();
      else if (code < NUM_PRIMITIVES)
        color.blue();
      else
        color.red();

      // Display word name (right centered)
      std::cout << std::setfill('.') << std::setw(38U - length)
                << color << " " << (char *) &m_dictionary[ptr + 1U]
                << " ";

      // Display Exec token
      if (!smudge) color.azul(); // Let color in grey for unused words
      std::cout << color << '(' << std::setfill('0') << std::setw(4) << std::hex << code << ")    ";

      // Word definition
      def_length = prev - ptr - length - 3U;

      // primitive
      if (code < NUM_PRIMITIVES)
        {
          color.white();
          std::cout << color << "primitive" << std::endl;
        }

      // Skip NFA
      d = dd = 2;
      while (d < def_length)
        {
          if (!smudge) color.white();

          // Display tokens in hexa
          grouping = WORD_GROUPING;
          while ((grouping--) && (d < def_length))
            {
              token = read16at(ptr + length + 3U + d);
              d += 2U;
              std::cout << color << std::setfill('0') << std::setw(4) << std::hex << token << " ";
            }

          // If number of tokens is not a multiple of WORD_GROUPIN add spaces
          ++grouping;
          if (grouping)
            {
              std::cout << color << std::setfill(' ') << std::setw(grouping * 5) << "    ";
            }
          std::cout << "   ";

          // Tokens name
          grouping = WORD_GROUPING;
          while ((grouping-- > 0) && (dd < def_length))
            {
              // (CREATE) EXIT xx .. yy
              if (creat)
                {
                  if (0 == skip)
                    {
                      uint32_t p = ptr + length + 3U + dd;
                      while (dd < def_length)
                        {
                          color.green();
                          std::cout << color << std::setfill('0') << std::setw(2) << std::hex << read8at(p) << " ";
                          ++p;
                          ++dd;
                        }
                      continue;
                    }
                  else
                    {
                      --skip; // skipping (CREATE) EXIT
                    }
                }

              if (truncated) { truncated = false; grouping--; }
              token = read16at(ptr + length + 3U + dd);
              dd += 2U;

              // Not a primitive: display the name
              if (token >= NUM_PRIMITIVES)
                {
                  std::pair<bool, int32_t> res = find(token, true);
                  if (!res.first)
                    {
                      // Not found. Display token in hex and in grey
                      color.green();
                      std::cout << color << std::setfill('0') << std::setw(4) << std::hex << token << " ";
                    }
                  else
                    {
                      // From NFA, go back to the begin of word definition
                      // To get flags
                      uint32_t j = token - 2U; // -2 fir skiping NFA
                      while (0 == (m_dictionary[j] & 0x80))
                        --j;

                      // Colorize
                      if ((smudge) || (m_dictionary[j] & FLAG_SMUDGE))
                        {
                          color.grey();
                        }
                      else
                        {
                          if (m_dictionary[j] & FLAG_IMMEDIATE)
                            color.yellow();
                          else
                            color.red();
                        }

                      // Display
                      std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                    }
                }
              else // primitive words
                {
                  std::pair<bool, int32_t> res = find(token, true);
                  if (!res.first)
                    {
                      // Not found. Display token in hex and in grey
                      color.grey();
                      std::cout << color << std::setfill('0') << std::setw(4) << std::hex << token << " ";
                    }
                  else
                    {
                      // Colorize
                      uint32_t j = res.second;
                      // FIXME: ugly but how to fix that ?
                      TextColor *color1 = color.clone();
                      color1->green();
                      if ((smudge) || (m_dictionary[j] & FLAG_SMUDGE))
                        {
                          color1->grey();
                          color.grey();
                        }
                      else if (m_dictionary[j] & FLAG_IMMEDIATE)
                        color.yellow();
                      else
                        color.blue();
                      uint32_t p = ptr + length + 4U + dd;

                      // (CREATE) EXIT xx .. yy
                      if ((FORTH_PRIMITIVE_PCREATE == token) && (FORTH_PRIMITIVE_EXIT == read16at(p-1)))
                        {
                          creat = true;
                          skip = 1;
                        }

                      // Several strategies
                      switch (token)
                        {
                        case FORTH_PRIMITIVE_COMPILE:
                          std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                          compiled = true;
                          break;
                        case FORTH_PRIMITIVE_0BRANCH:
                        case FORTH_PRIMITIVE_BRANCH:
                          std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                          if (compiled)
                            {
                              compiled = false;
                            }
                          else
                            {
                              uint16_t data16 = read16at(p);
                              dd += 2U;
                              --grouping;
                              std::cout << *color1 << std::setfill('0') << std::setw(4) << std::hex << data16 << " ";
                            }
                          break;
                        case FORTH_PRIMITIVE_LITERAL_16:
                          {
                            uint16_t data16 = read16at(p);
                            dd += 2U;
                            --grouping;
                            std::cout << *color1 << std::setfill('0') << std::setw(4) << std::hex << data16 << " ";
                          }
                          break;
                        case FORTH_PRIMITIVE_LITERAL_32:
                          {
                            uint32_t data32 = read32at(p);
                            dd += 4U;
                            --grouping;
                            std::cout << *color1 << std::setfill('0') << std::setw(8) << std::hex << data32 << " ";
                          }
                          break;
                        default:
                          compiled = false;
                          std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                          break;
                        }
                      truncated = (grouping < 0);
                      //if (truncated) std::cout << "|| ";
                      delete color1;
                    } // if found
                } // if primitive
            } // while grouping

          // End of line of group of words
          std::cout << std::endl;
          if (d < def_length - 1)
            {
              // Dictionary address
              color.grey();
              std::cout << color << std::setfill('0') << std::setw(4) << std::hex << ptr + length + 1U + d << "    ";
              std::cout << color << std::setfill(' ') << std::setw(40U) << "    ";
            }
        } // while definition

      // Go to the previous entry
      prev = ptr;
      ptr = ptr - nfa;
    } while (nfa);

  color.normal();
  std::cout << color << std::endl;
}

// **************************************************************
//! This function does not return a boolean but throw an exception
//! if the given range of address overflows/underflows the dictionary.
//! \param addr the dictionary to check
//! \param nb_bytes positive or negative offset to adr.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
void ForthDictionary::checkBounds(const uint32_t addr, const int32_t nb_bytes) const
{
  // FIXME: proteger en ecriture les anciens mots definis
  // FIXME: autoriser en lecture toutes les addr du dico
  if (/*(addr < m_here_at_colon) &&*/ (addr + nb_bytes >= DICTIONARY_SIZE))
    {
      OutOfBoundDictionary e(addr); throw e;
    }
}

// **************************************************************
//! \param data is a 32-bits data to store as Cell8 at the given
//! address.
//! \param addr the desired dictionary address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// FIXME: au lieu de addr >= &m_dictionary[NUM_PRIMITIVES]
// faire addr >= &m_dictionary[m_last + m_last_def_size]
// **************************************************************
void ForthDictionary::write8at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 0U);
  m_dictionary[addr] = (data >> 0) & 0xFF;
}

// **************************************************************
//! \param data is a 32-bits data to store as Cell16 at the given
//! address.
//! \param addr the desired dictionary address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
void ForthDictionary::write16at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 1U);
  m_dictionary[addr + 0U] = (data >> 8) & 0xFF;
  m_dictionary[addr + 1U] = (data >> 0) & 0xFF;
}

// **************************************************************
//! \param data is a 32-bits data to store at the given address.
//! \param addr the desired dictionary address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
void ForthDictionary::write32at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 3U);
  m_dictionary[addr + 0U] = (data >> 24) & 0xFF;
  m_dictionary[addr + 1U] = (data >> 16) & 0xFF;
  m_dictionary[addr + 2U] = (data >> 8) & 0xFF;
  m_dictionary[addr + 3U] = (data >> 0) & 0xFF;
}

// **************************************************************
//! \param addr the desired dictionary address.
//! \return the 8-bits data casted as Cell32 read at the given address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
Cell32 ForthDictionary::read8at(const uint32_t addr) const
{
  checkBounds(addr, 0U);
  return m_dictionary[addr];
}

// **************************************************************
//! \param addr the desired dictionary address.
//! \return the 16-bits data casted as Cell32 read at the given address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
Cell32 ForthDictionary::read16at(const uint32_t addr) const
{
  checkBounds(addr, 1U);
  Cell32 res =
    (m_dictionary[addr + 0U] << 8U) |
    (m_dictionary[addr + 1U] << 0U);

  return res;
}

// **************************************************************
//! \param addr the desired dictionary address.
//! \return the 32-bits data read at the given address.
//! \throw OutOfBoundDictionary if overflows/underflows is detected.
// **************************************************************
Cell32 ForthDictionary::read32at(const uint32_t addr) const
{
  // FIXME Cell8 *const addr
  checkBounds(addr, 3U);
  Cell32 res =
    (m_dictionary[addr + 0U] << 24UL) |
    (m_dictionary[addr + 1U] << 16UL) |
    (m_dictionary[addr + 2U] << 8UL) |
    (m_dictionary[addr + 3U] << 0UL);

  return res;
}

// **************************************************************
//! Reserve or release a consecutive number of bytes starting at
//! ForthDictionary::m_here. Then ForthDictionary::m_here is updated. Values
//! inside the reserved memory are not cleared.
//! \param nb_bytes the number of consecutive bytes needed: if > 0
//! memory is reserved, else if < 0 release the memory. if = 0 nothing
//! is made.
//! \throw OutOfBoundDictionary when attempting to go outside the dictionary bounds.
// **************************************************************
void ForthDictionary::allot(const int32_t nb_bytes)
{
  if (nb_bytes > 0)
    {
      checkBounds(m_here, nb_bytes);
      m_here = m_here + nb_bytes;
    }
  else if (nb_bytes < 0)
    {
      checkBounds(m_here - nb_bytes, nb_bytes);
      m_here = m_here - nb_bytes;
    }
  else // 0 == nb_bytes
    {
      // Do nothing
    }
}
