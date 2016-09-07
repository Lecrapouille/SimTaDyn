#include "Forth.hpp"

// **************************************************************
//
// **************************************************************
void Forth::checkDicoBoundaries(Cell16* const ip, std::string const& funcName) const
{
  Cell8* ip8 = reinterpret_cast<Cell8*>(ip);

  if ((ip8 < dictionary) || (ip8 > &dictionary[DATASPACE_SIZE]))
    {
      ForthDicoOOB e(ip, funcName); throw e;
    }
}

// **************************************************************
//
// **************************************************************
void Forth::dicoAppendCell16(Cell16 value)
{
  //std::cout << "dicoAppendCell16 " << value << "\n";
  dictionary[here++] = value / 256U;
  dictionary[here++] = value & 255U;
}

// **************************************************************
//
// **************************************************************
void Forth::writeCell16at(Cell8 *addr, Cell16 data)
{
  *addr++ = data >> 8;
  *addr = data;
}

// **************************************************************
//
// **************************************************************
Cell16 Forth::readCell16at(const Cell8 *addr)
{
  Cell16 val = (addr[0] << 8) | addr[1];
  //std::cout << "readCell16at " << val << std::endl;
  return val;
}

// **************************************************************
//
// **************************************************************
Cell16 Forth::readCell16at(const Cell16 *addr)
{
  return readCell16at(reinterpret_cast<const Cell8*>(addr));
}


// **************************************************************
// Convert a string into a token
// **************************************************************
bool Forth::toToken(std::string const& word, Cell16& token, Cell16& immediate)
{
  Cell32 prev = 1; // Previous forth word
  Cell32 length;

  // last is a Cell16 but for computation with minus operator we need 32bits
  Cell32 ptr = last;

  // 0 (aka NULL) meaning the last dictionary entry.  Because we are
  // using relative addresses as uint16_t to save space we cannot use
  // NULL
  while (0 != prev)
    {
      // Get the length of the forth name
      length = dictionary[ptr] & MASK_FORTH_NAME_SIZE;

      // Ignore words with the SMUDGE bit */
      if (!(dictionary[ptr] & FLAG_SMUDGE))
        {
          // Compare name lengths before comparing strings
          if ((length + 1U) == NEXT_MULTIPLE_OF_2(word.size() + 1U))
            {
              // Same length, check if names mismatch
              if (0 == std::strncmp(word.c_str(), (char*) &dictionary[ptr + 1U], word.size()))
                {
                  // Word found in dictionnary
                  token = dictionary[ptr + length + 3U] * 256U + dictionary[ptr + length + 4U];

                  // Set the param if the word is immediate
                  immediate = (dictionary[ptr] & FLAG_IMMEDIATE);
                  return true;
                }
            }
        }

      // Not found: go to the previous word
      prev = dictionary[ptr + length + 1U] * 256U + dictionary[ptr + length + 2U];
      ptr = ptr - prev;
    }

  // std::cerr << "Unknow word '" << word << "'" << std::endl;
  return false;
}

// **************************************************************
// Create a new forth definition (aka entry) at the 1st empty location of the dictionary.
// The 1st empty location in the dictionnary is given by the variable 'here'.
// The last inserted entry is given by the variable 'last'.
// An entry is stored as:
// byte -1 ||       byte0         | byte1 .. byteN |     byteN+1 byteN+2       | byteN+3 .. byteM || byteM+1 ..
//  ..     || flags + word length | the word name  | Realtive @ Previous entry | word deffinition ||   ..
// All entries are consecutive.
// **************************************************************
bool Forth::createDicoEntry(const Cell16 token, std::string const& word, const bool immediate)
{
  Cell32 length = word.size();
  Cell32 ptr = here;

  // Forth words are max 30 bytes long
  if (length > 31U)
    {
      std::cerr << "[ERR] The word length " << word << " is greater than 32 caracters." << std::endl;
      return false;
    }

  // Count the header size plus the string length plus the padding aligning address
  uint32_t reserved = NEXT_MULTIPLE_OF_2(length + 1U);

  // No more space in the dictionary ?
  if (reserved + 8U > DATASPACE_SIZE)
    {
      std::cerr << "[ERR] No more space in the dictionnary." << std::endl;
      return false;
    }

  // Store the word header
  dictionary[ptr++] = (1U << 7U) | (immediate << 6U) | ((uint8_t) reserved - 1U);

  // Store the word name
  uint32_t i;
  for (i = 0; i < length; ++i)
    {
      dictionary[ptr++] = word[i];
    }

  // Padding
  for (; i < reserved - 1U; ++i)
    {
      dictionary[ptr++] = SPARE_VALUE;
    }

  // Store the NFA of the preceding word
  uint32_t relative_address = here - last;
  if (relative_address > CELL16_MAX_VALUE)
    {
      std::cerr << "[ERR] I cannot create a new word containing more than 2^16 words." << std::endl;
    }

  dictionary[ptr++] = relative_address / 256U;
  dictionary[ptr++] = relative_address & 255U;

  // Store the token
  dictionary[ptr++] = token / 256U;
  dictionary[ptr++] = token & 255U;

  // Update the last entry and be sure to have aligned addresses
  last = here;
  here = NEXT_MULTIPLE_OF_2(ptr);

  return true;
}

// **************************************************************
// Save the dictionnary in a binary file.
// Use the commande hexdump -C filename for debugging it
// **************************************************************
bool Forth::dumpDico(std::string const& filename)
{
  std::ofstream out = std::ofstream(filename, std::ios::binary | std::ios::trunc);

  if (out.is_open())
    {
      out.write((char*) dictionary, here * sizeof (char));
      out.close();
      return true;
    }
  else
    {
      std::cerr << "Cannot dump the dico. Reason was xxx" << std::endl;
      return false;
    }
}

// **************************************************************
// Uggly print of the dictionary
// **************************************************************
void Forth::displayDico()
{
  uint32_t cphere = here;
  uint32_t next = 1;
  uint32_t def_length, length, token, ptr, i;
  bool smudge, immediate;

  std::cout << "Display Dico (LAST: " << last << ", HERE: " << here << ")"
            << "\n------------------------------------\n";

  // Iterate all words from the last entry
  ptr = last;
  while (0 != next)
    {
      // Get word name and length name
      length = dictionary[ptr] & MASK_FORTH_NAME_SIZE;
      std::cout << "At " << ptr << " (" << (uint32_t*) &dictionary[ptr] << "): ";
      for (i = 0; i < length; ++i)
        {
          // Ignore spared length used for address alignement
          if (SPARE_VALUE == dictionary[ptr + i + 1U])
            break;
          std::cout << dictionary[ptr + i + 1U];
        }
      std::cout  << " ";

      // NFA of the previous word
      next = dictionary[ptr + length + 1U] * 256U + dictionary[ptr + length + 2U];
      std::cout << "(LFA: " << next << ") ";

      // Display flags
      smudge = dictionary[ptr] & FLAG_SMUDGE;
      immediate = dictionary[ptr] & FLAG_IMMEDIATE;
      if (immediate) std::cout  << "<I> ";
      if (smudge) std::cout  << "<S> ";

      // Tokens
      def_length = cphere - ptr - length - 3U;
      std::cout << (def_length / 2U) << " token(s):";
      for (i = 0; i < def_length; i += 2)
        {
          token = dictionary[ptr + length + 3U + i] * 256U + dictionary[ptr + length + 4U + i];
          std::cout << " #" << token;
        }
      std::cout << std::endl;

      // Go to the previous entry
      cphere = ptr;
      ptr = ptr - next;
    }
}
