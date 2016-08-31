#include "Forth.hpp"
#include <fstream>

#define CELL(x)  reinterpret_cast<Cell>(x)
#define CADDR(x) reinterpret_cast<Cell*>(x)
#define NEXT_MULTIPLE_OF_4(x) (((x) + 3) & ~0x03)
#define NEXT_MULTIPLE_OF_2(x) (((x) + 1) & ~0x01)

bool Forth::searchDicoEntry(const std::string& word, uint32_t& ptr)
{
  uint32_t next = 1;
  uint32_t length;

  ptr = last;
  while (0 != next)
  {
      length = dictionary[ptr] & 0x1f;
      if (length == NEXT_MULTIPLE_OF_2(word.size() + 1U))
        {
          if (0 == std::strncmp(word.c_str(), (char*) &dictionary[ptr + 1U], word.size()))
            {
              return true;
            }
        }

      next = dictionary[ptr + length] * 256U + dictionary[ptr + length + 1U];
      ptr = ptr - next;
    }

  return false;
}

bool Forth::createDicoEntry(const Cell token, const std::string& word, const bool immediate)
{
  uint32_t length = word.size();
  uint32_t ptr = here;

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
      dictionary[ptr++] = 0x00;
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

  // Update the last entry
  last = here;
  here = ptr;

  return true;
}

void Forth::dumpDico()
{
  std::ofstream out = std::ofstream("file.binary", std::ios::binary | std::ios::trunc);

  if (out.is_open())
    {
      out.write((char*) dictionary, here * sizeof (char));
    }
  out.close();
}

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
          if (0 == dictionary[ptr + i + 1U])
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
