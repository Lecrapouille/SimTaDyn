#include "ForthDico.hpp"

ForthDico::ForthDico()
{
  m_here = 0U;
  m_last = 0U;
}

Cell16 ForthDico::last() const
{
  return m_last;
}

Cell16 ForthDico::here() const
{
  return m_here;
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
std::pair<bool, int32_t> ForthDico::find(const Cell16 token) const
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
bool ForthDico::dump(std::string const& filename) const
{
  std::ofstream out = std::ofstream(filename, std::ios::binary | std::ios::trunc);

  if (out.is_open())
    {
      out.write((char*) m_dictionary, m_here * sizeof (char));
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
// Uggly print of the dictionary
// **************************************************************
void ForthDico::display() const
{
  int def_length, length, token, nfa, ptr, code, prev, i, ii, kk, c;
  bool smudge, immediate;
  const char *color = DEFAULT;

  prev = m_here;
  ptr = m_last;
  do
    {
      // Dic addresses
      std::cout << GREY << std::setfill('0') << std::setw(4) << std::hex << ptr << " ";

      // word informations
      length = m_dictionary[ptr] & MASK_FORTH_NAME_SIZE;
      smudge = m_dictionary[ptr] & FLAG_SMUDGE;
      immediate = m_dictionary[ptr] & FLAG_IMMEDIATE;

      // Select color depending on word flag bits
      if (immediate)
        color = YELLOW;
      else if (smudge)
        color = GREY;
      else
        color = RED;

      // display word name (right centered)
      std::cout << std::setfill('.') << std::setw(32U - length)
                << color << " " << (char *) &m_dictionary[ptr + 1U]
                << " ";

      // Let color in grey for unused words
      if (!smudge)
        color = WHITE;

      // Next word in the dictionary (relative address)
      nfa = m_dictionary[ptr + length + 1U] * 256U + m_dictionary[ptr + length + 2U];

      // Code Pointer
      code = m_dictionary[ptr + length + 3U] * 256U + m_dictionary[ptr + length + 4U];
      std::cout << color << std::setfill('0') << std::setw(4) << std::hex << code << ": ";

      // Word definition grouped by 4 cell
      def_length = prev - ptr - length - 3U;
      for (i = 2; i < def_length; )
        {
          if (!smudge)
            color = WHITE;

          // Tokens in hexa
          c = 4U; ii = i; kk = 0;
          while ((c--) && (i < def_length))
            {
              token = m_dictionary[ptr + length + 3U + i] * 256U + m_dictionary[ptr + length + 4U + i];
              i += 2U;
              kk++;
              std::cout << color << std::setfill('0') << std::setw(4) << std::hex << token << " ";
            }

          // Align space
          if (kk < 4)
            {
              std::cout << color << std::setfill(' ') << std::setw((4 - kk) * 5) << " ";
            }

          // Tokens name
          c = 4U; i = ii;
          while ((c--) && (i < def_length))
            {
              token = m_dictionary[ptr + length + 3U + i] * 256U + m_dictionary[ptr + length + 4U + i];
              i += 2U;

              // Colorize
              if ((smudge) || (m_dictionary[token] & FLAG_SMUDGE))
                {
                  color = GREY;
                }
              else
                {
                  if (m_dictionary[token] & FLAG_IMMEDIATE)
                    color = YELLOW;
                  else if ((FORTH_PRIMITIVE_LITERAL_32 == token) ||
                           (FORTH_PRIMITIVE_LITERAL_16 == token))
                    color = BLUE;
                  else
                    color = RED;
                }

              if (token < NUM_PRIMITIVES)
                {
                  std::pair<bool, int32_t> res = find(token);
                  if (res.first)
                    {
                      std::cout << color << (char *) &m_dictionary[res.second + 1U] << " ";
                      if ((FORTH_PRIMITIVE_LITERAL_32 == token) ||
                          (FORTH_PRIMITIVE_LITERAL_16 == token))
                        {
                          uint32_t p = ptr + length + 4U + i;
                          c--;
                          if (FORTH_PRIMITIVE_LITERAL_32 == token)
                            {
                              uint32_t data32 = read32at(p);
                              std::cout << color << std::setfill('0') << std::setw(8) << std::hex << data32 << " ";
                              i += 4U;
                            }
                          else
                            {
                              uint16_t data16 = read16at(p);
                              std::cout << color << std::setfill('0') << std::setw(4) << std::hex << data16 << " ";
                              i += 2U;
                            }
                        }
                    }
                  else
                    {
                      std::cout << color << "XXX ";
                    }
                }
              else
                {
                  // from nfa, go back to begin of definition
                  uint32_t j = token - 2U; // 2: skip NFA
                  while (0 == (m_dictionary[j] & 0x80))
                    --j;
                  std::cout << color << (char *) &m_dictionary[j + 1U] << " ";
                }
            }

          std::cout << std::endl;
          if (i < def_length - 1)
            {
              std::cout << GREY << std::setfill('0') << std::setw(4) << std::hex << ptr + length + 1U + i << " ";
              std::cout << std::setfill(' ') << std::setw(29U + 6U) << " ";
            }
        }

      // primitive
      if (2U == def_length)
        std::cout << "primitive" << std::endl;

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
// Store at 8-byte data the first free location in the dictionary
// **************************************************************
void ForthDico::appendCell8(const Cell32 value)
{
  write8at(m_here, value);
  m_here += 1U;
}

// **************************************************************
// Store at 16-byte data the first free location in the dictionary
// **************************************************************
void ForthDico::appendCell16(const Cell32 value)
{
  write16at(m_here, value);
  m_here += 2U;
}

// **************************************************************
// Store at 32-byte data the first free location in the dictionary
// **************************************************************
void ForthDico::appendCell32(const Cell32 value)
{
  write32at(m_here, value);
  m_here += 4U;
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
