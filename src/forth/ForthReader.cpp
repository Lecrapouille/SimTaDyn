#include "ForthReader.hpp"

// **************************************************************
// ForthReader == TokenScanner
// **************************************************************
ForthReader::ForthReader()
{
  m_word.reserve(64U); // 32 is enough for correct Forth words
  init();
  m_mode = READ_STRING;
  m_str = "";
  m_filename = "";
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
void ForthReader::init()
{
  m_cursor_last = m_cursor_next = m_cursor_prev = m_lines = 0;
  m_eol = m_eof = true;
  m_word = "";
  m_word_picked = true;
}

// **************************************************************
//
// **************************************************************
bool ForthReader::setFileToParse(std::string const& filename)
{
  m_mode = READ_FILE;

  // Re-open a file ? Close the old one
  if (m_infile.is_open())
    {
      m_infile.close();
      m_infile.clear();
    }
  m_filename = filename;
  m_infile.open(filename, std::ios::in);

  return m_infile.good() && refill();
}

// **************************************************************
//
// **************************************************************
void ForthReader::setStringToParse(std::string const& str)
{
  init();
  m_mode = READ_STRING;
  m_filename = "";
  m_str = str;
}

// **************************************************************
// Ignore the curent line and go to the next one
// **************************************************************
void ForthReader::skipLine()
{
  m_eol = true;
  ++m_lines;

  if (READ_STRING == m_mode)
    {
      // Go to the next end of file markor.
      m_cursor_prev = m_cursor_last;
      m_cursor_last = m_str.find_first_of("\n", m_cursor_last);
      m_cursor_next = m_cursor_last;
      // Reached the end of the string ?
      m_eof = (m_cursor_last == std::string::npos);
    }
  else // if (READ_FILE == m_mode)
    {
      refill();
    }
}

// **************************************************************
// Read next line of file
// **************************************************************
bool ForthReader::refill()
{
  // End of line not reached: finish it first
  if (false == m_eol)
    return true;

  // Reading a string or ForthReader not init
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
// Check if a word can be extracted
// **************************************************************
bool ForthReader::hasMoreWords()
{
  bool res;

  do {
    // Get next word ?
    res = split();
    if (res) {
      return true; // extracted
    }

    // Not extracted (EOL). Loop while next line available
    res = refill();
  } while (false != res);

  // End of file
  return false;
}

// **************************************************************
//
// **************************************************************
std::string ForthReader::nextWord()
{
  //m_word_picked = true;
  return m_word;
}

// **************************************************************
// Read the next word delimited by space characters.
// Return true if a word has been found, else return false.
// The word extracted is saved as m_word
// **************************************************************
bool ForthReader::split()
{
  // Avoid accident by calling several times hasMoreWords() which
  // gives the side effect to drop previous word
  //if (false == m_word_picked)
  //  {std::cout << "ForthReader::split 1111\n";
  //    return true;}

  // Be sure to read an opened file
  if ((READ_FILE == m_mode) && (!m_infile.is_open()))
    return false;

  const std::string delimiters = " \t\r\n";

  // Skip delimiters at beginning
  m_cursor_last = m_str.find_first_not_of(delimiters, m_cursor_last);

  // Find first "non-delimiter"
  m_cursor_next = m_str.find_first_of(delimiters, m_cursor_last);

  if ((std::string::npos != m_cursor_next) || (std::string::npos != m_cursor_last))
    {
      // Found a word
      m_word = m_str.substr(m_cursor_last, m_cursor_next - m_cursor_last);

      m_cursor_prev = m_cursor_last;
      m_cursor_last = m_cursor_next;
      return true;
    }

  // Not found
  m_eol = true;
  m_word_picked = false;

  //
  return !m_eol;
}

// **************************************************************
// Return the current line and colon
// **************************************************************
std::pair<size_t, size_t> ForthReader::cursors()
{
  return std::make_pair(m_lines, m_cursor_prev + 1U);
}

// **************************************************************
// Return the filename currently reading, else return "" if reading
// a string.
// **************************************************************
std::string ForthReader::file()
{
  return m_filename;
}
