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

#include "ForthStream.hpp"

// **************************************************************
//! When calling this constructor, no stream was opened. You need
//! to call ForthStream::setFileToParse() or ForthStream::setStringToParse()
//! to init a stream.
// **************************************************************
ForthStream::ForthStream()
{
  // 64 is enough for storing correct Forth words (32 char max)
  m_word.reserve(64U);
  m_mode = NOTHING_TO_READ;
  init();
  m_str = "";
  m_filename = "";
  m_base = 10; // Useless but safer
}

// **************************************************************
//!
// **************************************************************
ForthStream::~ForthStream()
{
  ForthStream::close();
}

// **************************************************************
//! Called by the destructor.
// **************************************************************
void ForthStream::close()
{
  if (READ_FILE != m_mode)
    return ;

  if (m_infile.is_open())
    {
      m_infile.close();
      m_infile.clear();
      m_mode = NOTHING_TO_READ;
    }
}

// **************************************************************
//! Called by the constructor and ForthStream::setFileToParse()
//! or ForthStream::setStringToParse()
// **************************************************************
void ForthStream::init()
{
  m_cursor_last = m_cursor_next = m_cursor_prev = m_lines = 0;
  m_eol = m_eof = true;
  m_word = "";
  //m_word_picked = true;
}

// **************************************************************
//! \param filename the script Forth stored in an ascii file.
//! \return a boolean indicating if the file could be opned with success.
// **************************************************************
bool ForthStream::loadFile(std::string const& filename)
{
  close();
  init();

  m_filename = filename;
  m_infile.open(filename, std::ios::in);
  if (m_infile.is_open())
    {
      m_mode = READ_FILE;
      refill();
      return true;
    }
  return false;
}

// **************************************************************
//! \param str the script Forth stored as a string.
//! \param name gives a name to the forth script. It's used for
//! example when diplaying an error.
// **************************************************************
void ForthStream::loadString(std::string const& str,
                             std::string const& name)
{
  close();
  init();

  m_mode = READ_STRING;
  m_filename = name;
  m_str = str;
}

// **************************************************************
//!
// **************************************************************
void ForthStream::skipLine()
{
  m_eol = true;
  //++m_lines;

  if (READ_STRING == m_mode)
    {
      // Go to the next end of file markor.
      m_cursor_prev = m_cursor_last;
      m_cursor_last = m_str.find_first_of("\n", m_cursor_last);
      m_cursor_next = m_cursor_last;
      // Reached the end of the string ?
      m_eof = (m_cursor_last == std::string::npos);
    }
  else if (READ_FILE == m_mode)
    {
      refill();
    }
}

// **************************************************************
//!
// **************************************************************
std::string ForthStream::getLine()
{
  std::string line;

  // Extract the line
  m_cursor_last = m_str.find_first_not_of(m_delimiters, m_cursor_last);
  m_cursor_next = m_str.find_first_of("\n", m_cursor_last);
  if ((std::string::npos != m_cursor_next) || (std::string::npos != m_cursor_last))
    {
      line = m_str.substr(m_cursor_last, m_cursor_next - m_cursor_last);
    }

  // Refill data
  m_eol = true;
  m_cursor_prev = m_cursor_last;
  m_cursor_last = m_cursor_next;
  return line;
}

// **************************************************************
//! \return if next line was loaded.
// **************************************************************
bool ForthStream::refill()
{
  // End of line not reached: finish it first
  if (false == m_eol)
    return true;

  // Reading a string or ForthStream not init
  if (READ_FILE != m_mode)
    return false;

  // Trying to read a never opened file
  if (!m_infile.is_open())
    return false;

  m_cursor_prev = m_cursor_last = m_cursor_next = 0;

  // Try reading next line
  if (!std::getline(m_infile, m_str))
    {
      m_eof = true;
      return false;
    }

  // getline succeeded
  m_eol = false;
  ++m_lines;
  return true;
}

// **************************************************************
//! \return true if the stream has more data to extract,
//! else return false.
// **************************************************************
bool ForthStream::hasMoreWords()
{
  bool res;

  if (NOTHING_TO_READ == m_mode)
    return false;

  do {
    // Word extracted ?
    res = split();
    if (res) return true;
    // Not extracted (EOL). Loop while next line available
    res = refill();
  } while (false != res);

  // End of file
  return false;
}

// **************************************************************
//! \return true if a word has been found, else return false.
//! The word extracted is saved as m_word
// **************************************************************
bool ForthStream::split()
{
  // Avoid accident by calling several times hasMoreWords() which
  // gives the side effect to drop previous word
  //if (false == m_word_picked)
  //  {std::cout << "ForthStream::split 1111\n";
  //    return true;}

  // Be sure to read an opened file
  if ((READ_FILE == m_mode) && (!m_infile.is_open()))
    return false;

  // Skip m_delimiters at beginning
  m_cursor_last = m_str.find_first_not_of(m_delimiters, m_cursor_last);

  // Find first "non-delimiter"
  m_cursor_next = m_str.find_first_of(m_delimiters, m_cursor_last);

  if ((std::string::npos != m_cursor_next) || (std::string::npos != m_cursor_last))
    {
      // Found a word
      m_word = m_str.substr(m_cursor_last, m_cursor_next - m_cursor_last);

      // Convert the word to upper case
      //std::transform(m_word.begin(), m_word.end(), m_word.begin(), ::toupper);

      m_cursor_prev = m_cursor_last;
      m_cursor_last = m_cursor_next;

      m_eol = (std::string::npos == m_cursor_next) || (m_str[m_cursor_next] == '\n');
      //std::cout << m_word << " " << m_eol << std::endl;
      return true;
    }

  m_eol = true;
  // Not found
  //m_word_picked = false;

  return false;
}
