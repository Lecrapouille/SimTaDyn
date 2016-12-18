#include "ForthDico.hpp"

ForthDico::ForthDico()
{
  m_here = 0U;
  m_last = 0U;
}

// **************************************************************
// Create a new forth definition (aka entry) at the 1st empty location of the m_dictionary.
// The 1st empty location in the dictionnary is given by the variable 'here'.
// The last inserted entry is given by the variable 'last'.
// An entry is stored as:
// byte -1 ||       byte0         | byte1 .. byteN |     byteN+1 byteN+2       | byteN+3 .. byteM || byteM+1 ..
//  ..     || flags + word length | the word name  | Realtive @ Previous entry | word deffinition ||   ..
// All entries are consecutive.
// **************************************************************
void ForthDico::add(const Cell16 token, std::string const& word, const bool immediate)
{
  // Forth words are max 31 bytes long
  Cell32 length = word.size();
  if ((length > 31U) || (0U == length))
    {
      ForthMalformedWord e(word); throw e;
    }

  // No more space in the m_dictionary ?
  if (length + 8U > DICTIONARY_SIZE) // 8U = padded(1: flags, 2: NFA, 2: token, 2: EXIT)
    {
      ForthDicoNoSpace e; throw e;
    }

  Cell32 nfa = m_here - m_last;
  m_last = m_here;

  // Store the word header
  m_dictionary[m_here++] = (1U << 7U) | (immediate << 6U) | length;

  // Store the word name
  for (uint8_t i = 0; i < length; ++i)
    {
      m_dictionary[m_here++] = word[i];
    }

  // Store the NFA of the preceding word
  appendCell16(nfa);

  // Store the token
  appendCell16(token);
}

// **************************************************************
// Convert a string into a token
// **************************************************************
bool ForthDico::find(std::string const& word, Cell16& token, bool& immediate) const
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
          if (length == word.size())
            {
              // Same length, check if names mismatch
              if (0 == std::strncmp(word.c_str(), (char*) &m_dictionary[ptr + 1U], length))
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
// Smudge a word
// **************************************************************
bool ForthDico::smudge(std::string const& word)
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
      if (length == word.size())
        {
          // Same length, check if names mismatch
          if (0 == std::strncmp(word.c_str(), (char*) &m_dictionary[ptr + 1U], length))
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
//
// **************************************************************
bool ForthDico::exists(std::string const& word) const
{
  Cell16 token;
  bool immediate;

  return ForthDico::find(word, token, immediate);
}

// **************************************************************
// Convert a string into a token
// **************************************************************
std::pair<bool, int32_t> ForthDico::find(const Cell16 token, const bool even_smudge=false) const
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
// Save the dictionnary in a binary file.
// Use the commande hexdump -C filename for debugging it
// **************************************************************
bool ForthDico::dump(std::string const& filename) //const
{
  std::ofstream out(filename, std::ios::out | std::ios::binary | std::ios::trunc);

  if (out.is_open())
    {
      // Hack: store LAST word at the end of the dictionnary to
      // be sure that LAST will be splited in correct endian.
      write16at(m_here, m_last);

      // Store all the dictionary including LAST
      out.write((char*) m_dictionary, (m_here + 2U) * sizeof (Cell8));
      out.close();
      return true;
    }
  else
    {
      std::cerr << "Cannot dump the dico. Reason was xxx" << std::endl; // FIXME
      return false;
    }
}

// **************************************************************
// Load a dictionnary from a binary file.
// TBD: skip the dictionary ?
// **************************************************************
bool ForthDico::load(std::string const& filename)
{
  std::ifstream in(filename, std::ios::in | std::ios::binary);

  if (in.is_open())
    {
      // Get the length of file
      in.seekg(0, in.end);
      int length = in.tellg();
      in.seekg(0, in.beg);

      // Load the dictionary with LAST
      in.read((char*) m_dictionary, length);

      // Skip LAST and set HERE
      m_here = length - 2U;
      m_last = read16at(m_here);
      in.close();
      return true;
    }
  else
    {
      std::cerr << "Cannot load the file '" << filename << "'. Reason was xxx" << std::endl; // FIXME
      return false;
    }
}

// **************************************************************
//
// **************************************************************
void ForthDico::displayToken(const Cell16 token) const
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
// Pretty print of the dictionary
// **************************************************************
void ForthDico::display() const
{
  int def_length, length, token, nfa, ptr, code, prev, d, dd, grouping, skip;
  bool smudge, immediate;
  bool compiled = false;
  bool truncated = false;
  bool creat = false;
  const char *color = DEFAULT;
  const char *color1 = DEFAULT;

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
      std::cout << GREY << std::setfill('0') << std::setw(4) << std::hex << ptr << " ";

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
        color = YELLOW;
      else if (smudge)
        color = GREY;
      else if (code < NUM_PRIMITIVES)
        color = BLUE;
      else
        color = RED;

      // Display word name (right centered)
      std::cout << std::setfill('.') << std::setw(38U - length)
                << color << " " << (char *) &m_dictionary[ptr + 1U]
                << " ";

      // Display Exec token
      if (!smudge) color = AZUL; // Let color in grey for unused words
      std::cout << color << '(' << std::setfill('0') << std::setw(4) << std::hex << code << ")    ";

      // Word definition
      def_length = prev - ptr - length - 3U;

      // primitive
      if (code < NUM_PRIMITIVES)
        {
          std::cout << WHITE << "primitive" << std::endl;
        }

      // Skip NFA
      d = dd = 2;
      while (d < def_length)
        {
          if (!smudge) color = WHITE;

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
                          std::cout << GREEN << std::setfill('0') << std::setw(2) << std::hex << read8at(p) << " ";
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
                      std::cout << GREEN << std::setfill('0') << std::setw(4) << std::hex << token << " ";
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
                          color = GREY;
                        }
                      else
                        {
                          if (m_dictionary[j] & FLAG_IMMEDIATE)
                            color = YELLOW;
                          else
                            color = RED;
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
                      std::cout << GREY << std::setfill('0') << std::setw(4) << std::hex << token << " ";
                    }
                  else
                    {
                      // Colorize
                      uint32_t j = res.second;
                      color1 = GREEN;
                      if ((smudge) || (m_dictionary[j] & FLAG_SMUDGE))
                        {
                          color = color1 = GREY;
                        }
                      else if (m_dictionary[j] & FLAG_IMMEDIATE)
                        color = YELLOW;
                      else
                        color = BLUE;
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
                              std::cout << color1 << std::setfill('0') << std::setw(4) << std::hex << data16 << " ";
                            }
                          break;
                        case FORTH_PRIMITIVE_LITERAL_16:
                          {
                            uint16_t data16 = read16at(p);
                            dd += 2U;
                            --grouping;
                            std::cout << color1 << std::setfill('0') << std::setw(4) << std::hex << data16 << " ";
                          }
                          break;
                        case FORTH_PRIMITIVE_LITERAL_32:
                          {
                            uint32_t data32 = read32at(p);
                            dd += 4U;
                            --grouping;
                            std::cout << color1 << std::setfill('0') << std::setw(8) << std::hex << data32 << " ";
                          }
                          break;
                        default:
                          compiled = false;
                          std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                          break;
                        }
                      truncated = (grouping < 0);
                      //if (truncated) std::cout << "|| ";
                    } // if found
                } // if primitive
            } // while grouping

          // End of line of group of words
          std::cout << std::endl;
          if (d < def_length - 1)
            {
              // Dictionary address
              std::cout << GREY << std::setfill('0') << std::setw(4) << std::hex << ptr + length + 1U + d << "    ";
              std::cout << GREY << std::setfill(' ') << std::setw(40U) << "    ";
            }
        } // while definition

      // Go to the previous entry
      prev = ptr;
      ptr = ptr - nfa;
    } while (nfa);

  std::cout << DEFAULT << std::endl;
}

// **************************************************************
//
// **************************************************************
void ForthDico::checkBounds(const uint32_t addr, const int32_t nb_bytes) const
{
  // FIXME: proteger en ecriture les anciens mots definis
  // FIXME: autoriser en lecture toutes les addr du dico
  if (/*(addr < m_here_at_colon) &&*/ (addr + nb_bytes >= DICTIONARY_SIZE))
    {
      ForthDicoOOB e(addr); throw e;
    }
}

// **************************************************************
// FIXME: au lieu de addr >= &m_dictionary[NUM_PRIMITIVES]
// faire addr >= &m_dictionary[m_last + m_last_def_size]
// **************************************************************
void ForthDico::write8at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 0U);
  m_dictionary[addr] = (data >> 0) & 0xFF;
}

// **************************************************************
//
// **************************************************************
void ForthDico::write16at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 1U);
  m_dictionary[addr + 0U] = (data >> 8) & 0xFF;
  m_dictionary[addr + 1U] = (data >> 0) & 0xFF;
}

// **************************************************************
//
// **************************************************************
void ForthDico::write32at(const uint32_t addr, const Cell32 data)
{
  checkBounds(addr, 3U);
  m_dictionary[addr + 0U] = (data >> 24) & 0xFF;
  m_dictionary[addr + 1U] = (data >> 16) & 0xFF;
  m_dictionary[addr + 2U] = (data >> 8) & 0xFF;
  m_dictionary[addr + 3U] = (data >> 0) & 0xFF;
}

// **************************************************************
//
// **************************************************************
Cell32 ForthDico::read8at(const uint32_t addr) const
{
  checkBounds(addr, 0U);
  return m_dictionary[addr];
}

// **************************************************************
//
// **************************************************************
Cell32 ForthDico::read16at(const uint32_t addr) const
{
  checkBounds(addr, 1U);
  Cell32 res =
    (m_dictionary[addr + 0U] << 8U) |
    (m_dictionary[addr + 1U] << 0U);

  return res;
}

// **************************************************************
// FIXME Cell8 *const addr
// **************************************************************
Cell32 ForthDico::read32at(const uint32_t addr) const
{
  checkBounds(addr, 3U);
  Cell32 res =
    (m_dictionary[addr + 0U] << 24UL) |
    (m_dictionary[addr + 1U] << 16UL) |
    (m_dictionary[addr + 2U] << 8UL) |
    (m_dictionary[addr + 3U] << 0UL);

  return res;
}

// **************************************************************
// Reserve or release nb consecutive bytes from HERE
// **************************************************************
void ForthDico::allot(const int32_t nb_bytes)
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
