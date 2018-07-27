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

#include "ForthUtils.hpp"
#include <cstdlib>
#include <exception>
#include <stdexcept>
#include <climits>

namespace forth
{

#define ERR_CURRENT_BASE_SHALL_BE_LESS_THAN_33 "Current base shall be < 33"
#define ERR_BADLY_FORMED_NUMBER                "Badly formed number"
#define ERR_OUT_OF_RANGE_NUMBER                "Out of range number"
#define WARN_TRUNCATE_OUT_OF_RANGE_NUMBER      "Truncate out of range number"

// **************************************************************
//! \param word (in) the number to convert
//! \param number (out) the result of the conversion if the function
//! returned true (else the number is undefined).
//! \return false if the number is malformed (not a number in the
//! current base or too huge to store in a cell.
//! \note: prefixes 'b', 'h' and '0x' are not Forth standard.
// **************************************************************
bool toNumber(std::string const& word, uint8_t current_base, forth::cell& number, const char*& msg)
{
  int base = current_base;
  uint32_t negative = 0;
  uint32_t i = 0;

  // sign
  if ('-' == word[i])
    {
      ++i;
      negative = 1;
    }
  else if ('+' == word[i])
    {
      ++i;
    }

  // decimal
  if (('&' == word[i]) || ('#' == word[i]))
    {
      ++i;
      base = 10;
    }
  // binary ('b' is non standard)
  else if (('b' == word[i]) || ('%' == word[i]))
    {
      ++i;
      base = 2;
    }
  // hexadecimal ('h' is non standard)
  else if (('h' == word[i]) || ('$' == word[i]))
    {
      ++i;
      base = 16;
      if (current_base >= 33)
        {
          msg = ERR_CURRENT_BASE_SHALL_BE_LESS_THAN_33;
          return false;
        }
    }
  // hexadecimal, if base < 33.
  else if (('0' == word[i]) && ('x' == word[i + 1]))
    {
      if (current_base < 33)
        {
          i += 2U;
          base = 16;
        }
      else
        {
          msg = ERR_CURRENT_BASE_SHALL_BE_LESS_THAN_33;
          return false;
        }
    }
  // numeric value (e.g., ASCII code) an optional ' may be present
  // after the character
  else if ('\'' == word[i])
    {
      int s = word.size();
      if ((2 == s) || ((3 == s) && ('\'' == word[i + 2])))
        {
          number = (negative ? -word[i + 1] : word[i + 1]);
          return true;
        }
      msg = ERR_BADLY_FORMED_NUMBER;
      return false;
    }

  // Try to convert the string into number
  try
    {
      std::size_t sz;
      unsigned long val = std::stoul(word.substr(i, word.length() - i), &sz, base);
      number = (negative ? -val : val);
      return (sz + i) == word.length();
    }
  catch (std::invalid_argument const& ia)
    {
      // Do not set error message because we let the Forth interpreter to do it
      return false;
    }
  catch (std::out_of_range const& oor)
    {
      // Two strategies:
#if (FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_TRUNCATE_OUT_OF_RANGE_NUMBERS)
      // 1st strategy: runcate the number with a warning message to
      // avoid abort the program.
      number = (negative ? LONG_MIN : LONG_MAX);
      msg = WARN_TRUNCATE_OUT_OF_RANGE_NUMBER;
      return true;
#else // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE == FORTH_OUT_OF_RANGE_NUMBERS_ARE_WORDS
      // 2nd strategy: do not consider the word as a number which
      // probably result an "unknown word" error message.
      msg = ERR_OUT_OF_RANGE_NUMBER;
      return false;
#endif // FORTH_BEHAVIOR_NUMBER_OUT_OF_RANGE
    }
}

}
