#ifndef FORTH_READER_HPP_
#  define FORTH_READER_HPP_

#  include "ForthHelper.hpp"

class ForthReader
{
public:
  ForthReader();
  ~ForthReader();
  bool setFileToParse(std::string const& filename);
  void setStringToParse(std::string const& text);
  void skipLine();
  bool nextWord(std::string& word);
  std::pair<size_t, size_t> position();

protected:
  std::pair<bool, std::string> split(std::string const& text);

  enum Mode
  {
    READ_FILE = 0,
    READ_STRING
  };

  std::string m_text;     // The Forth code
  size_t m_cursor_last;   // Split iterator on Forth words
  size_t m_cursor_next;   // Split iterator on Forth words
  size_t m_what_line;     // The line
  bool m_eol;             // End of line reached ?
  std::ifstream m_infile; // Opened file
  enum Mode m_mode;       // Read from file or text
};

#endif /* FORTH_READER_HPP_ */
