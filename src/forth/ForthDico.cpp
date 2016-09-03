#include "Forth.hpp"

// **************************************************************
//
// **************************************************************
/*inline void Forth::checkDicoBoundaries(const Cell16* const ip, std::string const& funcName) const
{
  if ((ip <  CADDR(dictionary)) || (ip > CADDR(&dictionary[DATASPACE_SIZE])))
    {
      ForthDicoOOB e(ip, funcName); throw e;
    }
    }*/

// **************************************************************
// Convert a string into a token
// **************************************************************
bool Forth::toToken(const std::string& word, Cell16& token)
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
      // Compare name lengths before comparing strings
      length = dictionary[ptr] & MASK_FORTH_NAME_SIZE;
      if (length == NEXT_MULTIPLE_OF_2(word.size() + 1U))
        {
          // Same length, check if names mismatch
          if (0 == std::strncmp(word.c_str(), (char*) &dictionary[ptr + 1U], word.size()))
            {
              // word found in dictionnary
              token = dictionary[ptr + length + 2U] * 256U + dictionary[ptr + length + 3U];
              return true;
            }
        }

      // Not found: go to the previous word
      prev = dictionary[ptr + length] * 256U + dictionary[ptr + length + 1U];
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
bool Forth::createDicoEntry(const Cell16 token, const std::string& word, const bool immediate)
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
  dictionary[ptr++] = (1U << 7U) | (immediate << 6U) | ((uint8_t) reserved);

  // Store the word name
  uint32_t i;
  for (i = 0; i < length; ++i)
    {
      dictionary[ptr++] = word[i];
    }

  // Padding
  for (; i < reserved - 1U; ++i)
    {
      dictionary[ptr++] = 0xFF;
    }

  // Store the NFA of the preceding word
  uint32_t relative_address = here - last;
  if (relative_address > 66535U)
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
  uint32_t next = 1;
  uint32_t length, token, ptr, i;

  std::cout << "Display Dico: " << last
            << "\n------------------------------------\n";
  ptr = last;
  while (0 != next)
    {
      length = dictionary[ptr] & 0x1f;
      std::cout << "'";
      for (i = 0; i < length; ++i)
        {
          if (0xFF == dictionary[ptr + i + 1U])
            break;
          std::cout << dictionary[ptr + i + 1U];
        }
      std::cout  << "' ";
      next = dictionary[ptr + length] * 256U + dictionary[ptr + length + 1U];
      std::cout << "NFA: " << next;
      token = dictionary[ptr + length + 2U] * 256U + dictionary[ptr + length + 3U];
      std::cout << " Token: " << token << std::endl;

      ptr = ptr - next;
    }
}
