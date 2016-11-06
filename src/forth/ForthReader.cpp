#include "ForthReader.hpp"

// **************************************************************
//
// **************************************************************
ForthReader::ForthReader()
{
  m_cursor_last = m_cursor_next = m_what_line = 0;
}

// **************************************************************
//
// **************************************************************
ForthReader::~ForthReader()
{
  if (m_infile.is_open())
    {
      m_infile.close();
      m_infile.clear();
    }
}

// **************************************************************
//
// **************************************************************
bool ForthReader::setFileToParse(std::string const& filename)
{
  m_mode = READ_FILE;
  m_cursor_last = m_cursor_next = m_what_line = 0;
  m_eol = true;
  if (m_infile.is_open())
    {
      m_infile.close();
      m_infile.clear();
    }
  m_infile.open(filename, std::ios::in);
  return m_infile.good();
}

// **************************************************************
//
// **************************************************************
void ForthReader::setStringToParse(std::string const& text)
{
  m_mode = READ_STRING;
  m_cursor_last = m_cursor_next = m_what_line = 0;
  m_eol = false;
  m_text = text;
}

// **************************************************************
//
// **************************************************************
void ForthReader::skipLine()
{
  m_eol = true;
  ++m_what_line;
  m_cursor_last = m_cursor_next = 0;
}

// **************************************************************
//
// **************************************************************
bool ForthReader::nextWord(std::string& word)
{
  std::pair<bool, std::string> res;

  // If read in a file, check if we need to read m_cursor_next line
  if ((READ_FILE == m_mode) && (m_infile.is_open()))
    {
      if (m_eol)
        {
          ++m_what_line;
          m_cursor_last = m_cursor_next = 0;
          m_eol = !(std::getline(m_infile, m_text).good());
        }
    }

  if (!m_eol)
    {
      // Is there still ascii data in the buffer ?
      res = split(m_text);
      m_eol = res.first;
    }
  else
    {
      // We reached the end of the line
      return false;
    }

  //
  word = res.second;

  // Empty line case
  if (0 == word.compare(""))
    return false;

  // Success reading m_cursor_next word
  return true;
}

// **************************************************************
// Find and extract the first forth word if present. A forth word
// is a set of ASCII characters delimited by a space character.
// Return true if reached the end of the string (last word found),
// else false.
// **************************************************************
std::pair<bool, std::string> ForthReader::split(std::string const& text)
{
  std::string delimiter = " \t\n";
  std::string word;

  // Skip spaces
  m_cursor_last = text.find_first_not_of(delimiter, m_cursor_last);

  // Not found (empty string)
  if (m_cursor_last == std::string::npos)
    return std::make_pair(true, std::string(""));

  // Go to the end of the word
  m_cursor_next = text.find_first_of(delimiter, m_cursor_last);
  if (m_cursor_next != std::string::npos)
    {
      // Extract the word which is not the last of the line
      word = text.substr(m_cursor_last, m_cursor_next - m_cursor_last);
      m_cursor_last = m_cursor_next + 1;
      return std::make_pair(false, word);
    }

  // Extract the word which is the last of the line
  word = text.substr(m_cursor_last);
  return std::make_pair(true, word);
}

// **************************************************************
// Return the line and colone
// **************************************************************
std::pair<size_t, size_t> ForthReader::position()
{
  return std::make_pair(m_what_line, m_cursor_next);
}
